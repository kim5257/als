/*
 * sha1.cpp
 *
 *  Created on: 2015. 7. 30.
 *      Author: asran
 */

#include "sha/sha1.h"
#include <string.h>
#include "error/err_msg.h"

namespace sha
{

const uint32_t Sha1::mInitHash[HASH_SIZE] =
{ 0x67452301, 0xEFCDAB89, 0x98BADCFE, 0x10325476, 0xC3D2E1F0 };

const uint32_t Sha1::mK[K_SIZE] =
{ 0x5A827999, 0x6ED9EBA1, 0x8F1BBCDC, 0xCA62C1D6 };

Sha1::Sha1 (void)
		: mMsgLen (0),
			mMsgBlkOffset (0),
			mFinish (false)

{
	init ();
}

Sha1::~Sha1 (void)
{

}

void
Sha1::init (void)
{
	mFinish = false;
	mMsgLen = 0;
	mMsgBlkOffset = 0;

	memset (mMsgBlk, 0, sizeof(mMsgBlk));
	memcpy (mHash, mInitHash, sizeof(mInitHash));
}

void
Sha1::input (	const char* data,
				size_t len)
{
	uint64_t msgLen = mMsgLen;
	uint32_t dataOffset = 0;

	if ( (msgLen + (len * 8)) < mMsgLen )
	{
		// 에러처리, 지원하는 최대 길이를 넘어서는 데이터
		throw error::ErrMsg::createErrMsg("Input data length too long.");
	}

	mMsgLen = msgLen + (len * 8);

	while( 0 < len )
	{
		uint32_t remain = MSG_BLK_SIZE - mMsgBlkOffset;
		uint32_t moveSize = (remain < len)? (remain):(len);

		memcpy (&mMsgBlk[mMsgBlkOffset], &data[dataOffset], moveSize);

		dataOffset += moveSize;
		mMsgBlkOffset += moveSize;
		len -= moveSize;

		if ( mMsgBlkOffset == MSG_BLK_SIZE )
		{
			calcHash ();
		}
	}
}

void
Sha1::result (uint32_t* hash)
{
	if ( !mFinish )
	{
		finish ();
	}

	memcpy (hash, mHash, sizeof(mHash));
}

void
Sha1::calcHash (void)
{
	uint32_t wordSeq[WORD_BLK_SIZE] =
	{ 0, };
	uint32_t A = mHash[0];
	uint32_t B = mHash[1];
	uint32_t C = mHash[2];
	uint32_t D = mHash[3];
	uint32_t E = mHash[4];

	for(size_t cnt = 0;cnt < 16;++cnt)
	{
		wordSeq[cnt] = ((uint32_t)mMsgBlk[cnt * sizeof(uint32_t)]) << 24;
		wordSeq[cnt] |= ((uint32_t)mMsgBlk[cnt * sizeof(uint32_t) + 1]) << 16;
		wordSeq[cnt] |= ((uint32_t)mMsgBlk[cnt * sizeof(uint32_t) + 2]) << 8;
		wordSeq[cnt] |= ((uint32_t)mMsgBlk[cnt * sizeof(uint32_t) + 3]);
	}

	for(size_t cnt = 16;cnt < 80;++cnt)
	{
		wordSeq[cnt] = rotation (	(wordSeq[cnt - 3]
										^ wordSeq[cnt - 8]
										^ wordSeq[cnt - 14]
										^ wordSeq[cnt - 16]),
									1);
	}

	for(size_t cnt = 0;cnt < 20;++cnt)
	{
		uint32_t F = (B & C) | ((~B) & D);
		uint32_t tmp = rotation (A, 5) + F + E + mK[0] + wordSeq[cnt];

		E = D;
		D = C;
		C = rotation (B, 30);
		B = A;
		A = tmp;
	}

	for(size_t cnt = 20;cnt < 40;++cnt)
	{
		uint32_t F = B ^ C ^ D;
		uint32_t tmp = rotation (A, 5) + F + E + mK[1] + wordSeq[cnt];

		E = D;
		D = C;
		C = rotation (B, 30);
		B = A;
		A = tmp;
	}

	for(size_t cnt = 40;cnt < 60;++cnt)
	{
		uint32_t F = (B & C) | (B & D) | (C & D);
		uint32_t tmp = rotation (A, 5) + F + E + mK[2] + wordSeq[cnt];

		E = D;
		D = C;
		C = rotation (B, 30);
		B = A;
		A = tmp;
	}

	for(size_t cnt = 60;cnt < 80;++cnt)
	{
		uint32_t F = B ^ C ^ D;
		uint32_t tmp = rotation (A, 5) + F + E + mK[3] + wordSeq[cnt];

		E = D;
		D = C;
		C = rotation (B, 30);
		B = A;
		A = tmp;
	}

	mHash[0] += A;
	mHash[1] += B;
	mHash[2] += C;
	mHash[3] += D;
	mHash[4] += E;

	mMsgBlkOffset = 0;
}

void
Sha1::finish (void)
{
	mMsgBlk[mMsgBlkOffset++] = 0x80;

	if ( mMsgBlkOffset >= (MSG_BLK_SIZE - MSG_LEN_SIZE) )
	{
		uint32_t remain = MSG_BLK_SIZE - mMsgBlkOffset;
		memset (&mMsgBlk[mMsgBlkOffset], 0, remain);

		mMsgBlkOffset += remain;

		calcHash ();

		memset (mMsgBlk, 0, MSG_BLK_SIZE - MSG_LEN_SIZE);
	}
	else
	{
		uint32_t remain = MSG_BLK_SIZE - mMsgBlkOffset - MSG_LEN_SIZE;

		memset (&mMsgBlk[mMsgBlkOffset], 0, remain);
	}

	mMsgBlk[MSG_BLK_SIZE - 1] = (mMsgLen >> 0) & 0xFF;
	mMsgBlk[MSG_BLK_SIZE - 2] = (mMsgLen >> 8) & 0xFF;
	mMsgBlk[MSG_BLK_SIZE - 3] = (mMsgLen >> 16) & 0xFF;
	mMsgBlk[MSG_BLK_SIZE - 4] = (mMsgLen >> 24) & 0xFF;
	mMsgBlk[MSG_BLK_SIZE - 5] = (mMsgLen >> 32) & 0xFF;
	mMsgBlk[MSG_BLK_SIZE - 6] = (mMsgLen >> 40) & 0xFF;
	mMsgBlk[MSG_BLK_SIZE - 7] = (mMsgLen >> 48) & 0xFF;
	mMsgBlk[MSG_BLK_SIZE - 8] = (mMsgLen >> 56) & 0xFF;

	mMsgBlkOffset += MSG_LEN_SIZE;

	calcHash ();
}

}
