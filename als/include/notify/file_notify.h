/*
 * file_notify.h
 *
 *  Created on: 2015. 7. 9.
 *      Author: asran
 */

#ifndef FILE_NOTIFY_H_
#define FILE_NOTIFY_H_

#include <notify/notify_fd.h>
#include <notify/notify_setting.h>
#include <notify/on_file_notify_event.h>

namespace inotify
{

class	FileNotify
{
private:
	struct	MoveInfo
	{
		uint32_t	mCookie;
		WatchFD*	mWatch;
		int32_t	mTimeout;
	};
private:
	NotifyFD	mNotifyFD[MAX_NOTIFY_FD];
	WatchFD*	mRoot;
	pthread_t	mEventThreadId;
	bool		mEventThreadExit;
	OnFileNotifyEvent*	mNotifyEvent;
	std::list<MoveInfo>	mMoveList;
private:
				FileNotify		(	std::string	rootPath	);
	virtual	~FileNotify	(	void	);
private:
	void	init	(	std::string	path	);
	void	deinit	(	void	);
	void	setRootPath	(	std::string	path	);
	WatchFD*	addPath	(	WatchFD*		parent,
								std::string	fileName
							);
	void	delPath		(	WatchFD*		watch	);
	void	delPathAll		(	WatchFD*		baseWatch	);
	void	doDefEvent		(	NotifyFD&			fileNotify,
								inotify_event*	event
							);
	void	doEvent		(	NotifyFD&			fileNotify,
								inotify_event*	event
							);
	void	doMoveTimeout	(	void	);
	void	OnCreate		(	WatchFD*			watch,
								std::string		fileName
							);
	void	OnDelete		(	WatchFD*			watch,
								std::string		fileName
							);
	void	OnMoveFrom		(	WatchFD*			watch,
								std::string		fileName,
								uint32_t			cookie
							);
	void	OnMoveTo		(	WatchFD*			watch,
								std::string		fileName,
								uint32_t			cookie
							);
public:
	void	addRecursivePath	(	WatchFD*	baseWatch	);
	void	setOnFileNotifyEvent		(	OnFileNotifyEvent*	event	);
	void	start	(	void	);
	void	stop	(	void	);
public:
	inline WatchFD*	getRoot	(	void	);
private:
	static void*	doEvent_	(	void*	pThis	);
public:
	static FileNotify*	create		(	std::string	rootPath	);
	static void			destroy	(	FileNotify*	fileNotify	);
};

inline WatchFD*	FileNotify::getRoot	(	void	)
{
	return	mRoot;
}

}

#endif /* FILE_NOTIFY_H_ */
