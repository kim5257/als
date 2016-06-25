/*
 * watch_fd.h
 *
 *  Created on: 2015. 7. 8.
 *      Author: asran
 */

#ifndef WATCH_FD_H_
#define WATCH_FD_H_

#include <list>
#include <string>

namespace inotify
{

class WatchFD
{
public:
	typedef std::list<WatchFD*> watch_fd_list;
	typedef watch_fd_list::iterator watch_fd_list_itor;
private:
	int mFD;
	int mWD;
	std::string mFileName;
	WatchFD* mParent;
	watch_fd_list mChilds;
private:
	WatchFD (	int notifyFD,
				std::string fileName,
				WatchFD* parent);
	virtual
	~WatchFD (void);
private:
	void
	init (void);
	void
	deinit (void);
public:
	void
	addChild (WatchFD* child);
	void
	delChild (WatchFD* child);
	watch_fd_list_itor
	getChild (std::string name);
	void
	start (void);
	void
	stop (void);
	std::string
	getPath (void) const;
public:
	inline int
	getFD (void) const;
	inline int
	getWD (void) const;
	inline void
	setFileName (std::string fileName);
	inline std::string
	getFileName (void) const;
	inline void
	setParent (WatchFD* parent);
	inline WatchFD*
	getParent (void) const;
	inline watch_fd_list&
	getChilds (void);
	inline bool
	isFound (const watch_fd_list_itor& itor) const;
public:
	inline
	operator int () const;
public:
	static WatchFD*
	create (int notifyFD,
			std::string fileName,
			WatchFD* parent);
	static void
	destroy (WatchFD* watch);
};

int
WatchFD::getFD (void) const
{
	return mFD;
}

int
WatchFD::getWD (void) const
{
	return mWD;
}

void
WatchFD::setFileName (std::string fileName)
{
	mFileName = fileName;
}

std::string
WatchFD::getFileName (void) const
{
	return mFileName;
}

void
WatchFD::setParent (WatchFD* parent)
{
	mParent = parent;
}

WatchFD*
WatchFD::getParent (void) const
{
	return mParent;
}

WatchFD::watch_fd_list&
WatchFD::getChilds (void)
{
	return mChilds;
}

bool
WatchFD::isFound (const WatchFD::watch_fd_list_itor& itor) const
{
	return (mChilds.end () != itor)? (true):(false);
}

WatchFD::operator int () const
{
	return getWD ();
}

}

#endif /* WATCH_FD_H_ */
