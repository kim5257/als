/*
 * file_notify_setting.h
 *
 *  Created on: 2015. 6. 23.
 *      Author: asran
 */

#ifndef INCLUDE__FILE_NOTIFY_SETTING_H_
#define INCLUDE__FILE_NOTIFY_SETTING_H_

#define	MAX_NOTIFY_FD		(2)
#define	MAX_WATCH_FD		(8192)
#define	INOTIFY_BUF_LEN	(1024)

#define	EVENT_TIMEOUT		(10000)	///< 10ms
#define	IN_MOVE_TIMEOUT	(1000)		///< 이동 된 폴더 처리 타임아웃

#define	PATH_SEPARATE		'/'

#endif /* INCLUDE__FILE_NOTIFY_SETTING_H_ */
