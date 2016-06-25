/*
 * notify_fd.h
 *
 *  Created on: 2015. 7. 8.
 *      Author: asran
 */

#ifndef NOTIFY_FD_H_
#define NOTIFY_FD_H_

#include <unistd.h>

#include <notify/notify_setting.h>
#include <notify/watch_fd.h>

namespace inotify
{

class NotifyFD
{
private:
	int mFD;
	WatchFD::watch_fd_list mWatchList;
public:
	NotifyFD (void);
	virtual
	~NotifyFD (void);
private:
	void
	init (void);
	void
	deinit (void);
public:
	WatchFD::watch_fd_list_itor
	addWatch (	WatchFD* parent,
				std::string fileName);
	WatchFD::watch_fd_list_itor
	getWatch (int wd);

	void
	delWatch (WatchFD* watch);
	void
	delWatch (WatchFD::watch_fd_list_itor itor);
	void
	startWatch (void);
	void
	stopWatch (void);
public:
	inline int
	getFD (void) const;
	inline bool
	isFound (const WatchFD::watch_fd_list_itor& itor) const;
	inline bool
	isCanAdded (void) const;
public:
	inline
	operator int () const;
};

int
NotifyFD::getFD (void) const
{
	return mFD;
}

bool
NotifyFD::isFound (const WatchFD::watch_fd_list_itor& itor) const
{
	return (mWatchList.end () != itor)? (true):(false);
}

bool
NotifyFD::isCanAdded (void) const
{
	return (mWatchList.size () < MAX_WATCH_FD)? (true):(false);
}

NotifyFD::operator int () const
{
	return getFD ();
}

}

#endif /* NOTIFY_FD_H_ */
