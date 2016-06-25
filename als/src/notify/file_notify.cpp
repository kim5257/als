/*
 * file_notify.cpp
 *
 *  Created on: 2015. 7. 9.
 *      Author: asran
 */

#include "notify/file_notify.h"

#include <stack>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <pthread.h>
#include <linux/limits.h>
#include <sys/select.h>
#include <sys/inotify.h>
#include "error/err_msg.h"
#include <fstream>

#define	IS_MASK(mask, val)	((mask & val) != 0)

namespace inotify
{

FileNotify::FileNotify (std::string rootPath)
		: mRoot (NULL),
			mEventThreadId (0),
			mEventThreadExit (false)
{
	init (rootPath);
}

FileNotify::~FileNotify (void)
{
	deinit ();
}

void
FileNotify::init (std::string rootPath)
{
	setRootPath (rootPath);
}

void
FileNotify::deinit (void)
{

}

void
FileNotify::setRootPath (std::string path)
{
	mRoot = addPath (NULL, path);
	addRecursivePath (mRoot);
}

WatchFD*
FileNotify::addPath (	WatchFD* parent,
						std::string fileName)
{
	WatchFD* watch = NULL;
	NotifyFD* notifyFD = NULL;

	for(size_t cnt = 0;cnt < MAX_NOTIFY_FD;++cnt)
	{
		if ( mNotifyFD[cnt].isCanAdded () )
		{
			notifyFD = &mNotifyFD[cnt];
			break;
		}
	}

	if ( notifyFD == NULL )
	{
		throw error::ErrMsg::createErrMsg ("더이상 경로를 추가 할 수 없습니다.");
	}

	watch = *notifyFD->addWatch (parent, fileName);

	return watch;
}

void
FileNotify::delPath (WatchFD* watch)
{
	for(size_t cnt = 0;cnt < MAX_NOTIFY_FD;++cnt)
	{
		if ( mNotifyFD[cnt].getFD () == watch->getFD () )
		{
			// 찾았으면, 해당 watch 제거
			if ( watch->getParent () != NULL )
			{
				watch->getParent ()->delChild (watch);
			}

			mNotifyFD[cnt].delWatch (watch);
		}
	}
}

void
FileNotify::delPathAll (WatchFD* baseWatch)
{
	std::stack<WatchFD*> wstack;
	std::stack<WatchFD::watch_fd_list_itor> istack;

	if ( baseWatch->getParent () != NULL )
	{
		baseWatch->getParent ()->delChild (baseWatch);
	}

	wstack.push (baseWatch);
	istack.push (baseWatch->getChilds ().begin ());

	while( !wstack.empty () )
	{
		if ( wstack.top ()->getChilds ().end () != istack.top () )
		{
			wstack.push ((*istack.top ()));

			++istack.top ();

			istack.push (wstack.top ()->getChilds ().begin ());
		}
		else
		{
			delPath (wstack.top ());

			istack.pop ();
			wstack.pop ();
		}
	}
}

void
FileNotify::doDefEvent (NotifyFD& fileNotify,
						inotify_event* event)
{
	WatchFD* watch = *fileNotify.getWatch (event->wd);

	if ( watch != NULL )
	{
		std::string path = watch->getPath () + '/' + event->name;

		if ( IS_MASK(IN_ACCESS, event->mask) )
		{
			printf ("%-15s : %s\n", "Access", path.c_str ());
		}
		else if ( IS_MASK(IN_MODIFY, event->mask) )
		{
			printf ("%-15s : %s\n", "Modify", path.c_str ());
		}
		else if ( IS_MASK(IN_ATTRIB, event->mask) )
		{
			printf ("%-15s : %s\n", "Attrib", path.c_str ());
		}
		else if ( IS_MASK(IN_CLOSE_WRITE, event->mask) )
		{
			printf ("%-15s : %s\n", "Close write", path.c_str ());
		}
		else if ( IS_MASK(IN_CLOSE_NOWRITE, event->mask) )
		{
			printf ("%-15s : %s\n", "Close no write", path.c_str ());
		}
		else if ( IS_MASK(IN_OPEN, event->mask) )
		{
			printf ("%-15s : %s\n", "Open", path.c_str ());
		}
		else if ( IS_MASK(IN_MOVED_FROM, event->mask) )
		{
			printf ("%-15s [%08x][%08x]: %s\n",
					"Move from",
					event->cookie,
					event->mask,
					path.c_str ());

			OnMoveFrom (watch, event->name, event->cookie);
		}
		else if ( IS_MASK(IN_MOVED_TO, event->mask) )
		{
			printf ("%-15s [%08x][%08x]: %s\n",
					"Move to",
					event->cookie,
					event->mask,
					path.c_str ());

			OnMoveTo (watch, event->name, event->cookie);
		}
		else if ( IS_MASK(IN_CREATE, event->mask) )
		{
			printf ("%-15s [%08x][%08x]: %s\n",
					"Create",
					event->cookie,
					event->mask,
					path.c_str ());

			OnCreate (watch, event->name);
		}
		else if ( IS_MASK(IN_DELETE, event->mask) )
		{
			printf ("%-15s : %s\n", "Delete", path.c_str ());
		}
		else if ( IS_MASK(IN_DELETE_SELF, event->mask) )
		{
			printf ("%-15s : %s\n", "Delete self", path.c_str ());
		}
		else if ( IS_MASK(IN_MOVE_SELF, event->mask) )
		{
			printf ("%-15s : %s\n", "Move self", path.c_str ());
		}
	}
}

void
FileNotify::doEvent (	NotifyFD& fileNotify,
						inotify_event* event)
{
	WatchFD* watch = *fileNotify.getWatch (event->wd);

	if ( watch != NULL )
	{
		std::string path = watch->getPath ();
		std::string fileName = event->name;

		if ( mNotifyEvent != NULL )
		{
			if ( IS_MASK(IN_ACCESS, event->mask) )
			{
				mNotifyEvent->OnAccess (path, fileName);
			}
			else if ( IS_MASK(IN_MODIFY, event->mask) )
			{
				mNotifyEvent->OnModify (path, fileName);
			}
			else if ( IS_MASK(IN_ATTRIB, event->mask) )
			{
				mNotifyEvent->OnAttrib (path, fileName);
			}
			else if ( IS_MASK(IN_CLOSE_WRITE, event->mask) )
			{
				mNotifyEvent->OnCloseWrite (path, fileName);
			}
			else if ( IS_MASK(IN_CLOSE_NOWRITE, event->mask) )
			{
				mNotifyEvent->OnCloseNoWrite (path, fileName);
			}
			else if ( IS_MASK(IN_OPEN, event->mask) )
			{
				mNotifyEvent->OnOpen (path, fileName);
			}
			else if ( IS_MASK(IN_MOVED_FROM, event->mask) )
			{
				mNotifyEvent->OnMoveFrom (path, fileName);
			}
			else if ( IS_MASK(IN_MOVED_TO, event->mask) )
			{
				mNotifyEvent->OnMoveTo (path, fileName);
			}
			else if ( IS_MASK(IN_CREATE, event->mask) )
			{
				mNotifyEvent->OnCreate (path, fileName);
			}
			else if ( IS_MASK(IN_DELETE, event->mask) )
			{
				mNotifyEvent->OnDelete (path, fileName);
			}
			else if ( IS_MASK(IN_DELETE_SELF, event->mask) )
			{
				mNotifyEvent->OnDeleteSelf (path, fileName);
			}
			else if ( IS_MASK(IN_MOVE_SELF, event->mask) )
			{
				mNotifyEvent->OnMoveSelf (path, fileName);
			}
		}
	}
}

void
FileNotify::doMoveTimeout (void)
{
	std::list<MoveInfo>::iterator itor = mMoveList.begin ();

	for(;itor != mMoveList.end ();++itor)
	{
		itor->mTimeout -= (EVENT_TIMEOUT * 1000);
		if ( itor->mTimeout < 0 )
		{
			delPathAll (itor->mWatch);
			itor = mMoveList.erase (itor);
		}
	}
}

void
FileNotify::OnCreate (	WatchFD* watch,
						std::string fileName)
{
	WatchFD* newWatch = NULL;

	newWatch = addPath (watch, fileName);
	newWatch->start ();
}

void
FileNotify::OnDelete (	WatchFD* watch,
						std::string fileName)
{
	delPath (*watch->getChild (fileName));
}

void
FileNotify::OnMoveFrom (WatchFD* watch,
						std::string fileName,
						uint32_t cookie)
{
	MoveInfo moveInfo;

	moveInfo.mCookie = cookie;
	moveInfo.mWatch = *watch->getChild (fileName);
	moveInfo.mTimeout = IN_MOVE_TIMEOUT;

	mMoveList.push_back (moveInfo);
}

void
FileNotify::OnMoveTo (	WatchFD* watch,
						std::string fileName,
						uint32_t cookie)
{
	std::list<MoveInfo>::iterator itor = mMoveList.begin ();

	for(;itor != mMoveList.end ();++itor)
	{
		if ( itor->mCookie == cookie )
		{
			break;
		}
	}

	if ( itor != mMoveList.end () )
	{
		// 이동 처리 수행
		WatchFD* moveWatch = itor->mWatch;

		moveWatch->setFileName (fileName);

		// 부모 watch가 다르면 변경
		if ( watch != moveWatch->getParent () )
		{
			moveWatch->getParent ()->delChild (moveWatch);
			moveWatch->setParent (watch);
			watch->addChild (moveWatch);
		}

		mMoveList.erase (itor);
	}
	else
	{
		// 생성 처리 수행
		OnCreate (watch, fileName);
		addRecursivePath (*watch->getChild (fileName));
	}
}

void
FileNotify::addRecursivePath (WatchFD* baseWatch)
{
	std::stack<DIR*> dirStack;
	DIR* dir = NULL;
	WatchFD* curWatch = NULL;
	WatchFD* watch = NULL;

	std::string startPath = baseWatch->getPath ();
	dir = opendir (startPath.c_str ());
	dirStack.push (dir);

	curWatch = baseWatch;
	while( !dirStack.empty () )
	{
		dirent* dirEntry = NULL;

		while( (dirEntry = readdir (dirStack.top ())) != NULL )
		{
			if ( dirEntry->d_type == DT_DIR )
			{
				// . / .. 폴더는 제외
				if ( (strncmp (dirEntry->d_name, ".", NAME_MAX) != 0)
						&& (strncmp (dirEntry->d_name, "..", NAME_MAX) != 0) )
				{
					watch = addPath (curWatch, dirEntry->d_name);
					watch->start ();

					std::string path = watch->getPath ();
					dir = opendir (path.c_str ());
					dirStack.push (dir);

					curWatch = watch;
				}
			}
		}

		curWatch = curWatch->getParent ();
		closedir (dirStack.top ());
		dirStack.pop ();
	}

	// 테스트
	{
		std::stack<WatchFD*> wstack;
		std::stack<WatchFD::watch_fd_list_itor> istack;

		wstack.push (baseWatch);
		istack.push (baseWatch->getChilds ().begin ());

		while( !wstack.empty () )
		{
			if ( wstack.top ()->getChilds ().end () != istack.top () )
			{
				wstack.push ((*istack.top ()));

				++istack.top ();

				istack.push (wstack.top ()->getChilds ().begin ());
			}
			else
			{
				std::string path = wstack.top ()->getPath ();
				printf ("%s\n", path.c_str ());

				istack.pop ();
				wstack.pop ();
			}
		}
	}
}

void
FileNotify::setOnFileNotifyEvent (OnFileNotifyEvent* event)
{
	mNotifyEvent = event;
}

void
FileNotify::start (void)
{
	int ret = 0;

	for(size_t cnt = 0;cnt < MAX_NOTIFY_FD;++cnt)
	{
		mNotifyFD[cnt].startWatch ();
	}

	mEventThreadExit = false;
	ret = pthread_create (&mEventThreadId,
	NULL,
							FileNotify::doEvent_, this);
	if ( ret != 0 )
	{
		mEventThreadExit = true;
		throw error::ErrMsg::createErrMsg ("Create thread failed.");
	}
}

void
FileNotify::stop (void)
{
	void* ret = NULL;

	mEventThreadExit = true;
	pthread_join (mEventThreadId, &ret);

	for(size_t cnt = 0;cnt < MAX_NOTIFY_FD;++cnt)
	{
		mNotifyFD[cnt].stopWatch ();
	}

	if ( ret != NULL )
	{
		throw error::ErrMsg::createErrMsg ("Occurred error when stopping thread.");
	}
}

void*
FileNotify::doEvent_ (void* pThis)
{
	void* ret = NULL;
	FileNotify* fileNotify = (FileNotify*)pThis;

	char buf[INOTIFY_BUF_LEN] =
	{ 0, };
	fd_set fdSetOrg;
	timeval timeoutOrg;

	timeoutOrg.tv_sec = 0;
	timeoutOrg.tv_usec = 10000;	// 10ms

	FD_ZERO(&fdSetOrg);
	for(size_t cnt = 0;cnt < MAX_NOTIFY_FD;++cnt)
	{
		FD_SET(fileNotify->mNotifyFD[cnt].getFD (), &fdSetOrg);
	}

	while( !fileNotify->mEventThreadExit )
	{
		fd_set fdSet = fdSetOrg;
		timeval timeout = timeoutOrg;
		int eventCnt = 0;

		eventCnt = select (FD_SETSIZE, &fdSet, NULL, NULL, &timeout);
		if ( 0 < eventCnt )
		{
			for(size_t cnt = 0;cnt < MAX_NOTIFY_FD;++cnt)
			{
				if ( FD_ISSET(fileNotify->mNotifyFD[cnt].getFD(), &fdSet) != 0 )
				{
					// 이벤트 처리
					int len = read (fileNotify->mNotifyFD[cnt].getFD (),
									buf,
									sizeof(buf));
					int offset = 0;

					while( offset < len )
					{
						inotify_event* event = (inotify_event*)&buf[offset];

						if ( 0 < event->len )
						{
							fileNotify->doDefEvent (fileNotify->mNotifyFD[cnt],
													event);
							fileNotify->doEvent (	fileNotify->mNotifyFD[cnt],
													event);
						}

						offset += sizeof(inotify_event) + event->len;
					}
				}
			}
		}
		else
		{
			// 기타 처리
			fileNotify->doMoveTimeout ();
		}
	}

	return ret;
}

FileNotify*
FileNotify::create (std::string rootPath)
{
	FileNotify* fileNotify = NULL;

	fileNotify = new FileNotify (rootPath);

	return fileNotify;
}

void
FileNotify::destroy (FileNotify* fileNotify)
{
	delete fileNotify;
}

}
