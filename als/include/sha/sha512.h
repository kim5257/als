/*
 * sha512.h
 *
 *  Created on: 2015. 8. 6.
 *      Author: asran
 */

#ifndef SHA512_H_
#define SHA512_H_

#include <stdint.h>
#include <stddef.h>

#define	HASH_SIZE		(8)
#define	MSG_BLK_SIZE	(128)
#define	WORD_BLK_SIZE	(80)
#define	K_SIZE			(80)
#define	MSG_LEN_SIZE	(16)

namespace sha
{

class Sha512
{
private:
	static const uint64_t mInitHash[HASH_SIZE];
	static const uint64_t mK[K_SIZE];
private:
	uint64_t mHash[HASH_SIZE];
	uint64_t mMsgLen[MSG_LEN_SIZE / 8];
	uint8_t mMsgBlk[MSG_BLK_SIZE];
	uint32_t mMsgBlkOffset;
	bool mFinish;
public:
	Sha512 (void);
	virtual
	~Sha512 (void);
public:
	void
	init (void);
	void
	input (	const char* data,
			size_t len);
	void
	result (uint64_t* hash);
private:
	void
	calcHash (void);
	void
	finish (void);
private:
	inline void
	set128 (uint64_t a[],
			uint64_t high,
			uint64_t low);
	inline bool
	add128 (uint64_t a[],
			uint64_t b);
	inline uint64_t
	shift (	uint64_t data,
			int cnt);
	inline uint64_t
	rotation (	uint64_t data,
				int cnt);
};

void
Sha512::set128 (uint64_t a[],
				uint64_t high,
				uint64_t low)
{
	a[0] = low;
	a[1] = high;
}

bool
Sha512::add128 (uint64_t a[],
				uint64_t b)
{
	bool overflow = false;
	uint64_t tmp[2];

	tmp[0] = a[0] + b;
	tmp[1] = a[1];

	a[1] += (tmp[0] < a[0])? (1):(0);
	a[0] += b;
	overflow = (tmp[1] <= a[1])? (false):(true);

	return overflow;
}

uint64_t
Sha512::shift (	uint64_t data,
				int cnt)
{
	return (data >> cnt);
}

inline uint64_t
Sha512::rotation (	uint64_t data,
					int cnt)
{
	return ((data >> cnt) & (0xFFFFFFFFFFFFFFFF))
			| (data << ((sizeof(uint64_t) * 8) - cnt));
}

}

#endif /* SHA512_H_ */
