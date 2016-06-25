/*
 * thread.h
 *
 *  Created on: 2015. 8. 21.
 *      Author: asran
 */

#ifndef THREAD_THREAD_H_
#define THREAD_THREAD_H_

#include <pthread.h>
#include <thread/runnable.h>
#include <error/err_msg.h>

namespace thread
{

class Thread
{
private:
	pthread_t mTID;
	Runnable* mRunnable;
	uint8_t mExitFlag;
	error::ErrMsg* mThreadErrMsg;
public:
	Thread (Runnable* runnable);
	virtual
	~Thread (void);
public:
	void
	start (void);
	void
	stop (void);
	void*
	join (void);
public:
	inline void
	reqExit (void);
private:
	static void*
	threadFunc (void* obj);
};

void
Thread::reqExit (void)
{
	mExitFlag = 1;
}

}

#endif /* THREAD_THREAD_H_ */
