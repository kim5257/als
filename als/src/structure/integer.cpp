/*
 * @file Integer.cpp
 * @date 2015. 10. 21.
 *      Author: asran
 */

#include <string.h>
#include <structure/integer.h>

namespace datastruct
{

uint32_t Integer::DATA_BIT_SIZE = sizeof(uint64_t) << 3;

Integer::Integer (size_t dwordSize)
		: mData (NULL),
			mDWordSize (0)
{
	init (dwordSize);
}

Integer::Integer (const Integer& obj)
		: mData (NULL),
			mDWordSize (0)
{
	substitute (obj);
}

Integer::~Integer (void)
{
	deinit ();
}

void
Integer::init (size_t dwordSize)
{
	if ( mData != NULL )
	{
		deinit ();
	}

	mDWordSize = dwordSize;
	mData = new uint64_t[mDWordSize];
	memset (mData, 0, sizeof(uint64_t) * mDWordSize);

}

void
Integer::deinit (void)
{
	delete[] mData;
	mData = NULL;
}

void
Integer::clear (void)
{
	memset (mData, 0, sizeof(uint64_t) * mDWordSize);
}

Integer&
Integer::substitute (const Integer& obj)
{
	clear ();

	init (obj.mDWordSize);
	memcpy (mData, obj.mData, sizeof(uint64_t) * mDWordSize);

	return *this;
}

Integer&
Integer::substitute (uint64_t val)
{
	clear ();

	mData[0] = val;

	return *this;
}

Integer
Integer::add (const Integer& val) const
{
	size_t minWordSize =
			(mDWordSize < val.mDWordSize)? (mDWordSize):(val.mDWordSize);
	size_t maxWordSize =
			(mDWordSize > val.mDWordSize)? (mDWordSize):(val.mDWordSize);
	Integer result (maxWordSize);
	uint32_t carryBit = 0;

	for(size_t cnt = 0;cnt < minWordSize;++cnt)
	{
		result.mData[cnt] = mData[cnt] + val.mData[cnt] + carryBit;
		carryBit = (mData[cnt] > result.mData[cnt])? (1):(0);
	}

	if ( carryBit != 0 && (minWordSize != maxWordSize) )
	{
		result.mData[minWordSize + 1] += carryBit;
	}

	return result;
}

Integer
Integer::sub (const Integer& val) const
{
	size_t minWordSize =
			(mDWordSize < val.mDWordSize)? (mDWordSize):(val.mDWordSize);
	size_t maxWordSize =
			(mDWordSize > val.mDWordSize)? (mDWordSize):(val.mDWordSize);
	Integer result (maxWordSize);
	uint32_t carryBit = 0;

	for(size_t cnt = 0;cnt < minWordSize;++cnt)
	{
		result.mData[cnt] = mData[cnt] - val.mData[cnt] - carryBit;
		carryBit = (mData[cnt] < result.mData[cnt])? (1):(0);
	}

	if ( carryBit != 0 && (minWordSize != maxWordSize) )
	{
		result.mData[minWordSize + 1] -= carryBit;
	}

	return result;
}

Integer
Integer::mul (const Integer& val) const
{
	size_t maxWordSize =
			(mDWordSize > val.mDWordSize)? (mDWordSize):(val.mDWordSize);
	Integer result (maxWordSize);

	Integer dataA = (compare(val) < 0)?(*this):(val);
	Integer dataB = (compare(val) >= 0)?(*this):(val);

	while( dataA.compare(0) != 0 )
	{
		if( dataA.bitAnd(1).compare(0) != 0 )
		{
			result.selfAdd(dataB);
		}

		dataA.selfShiftRight(1);
		dataB.selfShiftLeft(1);
	}

	return result;
}

Integer&
Integer::selfAdd (const Integer& val)
{
	size_t minWordSize =
			(mDWordSize < val.mDWordSize)? (mDWordSize):(val.mDWordSize);
	uint32_t carryBit = 0;

	for(size_t cnt = 0;cnt < minWordSize;++cnt)
	{
		uint64_t sum = mData[cnt] + (val.mData[cnt] + carryBit);
		carryBit = (mData[cnt] > sum)? (1):(0);
		mData[cnt] = sum;
	}

	if ( carryBit != 0 && (this->mDWordSize != minWordSize) )
	{
		mData[minWordSize + 1] += carryBit;
	}

	return *this;
}

Integer
Integer::shiftRight (size_t offset) const
{
	Integer result (mDWordSize);

	size_t cnt = 0;

	for(;cnt < mDWordSize - 1;++cnt)
	{
		result.mData[cnt] = (mData[cnt] >> offset)
							| (mData[cnt + 1] << (DATA_BIT_SIZE - offset));
	}

	result.mData[cnt] = mData[cnt] >> offset;

	return result;
}

Integer
Integer::shiftLeft (size_t offset) const
{
	Integer result (mDWordSize);

	size_t cnt = mDWordSize - 1;

	for(;cnt > 0;--cnt)
	{
		result.mData[cnt] = (mData[cnt - 1] >> (DATA_BIT_SIZE - offset))
							| (mData[cnt] << offset);
	}

	result.mData[cnt] = mData[cnt] << offset;

	return result;
}

Integer&
Integer::selfShiftRight (size_t offset)
{
	size_t cnt = 0;

	for(;cnt < mDWordSize - 1;++cnt)
	{
		mData[cnt] = (mData[cnt] >> offset)
						| (mData[cnt + 1] << (DATA_BIT_SIZE - offset));
	}

	mData[cnt] = mData[cnt] >> offset;

	return *this;
}

Integer&
Integer::selfShiftLeft (size_t offset)
{
	size_t cnt = mDWordSize - 1;

	for(;cnt > 0;--cnt)
	{
		mData[cnt] = (mData[cnt - 1] >> (DATA_BIT_SIZE - offset))
						| (mData[cnt] << offset);
	}

	mData[cnt] = mData[cnt] << offset;

	return *this;
}

Integer
Integer::bitAnd (const Integer& val) const
{
	size_t minWordSize =
			(mDWordSize < val.mDWordSize)? (mDWordSize):(val.mDWordSize);
	Integer result(minWordSize);

	for(size_t cnt=0;cnt<minWordSize;++cnt)
	{
		result.mData[cnt] = this->mData[cnt] & val.mData[cnt];
	}

	return	result;
}

Integer
Integer::bitAnd (uint64_t val) const
{
	Integer result(mDWordSize);

	result.mData[0] = this->mData[0] & val;

	return	result;
}

int
Integer::compare (const Integer& val) const
{
	size_t minWordSize =
			(mDWordSize < val.mDWordSize)? (mDWordSize):(val.mDWordSize);
	int result = 0;

	if ( mDWordSize < val.mDWordSize )
	{
		for(size_t cnt=minWordSize;cnt<val.mDWordSize;++cnt)
		{
			result = (val.mData[cnt]==0)?(0):(-1);
			if( result != 0 )
			{
				break;
			}
		}
	}
	else if( mDWordSize > val.mDWordSize )
	{
		for(size_t cnt=minWordSize;cnt<mDWordSize;++cnt)
		{
			result = (mData[cnt]==0)?(0):(1);
			if( result != 0 )
			{
				break;
			}
		}
	}

	if( result == 0 )
	{
		for(size_t cnt=minWordSize-1;cnt >= 0;--cnt)
		{
			if( mData[cnt] < val.mData[cnt] )
			{
				result = -1;
			}
			else if( mData[cnt] > val.mData[cnt] )
			{
				result = 1;
			}

			if( result != 0 )
			{
				break;
			}
		}
	}

	return result;
}

int
Integer::compare (uint64_t val) const
{
	int result = 0;

	for(size_t cnt=1;cnt<this->mDWordSize;++cnt)
	{
		if( this->mData[cnt] != 0 )
		{
			result = 1;
			break;
		}
	}

	if( result == 0 )
	{
		result = (this->mData[0] > val)?(1):(result);
		result = (this->mData[0] < val)?(-1):(result);
	}

	return result;
}

}
