#pragma once

#include "animvalue.hh"
#include "fs.hh"
#include "song.hh"
#include <boost/shared_ptr.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/thread.hpp>
#include <deque>
#include <set>

/// songs class for songs screen
class Songs: boost::noncopyable {
  public:
	/// constructor
	Songs(std::string const& songlist = std::string());
	~Songs();
	/// updates filtered songlist
	void update() { if (m_dirty) filter_internal(); }
	/// reloads songlist
	void reload();
	/// array access
	Song& operator[](std::size_t pos) { return *m_filtered[pos]; }
	/// number of songs
	int size() const { return m_filtered.size(); };
	/// true if empty
	int empty() const { return m_filtered.empty(); };
	/// advances to next song
	void advance(int diff) {
		int size = m_filtered.size();
		if (size == 0) return;  // Do nothing if no songs are available
		int _current = size ? (int(math_cover.getTarget()) + diff) % size : 0;
		if (_current < 0) _current += m_filtered.size();
		math_cover.setTarget(_current,this->size());
	}
	/// get current id
	int currentId() const { return math_cover.getTarget(); }
	/// gets current position
	double currentPosition() { return math_cover.getValue(); }
	/// gets current velocity
	double currentVelocity() const { return math_cover.getVelocity(); }
	/// sets margins for animation
	void setAnimMargins(double left, double right) { math_cover.setMargins(left, right); }
	/// @return current song
	Song& current() { return *m_filtered[math_cover.getTarget()]; }
	/// @return current Song
	Song const& current() const { return *m_filtered[math_cover.getTarget()]; }
	/// filters songlist by regular expression
	void setFilter(std::string const& regex);
	/// sort descending
	std::string sortDesc() const;
	/// randomizes songlist
	void randomize();
	/// randomizes if ordered
	void random() { if (m_order) randomize(); advance(1); }
	/// changes sorting
	void sortChange(int diff);
	/// parses file into Song &tmp
	void parseFile(Song& tmp);

  private:
	class RestoreSel;
	typedef std::vector<boost::shared_ptr<Song> > SongVector;
	typedef std::set<fs::path> SongDirs;
	SongDirs m_songdirs;
	std::string m_songlist;
	SongVector m_songs, m_filtered;
	AnimAcceleration math_cover;
	std::string m_filter;
	int m_order;
	void dumpSongs_internal() const;
	void reload_internal();
	void reload_internal(fs::path const& p);
	void randomize_internal();
	void filter_internal();
	void sort_internal();
	volatile bool m_dirty;
	volatile bool m_loading;
	boost::scoped_ptr<boost::thread> m_thread;
	mutable boost::mutex m_mutex;
};
