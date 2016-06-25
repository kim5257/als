/*
 * @file Integer.h
 * @date 2015. 10. 21.
 *      Author: asran
 */
#ifndef STRUCTURE_INTEGER_H_
#define STRUCTURE_INTEGER_H_

#include <stdint.h>
#include <stddef.h>
#include <thread/lock.h>

namespace datastruct
{

class Integer
{
private:
	static uint32_t DATA_BIT_SIZE;
private:
	uint64_t* mData;
	size_t mDWordSize;
public:
	Integer (size_t dwordSize);
	Integer (const Integer& obj);
	virtual
	~Integer (void);
private:
	void
	init (size_t dwordSize);
	void
	deinit (void);
	void
	clear (void);
public:
	Integer&
	substitute (const Integer& obj);
	Integer&
	substitute (uint64_t val);
	Integer
	add (const Integer& val) const;
	Integer
	sub (const Integer& val) const;
	Integer
	mul (const Integer& val) const;
	Integer&
	selfAdd (const Integer& val);
	Integer
	shiftRight (size_t offset) const;
	Integer
	shiftLeft (size_t offset) const;
	Integer&
	selfShiftRight (size_t offset);
	Integer&
	selfShiftLeft (size_t offset);
	Integer
	bitAnd (const Integer& val) const;
	Integer
	bitAnd (uint64_t val) const;
	int
	compare (const Integer& val) const;
	int
	compare (uint64_t val) const;
};

}

#endif /* STRUCTURE_INTEGER_H_ */
