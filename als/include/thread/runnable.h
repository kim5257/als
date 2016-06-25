/*
 * runnable.h
 *
 *  Created on: 2015. 8. 21.
 *      Author: asran
 */

#ifndef THREAD_RUNNABLE_H_
#define THREAD_RUNNABLE_H_

#include <stdint.h>

namespace thread
{

class Runnable
{
public:
	Runnable (void);
	virtual
	~Runnable (void);
public:
	virtual void*
	run (uint8_t* exitFlag)=0;
};

}

#endif /* THREAD_RUNNABLE_H_ */
