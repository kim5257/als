/*
 * on_file_notify_event.h
 *
 *  Created on: 2015. 7. 10.
 *      Author: asran
 */

#ifndef ON_FILE_NOTIFY_EVENT_H_
#define ON_FILE_NOTIFY_EVENT_H_

#include <string>
#include <sys/inotify.h>

namespace inotify
{

class OnFileNotifyEvent
{
public:
	OnFileNotifyEvent ()
	{
	}
	virtual
	~OnFileNotifyEvent ()
	{
	}
public:
	virtual void
	OnAccess (	std::string path,
				std::string fileName)=0;
	virtual void
	OnModify (	std::string path,
				std::string fileName)=0;
	virtual void
	OnAttrib (	std::string path,
				std::string fileName)=0;
	virtual void
	OnCloseWrite (	std::string path,
					std::string fileName)=0;
	virtual void
	OnCloseNoWrite (std::string path,
					std::string fileName)=0;
	virtual void
	OnOpen (std::string path,
			std::string fileName)=0;
	virtual void
	OnMoveFrom (std::string path,
				std::string fileName)=0;
	virtual void
	OnMoveTo (	std::string path,
				std::string fileName)=0;
	virtual void
	OnMove (std::string path,
			std::string fileName)=0;
	virtual void
	OnCreate (	std::string path,
				std::string fileName)=0;
	virtual void
	OnDelete (	std::string path,
				std::string fileName)=0;
	virtual void
	OnDeleteSelf (	std::string path,
					std::string fileName)=0;
	virtual void
	OnMoveSelf (std::string path,
				std::string fileName)=0;
};

}

#endif /* ON_FILE_NOTIFY_EVENT_H_ */
