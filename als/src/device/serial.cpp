/*
 * serial.cpp
 *
 *  Created on: 2015. 11. 26.
 *      Author: arun
 */

#include "device/serial.h"
#include "error/err_msg.h"
#include <fcntl.h>
#include <termios.h>

namespace device
{

int Serial::OPEN_FLAG = O_RDWR | O_NOCTTY | O_NONBLOCK;

Serial::Serial (void)
		: mDevice (0)
{

}

Serial::Serial (const char deviceName[],
				int baudrate)
{
	open (deviceName, baudrate);
}

Serial::~Serial (void)
{
	close ();
}

void
Serial::open (	const char deviceName[],
				int baudrate)
{
	termios info =
	{ 0, };
	termios targetInfo =
	{ 0, };

	info.c_cflag = transBaudrate (baudrate);
	info.c_cflag |= CS8;
	info.c_cflag |= CLOCAL;
	info.c_cflag |= CREAD;
	info.c_iflag = 0;
	info.c_oflag = 0;
	info.c_lflag = 0;
	info.c_cc[VTIME] = 0;
	info.c_cc[VMIN] = 0;

	mDevice = ::open (deviceName, OPEN_FLAG);
	if ( mDevice < 0 )
	{
		throw error::ErrMsg::createErrMsg ("장치 열기에 실패했습니다.", mDevice);
		mDevice = 0;
	}

	tcsetattr (mDevice, TCSANOW, &info);

	// 장치 기본 설정이 문제없이 적용되었는지 확인하기 위해 장치 설정을 가져와서 요청한 설정 값과 비교
	tcgetattr (mDevice, &targetInfo);
	if ( memcmp (&info, &targetInfo, sizeof(termios)) != 0 )
	{
		close ();
		throw error::ErrMsg::createErrMsg ("장치 설정에 실패했습니다.");
	}
}

void
Serial::close (void)
{
	if ( mDevice != 0 )
	{
		::close (mDevice);
		mDevice = 0;
	}
}

int
Serial::transBaudrate (int baudrate)
{
	int setVal = 0;

	switch( baudrate )
	{
	case 57600:
		setVal = B57600;
		break;
	case 115200:
		setVal = B115200;
		break;
	case 230400:
		setVal = B230400;
		break;
	case 460800:
		setVal = B460800;
		break;
	case 500000:
		setVal = B500000;
		break;
	case 576000:
		setVal = B576000;
		break;
	case 921600:
		setVal = B921600;
		break;
	case 1000000:
		setVal = B1000000;
		break;
	case 1152000:
		setVal = B1152000;
		break;
	case 1500000:
		setVal = B1500000;
		break;
	case 2000000:
		setVal = B2000000;
		break;
	case 2500000:
		setVal = B2500000;
		break;
	case 3000000:
		setVal = B3000000;
		break;
	case 3500000:
		setVal = B3500000;
		break;
	case 4000000:
		setVal = B4000000;
		break;
	default:
		break;
	}

	if ( setVal == 0 )
	{
		throw error::ErrMsg::createErrMsg ("올바른 장치 속도가 아닙니다.");
	}

	return setVal;
}

ssize_t
Serial::read (	void* buf,
				int size)
{
	::read(mDevice, buf, size);
	return 0;
}

ssize_t
Serial::write (	const void* buf,
				int size)
{
	return 0;
}

}
