#ifndef __SCREENCONFIGURATION_H__
#define __SCREENCONFIGURATION_H__

#include "screen.hh"
#include "configuration.hh"
#include "theme.hh"
#include <boost/scoped_ptr.hpp>
#include <boost/ptr_container/ptr_vector.hpp>
#include "opengl_text.hh"

/// options dialogue
class ScreenConfiguration: public Screen {
  public:
	/// constructor
	ScreenConfiguration(std::string const& name, Audio& m_audio);
	void enter();
	void exit();
	void manageEvent(SDL_Event event);
	void draw();

  private:
	Audio& m_audio;
	boost::scoped_ptr<ThemeConfiguration> theme;
	boost::ptr_vector<Configuration> configuration;
	unsigned int selected;
};

#endif