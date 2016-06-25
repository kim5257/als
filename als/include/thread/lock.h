/*
 * lock.h
 *
 *  Created on: 2015. 8. 14.
 *      Author: asran
 */

#ifndef LOCK_H_
#define LOCK_H_

#if defined(_WIN32) || defined(_WIN64)

#include <WinBase.h>

typedef CRITICAL_SECTION lock_t;

#else

#include <pthread.h>

typedef pthread_mutex_t lock_t;

#define	INIT(lock)		pthread_mutex_init(&lock, NULL);
#define	DESTROY(lock)	pthread_mutex_destroy(&lock);
#define	LOCK(lock)		pthread_mutex_lock(&lock);
#define	UNLOCK(lock)	pthread_mutex_unlock(&lock);

#endif

namespace thread
{

class Lock
{
private:
	lock_t mLock;
public:
	inline
	Lock (void);
	inline virtual
	~Lock (void);
public:
	inline void
	lock (void);
	inline void
	unlock (void);
};

Lock::Lock (void)
{
	INIT(mLock);
}

Lock::~Lock (void)
{
	DESTROY(mLock);
}

void
Lock::lock (void)
{
	LOCK(mLock);
}

void
Lock::unlock (void)
{
	UNLOCK(mLock);
}


}

#endif /* LOCK_H_ */
