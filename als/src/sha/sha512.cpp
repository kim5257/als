/*
 * sha512.cpp
 *
 *  Created on: 2015. 8. 6.
 *      Author: asran
 */

#include "sha/sha512.h"
#include "error/err_msg.h"
#include <string.h>
#include <stdio.h>

namespace sha
{

const uint64_t Sha512::mInitHash[HASH_SIZE] =
{ 0x6A09E667F3BCC908, 0xBB67AE8584CAA73B, 0x3C6EF372FE94F82B,
		0xA54FF53A5F1D36F1, 0x510E527FADE682D1, 0x9B05688C2B3E6C1F,
		0x1F83D9ABFB41BD6B, 0x5BE0CD19137E2179, };

const uint64_t Sha512::mK[K_SIZE] =
{ 0x428A2F98D728AE22, 0x7137449123EF65CD, 0xB5C0FBCFEC4D3B2F,
		0xE9B5DBA58189DBBC, 0x3956C25BF348B538, 0x59F111F1B605D019,
		0x923F82A4AF194F9B, 0xAB1C5ED5DA6D8118, 0xD807AA98A3030242,
		0x12835B0145706FBE, 0x243185BE4EE4B28C, 0x550C7DC3D5FFB4E2,
		0x72BE5D74F27B896F, 0x80DEB1FE3B1696B1, 0x9BDC06A725C71235,
		0xC19BF174CF692694, 0xE49B69C19EF14AD2, 0xEFBE4786384F25E3,
		0x0FC19DC68B8CD5B5, 0x240CA1CC77AC9C65, 0x2DE92C6F592B0275,
		0x4A7484AA6EA6E483, 0x5CB0A9DCBD41FBD4, 0x76F988DA831153B5,
		0x983E5152EE66DFAB, 0xA831C66D2DB43210, 0xB00327C898FB213F,
		0xBF597FC7BEEF0EE4, 0xC6E00BF33DA88FC2, 0xD5A79147930AA725,
		0x06CA6351E003826F, 0x142929670A0E6E70, 0x27B70A8546D22FFC,
		0x2E1B21385C26C926, 0x4D2C6DFC5AC42AED, 0x53380D139D95B3DF,
		0x650A73548BAF63DE, 0x766A0ABB3C77B2A8, 0x81C2C92E47EDAEE6,
		0x92722C851482353B, 0xA2BFE8A14CF10364, 0xA81A664BBC423001,
		0xC24B8B70D0F89791, 0xC76C51A30654BE30, 0xD192E819D6EF5218,
		0xD69906245565A910, 0xF40E35855771202A, 0x106AA07032BBD1B8,
		0x19A4C116B8D2D0C8, 0x1E376C085141AB53, 0x2748774CDF8EEB99,
		0x34B0BCB5E19B48A8, 0x391C0CB3C5C95A63, 0x4ED8AA4AE3418ACB,
		0x5B9CCA4F7763E373, 0x682E6FF3D6B2B8A3, 0x748F82EE5DEFB2FC,
		0x78A5636F43172F60, 0x84C87814A1F0AB72, 0x8CC702081A6439EC,
		0x90BEFFFA23631E28, 0xA4506CEBDE82BDE9, 0xBEF9A3F7B2C67915,
		0xC67178F2E372532B, 0xCA273ECEEA26619C, 0xD186B8C721C0C207,
		0xEADA7DD6CDE0EB1E, 0xF57D4F7FEE6ED178, 0x06F067AA72176FBA,
		0x0A637DC5A2C898A6, 0x113F9804BEF90DAE, 0x1B710B35131C471B,
		0x28DB77F523047D84, 0x32CAAB7B40C72493, 0x3C9EBE0A15C9BEBC,
		0x431D67C49C100D4C, 0x4CC5D4BECB3E42B6, 0x597F299CFC657E2A,
		0x5FCB6FAB3AD6FAEC, 0x6C44198C4A475817 };

Sha512::Sha512 (void)
{
	init ();
}

Sha512::~Sha512 (void)
{

}

void
Sha512::init (void)
{
	mFinish = false;
	set128 (mMsgLen, 0, 0);
	mMsgBlkOffset = 0;

	memset (mMsgBlk, 0, sizeof(mMsgBlk));
	memcpy (mHash, mInitHash, sizeof(mInitHash));
}

void
Sha512::input (	const char* data,
				size_t len)
{
	uint32_t dataOffset = 0;

	if ( !add128 (mMsgLen, len * 8) )
	{
		// 에러처리, 지원하는 최대 길이를 넘어서는 데이터
		throw error::ErrMsg::createErrMsg("Input data length too long.");
	}

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
Sha512::result (uint64_t* hash)
{
	if ( !mFinish )
	{
		finish ();
	}

	memcpy (hash, mHash, sizeof(mHash));
}

void
Sha512::calcHash (void)
{
	uint64_t wordSeq[WORD_BLK_SIZE] =
	{ 0, };
	uint64_t A = mHash[0];
	uint64_t B = mHash[1];
	uint64_t C = mHash[2];
	uint64_t D = mHash[3];
	uint64_t E = mHash[4];
	uint64_t F = mHash[5];
	uint64_t G = mHash[6];
	uint64_t H = mHash[7];

	for(size_t cnt = 0;cnt < 16;++cnt)
	{
		wordSeq[cnt] = ((uint64_t)mMsgBlk[cnt * sizeof(uint64_t)]) << 56;
		wordSeq[cnt] |= ((uint64_t)mMsgBlk[cnt * sizeof(uint64_t) + 1]) << 48;
		wordSeq[cnt] |= ((uint64_t)mMsgBlk[cnt * sizeof(uint64_t) + 2]) << 40;
		wordSeq[cnt] |= ((uint64_t)mMsgBlk[cnt * sizeof(uint64_t) + 3]) << 32;
		wordSeq[cnt] |= ((uint64_t)mMsgBlk[cnt * sizeof(uint64_t) + 4]) << 24;
		wordSeq[cnt] |= ((uint64_t)mMsgBlk[cnt * sizeof(uint64_t) + 5]) << 16;
		wordSeq[cnt] |= ((uint64_t)mMsgBlk[cnt * sizeof(uint64_t) + 6]) << 8;
		wordSeq[cnt] |= ((uint64_t)mMsgBlk[cnt * sizeof(uint64_t) + 7]);
	}

	for(size_t cnt = 16;cnt < 80;++cnt)
	{
		wordSeq[cnt] = (rotation (wordSeq[cnt - 2], 19)
						^ rotation (wordSeq[cnt - 2], 61)
						^ shift (wordSeq[cnt - 2], 6))
						+ wordSeq[cnt - 7] + (rotation (wordSeq[cnt - 15], 1)
												^ rotation (wordSeq[cnt - 15],
															8)
												^ shift (wordSeq[cnt - 15], 7))
						+ wordSeq[cnt - 16];
	}

	for(size_t cnt = 0;cnt < WORD_BLK_SIZE / 8;++cnt)
	{
		for(size_t cnt2 = 0;cnt2 < 8;++cnt2)
		{
			uint64_t temp1 = H
								+ (rotation (E, 14)
									^ rotation (E, 18)
									^ rotation (E, 41))
								+ (G ^ (E & (F ^ G)))
								+ mK[cnt * 8 + cnt2]
								+ wordSeq[cnt * 8 + cnt2];
			uint64_t temp2 = (rotation (A, 28)
								^ rotation (A, 34)
								^ rotation (A, 39))
								+ ((A & B) | (C & (A | B)));

			H = G;
			G = F;
			F = E;
			E = D + temp1;
			D = C;
			C = B;
			B = A;
			A = temp1 + temp2;
		}
	}

	mHash[0] += A;
	mHash[1] += B;
	mHash[2] += C;
	mHash[3] += D;
	mHash[4] += E;
	mHash[5] += F;
	mHash[6] += G;
	mHash[7] += H;

	mMsgBlkOffset = 0;
}

void
Sha512::finish (void)
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

	mMsgBlk[MSG_BLK_SIZE - 1] = (mMsgLen[0] >> 0) & 0xFF;
	mMsgBlk[MSG_BLK_SIZE - 2] = (mMsgLen[0] >> 8) & 0xFF;
	mMsgBlk[MSG_BLK_SIZE - 3] = (mMsgLen[0] >> 16) & 0xFF;
	mMsgBlk[MSG_BLK_SIZE - 4] = (mMsgLen[0] >> 24) & 0xFF;
	mMsgBlk[MSG_BLK_SIZE - 5] = (mMsgLen[0] >> 32) & 0xFF;
	mMsgBlk[MSG_BLK_SIZE - 6] = (mMsgLen[0] >> 40) & 0xFF;
	mMsgBlk[MSG_BLK_SIZE - 7] = (mMsgLen[0] >> 48) & 0xFF;
	mMsgBlk[MSG_BLK_SIZE - 8] = (mMsgLen[0] >> 56) & 0xFF;

	mMsgBlk[MSG_BLK_SIZE - 9] = (mMsgLen[1] >> 0) & 0xFF;
	mMsgBlk[MSG_BLK_SIZE - 10] = (mMsgLen[1] >> 8) & 0xFF;
	mMsgBlk[MSG_BLK_SIZE - 11] = (mMsgLen[1] >> 16) & 0xFF;
	mMsgBlk[MSG_BLK_SIZE - 12] = (mMsgLen[1] >> 24) & 0xFF;
	mMsgBlk[MSG_BLK_SIZE - 13] = (mMsgLen[1] >> 32) & 0xFF;
	mMsgBlk[MSG_BLK_SIZE - 14] = (mMsgLen[1] >> 40) & 0xFF;
	mMsgBlk[MSG_BLK_SIZE - 15] = (mMsgLen[1] >> 48) & 0xFF;
	mMsgBlk[MSG_BLK_SIZE - 16] = (mMsgLen[1] >> 56) & 0xFF;

	mMsgBlkOffset += MSG_LEN_SIZE;

	calcHash ();
}

}
