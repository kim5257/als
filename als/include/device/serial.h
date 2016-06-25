/*
 * serial.h
 *
 *  Created on: 2015. 11. 26.
 *      Author: arun
 */

#ifndef DEVICE_SERIAL_H_
#define DEVICE_SERIAL_H_

#include <unistd.h>
#include <stdint.h>

namespace device
{

class Serial
{
private:
	static int OPEN_FLAG;
private:
	int mDevice;
public:
	Serial (void);
	Serial (const char deviceName[],
			int baudrate);
	virtual
	~Serial (void);
public:
	void
	open (	const char deviceName[],
			int baudrate);
	void
	close (void);
	ssize_t
	read (	void* buf,
			int size);
	ssize_t
	write (	const void* buf,
			int size);
private:
	int
	transBaudrate (int baudrate);
};

}

#endif /* DEVICE_SERIAL_H_ */
