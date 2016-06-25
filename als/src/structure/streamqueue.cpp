/*
 * queue.cpp
 *
 *  Created on: 2015. 8. 13.
 *      Author: asran
 */

#include <stdio.h>
#include <string.h>
#include "structure/streamqueue.h"

namespace datastruct
{

StreamQueue::StreamQueue (size_t size)
		: mQueueSize (0),
			mDataSize (0),
			mStart (0),
			mEnd (0),
			mData (NULL)
{
	alloc (size);
}

StreamQueue::~StreamQueue (void)
{
	destroy ();
}

void
StreamQueue::alloc (size_t size)
{
	mData = new uint8_t[size];
	mQueueSize = size;

	mDataSize = 0;
	mStart = 0;
	mEnd = 0;
}

void
StreamQueue::destroy (void)
{
	mDataSize = 0;
	mEnd = 0;
	mStart = 0;

	mQueueSize = 0;
	delete[] mData;
	mData = NULL;
}

void
StreamQueue::clear (void)
{
	mDataSize = 0;
	mStart = 0;
	mEnd = 0;
}

bool
StreamQueue::push (	const uint8_t data[],
					size_t size)
{
	bool ret = false;

	do
	{
		if ( size > getRemainSize () )
		{
			break;
		}

		mLock.lock ();

		size_t start = mEnd;
		size_t end = (mStart > mEnd)? (mStart):(getQueueSize ());
		size_t csize = (size > (end - start))? (end - start):(size);

		mEnd = (mEnd + size < getQueueSize ())?
				(mEnd + size):(size - (getQueueSize () - mEnd));
		mDataSize += size;

		mLock.unlock ();

		memcpy (&mData[start], data, csize);

		if ( csize < size )
		{
			size_t dstart = csize;
			csize = size - csize;

			memcpy (mData, &data[dstart], csize);
		}

		ret = true;
	}
	while( 0 );

	return ret;
}

bool
StreamQueue::poll (	uint8_t data[],
					size_t size)
{
	bool ret = false;

	do
	{
		if ( size > getSize () )
		{
			break;
		}

		mLock.lock ();

		size_t start = mStart;
		size_t end = (mEnd > mStart)? (mEnd):(getQueueSize ());
		size_t csize = (size > (end - start))? (end - start):(size);

		mStart =
				(mStart + size < getQueueSize ())?
						(mStart + size):(size - (getQueueSize () - mStart));
		mDataSize -= size;

		mLock.unlock ();

		memcpy (data, &mData[start], csize);

		if ( csize < size )
		{
			size_t dstart = csize;
			csize = size - csize;

			memcpy (&data[dstart], mData, csize);
		}

		ret = true;
	}
	while( 0 );

	return ret;
}

bool
StreamQueue::peek (	uint8_t data[],
					size_t size) const
{
	bool ret = false;

	do
	{
		if ( size > getSize () )
		{
			break;
		}

		size_t start = mStart;
		size_t end = (mEnd > mStart)? (mEnd):(getQueueSize ());
		size_t csize = (size > (end - start))? (end - start):(size);
		memcpy (data, &mData[start], csize);

		if ( csize < size )
		{
			size_t dstart = csize;
			csize = size - csize;

			memcpy (&data[dstart], mData, csize);
		}

		ret = true;
	}
	while( 0 );

	return ret;
}

bool
StreamQueue::flush (size_t size)
{
	bool ret = false;

	do
	{
		if ( size > getSize () )
		{
			break;
		}

		mLock.lock ();

		mStart =
				(mStart + size < getQueueSize ())?
						(mStart + size):(size - (getQueueSize () - mStart));
		mDataSize -= size;

		mLock.unlock ();

		ret = true;
	}
	while( 0 );

	return ret;
}

size_t
StreamQueue::findByte (uint8_t val) const
{
	size_t ret = 0;
	size_t idx = mStart;
	size_t end = (mEnd >= mStart)? (mEnd):(getQueueSize ());

	for(;idx<end;++idx)
	{
		if( val == mData[idx] )
		{
			ret = (idx - mStart) + 1;
			break;
		}
	}

	if( (ret < 0) && (end != mEnd) )
	{
		idx = 0;
		end = mEnd;
		for(;idx<end;++idx)
		{
			if( val == mData[idx] )
			{
				ret = (getSize() - (end-idx)) + 1;
				break;
			}
		}
	}

	return ret;
}

}
