/*
 * watch_fd.cpp
 *
 *  Created on: 2015. 7. 8.
 *      Author: asran
 */

#include "notify/watch_fd.h"

#include <stdio.h>
#include <sys/inotify.h>
#include "error/err_msg.h"

namespace inotify
{

WatchFD::WatchFD (	int notifyFD,
					std::string fileName,
					WatchFD* parent)
		: mFD (notifyFD),
			mWD (0),
			mFileName (fileName),
			mParent (parent)
{
	init ();
}

WatchFD::~WatchFD (void)
{
	deinit ();
}

void
WatchFD::init (void)
{

}

void
WatchFD::deinit (void)
{
	stop ();
}

void
WatchFD::addChild (WatchFD* child)
{
	if ( this != child->getParent () )
	{
		throw error::ErrMsg::createErrMsg ("추가하려는 자식 폴더의 부모가 아닙니다.");
	}
	mChilds.push_back (child);
}

void
WatchFD::delChild (WatchFD* child)
{
	mChilds.remove (child);
}

WatchFD::watch_fd_list_itor
WatchFD::getChild (std::string name)
{
	watch_fd_list_itor itor = mChilds.begin ();

	for(;itor != mChilds.end ();++itor)
	{
		if ( (*itor)->getFileName () == name )
		{
			break;
		}
	}

	return itor;
}

void
WatchFD::start (void)
{
	std::string path = getPath ();
	if ( mWD == 0 )
	{
		mWD = inotify_add_watch (getFD (), path.c_str (), IN_ALL_EVENTS);
	}
}

void
WatchFD::stop (void)
{
	inotify_rm_watch (getFD (), getWD ());
	mWD = 0;
}

std::string
WatchFD::getPath (void) const
{
	std::string path = mFileName;
	WatchFD* parent = getParent ();

	while( parent != NULL )
	{
		path = parent->getFileName () + '/' + path;
		parent = parent->getParent ();
	}

	return path;
}

WatchFD*
WatchFD::create (	int notifyFD,
					std::string fileName,
					WatchFD* parent)
{
	WatchFD* watch = NULL;

	watch = new WatchFD (notifyFD, fileName, parent);

	return watch;
}

void
WatchFD::destroy (WatchFD* watch)
{
	delete watch;
}

}
