#include "screen_practice.hh"

#include "util.hh"
#include "joystick.hh"
#include "configuration.hh"

ScreenPractice::ScreenPractice(std::string const& name, Audio& audio, Capture& capture):
  Screen(name), m_audio(audio), m_capture(capture)
{}

void ScreenPractice::enter() {
	probe();
	m_audio.playMusic(getThemePath("practice.ogg"));
	theme.reset(new ThemePractice());
	// draw vu meters
	for (unsigned int i = 0, mics = m_capture.analyzers().size(); i < mics; ++i) {
		ProgressBar* b;
		m_vumeters.push_back(b = new ProgressBar(getThemePath("vumeter_bg.svg"), getThemePath("vumeter_fg.svg"), ProgressBar::VERTICAL, 0.136, 0.023));
		b->dimensions.screenBottom().left(-0.4 + i * 0.2).fixedWidth(0.04);
	}
}

void ScreenPractice::exit() {
	m_vumeters.clear();
	theme.reset();
}

void ScreenPractice::manageEvent(SDL_Event event) {
	ScreenManager * sm = ScreenManager::getSingletonPtr();
	if (event.type == SDL_KEYDOWN) {
		switch(event.key.keysym.sym) {
			case SDLK_ESCAPE:
			case SDLK_q:
				sm->activateScreen("Intro");
				break;
			case SDLK_SPACE:
			case SDLK_PAUSE:
				m_audio.togglePause();
				break;
			default: // nothing to do, fixes warnings
				break;
		}
	}
	switch( event.type ) {
		case SDL_JOYAXISMOTION:
		case SDL_JOYHATMOTION:
		case SDL_JOYBALLMOTION:
		case SDL_JOYBUTTONDOWN:
		case SDL_JOYBUTTONUP:
			//check_joystick_event(event, m_audio);
			break;
	}
}

void ScreenPractice::draw() {
	theme->bg.draw();
	this->draw_analyzers();
}

void ScreenPractice::draw_analyzers() {
	boost::ptr_vector<Analyzer>& analyzers = m_capture.analyzers();
	if (analyzers.empty()) return;
	bool text = false;

	for (unsigned int i = 0; i < analyzers.size(); ++i) {
		Analyzer& analyzer = analyzers[i];
		analyzer.process();
		Tone const* tone = analyzer.findTone();
		double freq = (tone ? tone->freq : 0.0);
		MusicalScale scale;
		// getPeak returns 0.0 when clipping, negative values when not that loud.
		// Normalizing to [0,1], where 0 is -43 dB or less (to match the vumeter graphic)
		m_vumeters[i].draw(analyzer.getPeak() / 43.0 + 1.0);

		if (freq != 0.0) {
			Analyzer::tones_t tones = analyzer.getTones();

			for (Analyzer::tones_t::const_iterator t = tones.begin(); t != tones.end(); ++t) {
				if (t->age < Tone::MINAGE) continue;
				int note = scale.getNoteId(t->freq);
				if (note < 0) continue;
				int octave = note / 12 - 1;
				double noteOffset = scale.getNoteNum(note);
				bool sharp = scale.isSharp(note);
				noteOffset += octave*7;
				noteOffset += 0.4 * scale.getNoteOffset(t->freq);
				float posXnote = -0.25 + 0.2 * i + 0.002 * t->stabledb;
				float posYnote = .075-noteOffset*0.015;

				theme->note.dimensions.left(posXnote).center(posYnote);
				theme->note.draw();
				if (sharp) {
					theme->sharp.dimensions.right(posXnote).center(posYnote);
					theme->sharp.draw();
				}
			}

			if (!text) {
				theme->note_txt.draw(scale.getNoteStr(freq));
				text = true;
			}
		}
	}

}