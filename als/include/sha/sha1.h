/*
 * sha1.h
 *
 *  Created on: 2015. 7. 30.
 *      Author: asran
 */

#ifndef SHA1_H_
#define SHA1_H_

#include <stdint.h>
#include <stddef.h>

#define	HASH_SIZE		(5)
#define	MSG_BLK_SIZE	(64)
#define	WORD_BLK_SIZE	(80)
#define	K_SIZE			(4)
#define	MSG_LEN_SIZE	(8)

namespace sha
{

class Sha1
{
private:
	static const uint32_t mInitHash[HASH_SIZE];
	static const uint32_t mK[K_SIZE];
private:
	uint32_t mHash[HASH_SIZE];
	uint64_t mMsgLen;
	uint8_t mMsgBlk[MSG_BLK_SIZE];
	uint32_t mMsgBlkOffset;
	bool mFinish;
public:
	Sha1 (void);
	virtual
	~Sha1 (void);
public:
	void
	init (void);
	void
	input (	const char* data,
			size_t len);
	void
	result (uint32_t* hash);
private:
	void
	calcHash (void);
	void
	finish (void);
private:
	inline uint32_t
	rotation (	uint32_t data,
				int cnt);
};

inline uint32_t
Sha1::rotation (uint32_t data,
				int cnt)
{
	return ((data << cnt) & (0xFFFFFFFF))
			| (data >> ((sizeof(uint32_t) * 8) - cnt));
}

}

#endif /* SHA1_H_ */
