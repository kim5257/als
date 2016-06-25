/*
 * notify_fd.cpp
 *
 *  Created on: 2015. 7. 8.
 *      Author: asran
 */

#include "notify/notify_fd.h"

#include <sys/inotify.h>
#include "error/err_msg.h"
#include "notify/notify_setting.h"

namespace inotify
{

NotifyFD::NotifyFD (void)
		: mFD (0)
{
	init ();
}

NotifyFD::~NotifyFD (void)
{
	deinit ();
}

void
NotifyFD::init (void)
{
	mFD = inotify_init ();
}

void
NotifyFD::deinit (void)
{
	WatchFD::watch_fd_list_itor itor = mWatchList.begin ();

	for(;itor != mWatchList.end ();++itor)
	{
		WatchFD::destroy (*itor);
	}

	mWatchList.clear ();

	close (mFD);
}

WatchFD::watch_fd_list_itor
NotifyFD::addWatch (WatchFD* parent,
					std::string fileName)
{
	WatchFD::watch_fd_list_itor watch_itor = mWatchList.end ();

	if ( mWatchList.size () < MAX_WATCH_FD )
	{
		WatchFD* watch = WatchFD::create (this->getFD (), fileName, parent);

		if ( parent != NULL )
		{
			parent->addChild (watch);
		}
		mWatchList.push_back (watch);
		watch_itor = mWatchList.end ();
		--watch_itor;
	}

	return watch_itor;
}

WatchFD::watch_fd_list_itor
NotifyFD::getWatch (int wd)
{
	WatchFD::watch_fd_list_itor watch_itor = mWatchList.end ();
	WatchFD::watch_fd_list_itor itor = mWatchList.begin ();
	for(;itor != mWatchList.end ();++itor)
	{
		if ( wd == (*itor)->getWD () )
		{
			watch_itor = itor;
			break;
		}
	}

	return watch_itor;
}

void
NotifyFD::delWatch (WatchFD* watch)
{
	WatchFD::destroy (watch);
	mWatchList.remove (watch);
}

void
NotifyFD::delWatch (WatchFD::watch_fd_list_itor itor)
{
	WatchFD::destroy (*itor);
	mWatchList.erase (itor);
}

void
NotifyFD::startWatch (void)
{
	WatchFD::watch_fd_list_itor itor = mWatchList.begin ();

	for(;itor != mWatchList.end ();++itor)
	{
		(*itor)->start ();
	}
}

void
NotifyFD::stopWatch (void)
{
	WatchFD::watch_fd_list_itor itor = mWatchList.begin ();

	for(;itor != mWatchList.end ();++itor)
	{
		(*itor)->stop ();
	}
}

}
