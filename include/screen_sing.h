#ifndef __SCREENSING_H__
#define __SCREENSING_H__

#include "../config.h"

#include <screen.h>
#include <pitch_graph.h>
#include <theme.h>
#include <video.h>
#include <lyrics.h>

class CScreenSing: public CScreen {
  public:
	CScreenSing(std::string const& name, unsigned int width, unsigned int height, Analyzer const& analyzer);
	~CScreenSing();
	void enter();
	void exit();
	void manageEvent(SDL_Event event);
	void draw();
  private:
	Analyzer const& m_analyzer;
	SDL_Surface* videoSurf;
	SDL_Surface* backgroundSurf;
	unsigned int backgroundSurf_id;
	unsigned int theme_id;
	unsigned int pitchGraph_id;
	// Keeps the pitch tracking graphics
	// in separate surface
	PitchGraph pitchGraph;
	std::vector<Note> m_sentence;
	bool play;
	bool finished;
	double playOffset;
	CVideo* video;
	CThemeSing* theme;
	Lyrics* lyrics;
};

#endif