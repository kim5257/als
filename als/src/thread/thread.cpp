/*
 * thread.cpp
 *
 *  Created on: 2015. 8. 21.
 *      Author: asran
 */

#include "thread/thread.h"

namespace thread
{

Thread::Thread (Runnable* runnable)
		: mTID (0),
			mRunnable (runnable),
			mExitFlag (0),
			mThreadErrMsg (NULL)
{

}

Thread::~Thread (void)
{
	stop ();
}

void
Thread::start (void)
{
	int ret = 0;

	if ( mTID != 0 )
	{
		throw error::ErrMsg::createErrMsg ("Thread already started.");
	}

	mExitFlag = 0;

	ret = pthread_create (&mTID, NULL, Thread::threadFunc, this);

	if ( ret != 0 )
	{
		reqExit ();
		throw error::ErrMsg::createErrMsg ("Create thread failed.");
	}
}

void
Thread::stop (void)
{
	void* ret = NULL;

	if ( mTID == 0 )
	{
		throw error::ErrMsg::createErrMsg ("Thread didn't start.");
	}

	reqExit ();
	ret = join ();

	mTID = 0;

	if ( ret != NULL )
	{
		throw error::ErrMsg::createErrMsg ("Occurred error when stopping thread.");
	}
	else if ( mThreadErrMsg != NULL )
	{
		throw mThreadErrMsg;
	}
}

void*
Thread::join (void)
{
	void* ret = NULL;

	pthread_join (mTID, &ret);

	return ret;
}

void*
Thread::threadFunc (void* obj)
{
	void* ret = NULL;
	Thread* threadObj = (Thread*)obj;

	try
	{
		ret = threadObj->mRunnable->run (&threadObj->mExitFlag);
	}
	catch( error::ErrMsg* err )
	{
		threadObj->mThreadErrMsg = err;
	}

	return ret;

}

}
