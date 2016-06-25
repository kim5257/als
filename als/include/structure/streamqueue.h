/*
 * streamqueue.h
 *
 *  Created on: 2015. 8. 13.
 *      Author: asran
 */

#ifndef QUEUE_H_
#define QUEUE_H_

#include <stdint.h>
#include <stddef.h>
#include <thread/lock.h>

namespace datastruct
{

class StreamQueue
{
private:
	size_t mQueueSize;
	size_t mDataSize;
	size_t mStart;
	size_t mEnd;
	uint8_t* mData;
	thread::Lock mLock;
public:
	StreamQueue (size_t size);
	virtual
	~StreamQueue (void);
private:
	void
	alloc (size_t size);
	void
	destroy (void);
public:
	void
	clear (void);
	bool
	push (	const uint8_t data[],
			size_t size);
	bool
	poll (	uint8_t data[],
			size_t size);
	bool
	peek (	uint8_t data[],
			size_t size) const;
	bool
	flush (size_t size);
	size_t
	findByte (uint8_t val) const;
public:
	inline size_t
	getQueueSize (void) const;
	inline size_t
	getSize (void) const;
	inline size_t
	getRemainSize (void) const;
	inline bool
	isEmpty (void) const;
	inline bool
	isFull (void) const;
};

size_t
StreamQueue::getQueueSize (void) const
{
	return mQueueSize;
}

size_t
StreamQueue::getSize (void) const
{
	return mDataSize;
}

size_t
StreamQueue::getRemainSize (void) const
{
	return getQueueSize () - getSize ();
}

bool
StreamQueue::isEmpty (void) const
{
	return (getSize () == 0)? (true):(false);
}

bool
StreamQueue::isFull (void) const
{
	return (getSize () == getQueueSize ())? (true):(false);
}

}

#endif /* QUEUE_H_ */
