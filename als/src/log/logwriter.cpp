/*
 * logwriter.cpp
 *
 *  Created on: 2015. 7. 17.
 *      Author: asran
 */

#include "log/logwriter.h"
#include <iostream>
#include <stdlib.h>
#include <signal.h>
#include <execinfo.h>

namespace logwriter
{

LogWriter* LogWriter::mLogWriter = NULL;

/**
 * 생성자
 */
LogWriter::LogWriter (	const char path[],	///< 로그 생성 위치
						const char prefix[]	///< 로그 파일명 접두사
						)
		: mLogFile (NULL),
			mPath (path),
			mPrefix (prefix)
{
	init ();
}

/**
 * 소멸자
 */
LogWriter::~LogWriter (void)
{
	deinit ();
}

void
LogWriter::init (void)
{
}

void
LogWriter::deinit (void)
{
	close ();
}

void
LogWriter::open (std::string fileName)
{
	if ( mLogFile == NULL )
	{
		mLogFile = fopen (fileName.c_str (), "w");
	}
}

void
LogWriter::close (void)
{
	if ( mLogFile != NULL )
	{
		fclose (mLogFile);
		mLogFile = NULL;
	}
}

/**
 * 로그 파일 검사
 * @brief 기존 기록하던 로그파일이 없으면 새로 생성, \
 * 기존 기록하던 로그파일이 있으면, 기록주기에 따라 새로운 로그파일 생성
 */
void
LogWriter::chkFile (void)
{
	// 파일포인터가 NULL이면 파일 파로 생성
	if ( mLogFile == NULL )
	{
		std::string timeString;
		timeval curTime;

		gettimeofday (&curTime, NULL);

		mCreateTime = curTime;
		timeString = getTimeString (mCreateTime);
		mFileName = mPath + '/' + mPrefix + '_' + timeString + ".log";

		open (mFileName);
	}
	// 파일포인터가 NULL이 아니라면 파일 생성 주기에 따라
	// 기존 로그파일에 쓰기할 지, 새로 로그파일을 생성할 지 결정
	else
	{
		timeval curTime;
		timeval subTime;

		gettimeofday (&curTime, NULL);

		timersub(&curTime, &mCreateTime, &subTime);

		// 파일 생성 주기에 도달했는지 검사
		if ( LOG_WRITE_CYCLES < subTime.tv_sec )
		{
			std::string timeString;

			mCreateTime = curTime;
			timeString = getTimeString (mCreateTime);
			mFileName = mPath + '/' + mPrefix + '_' + timeString + ".log";

			close ();
			open (mFileName);
		}
	}
}

/**
 * 시간 정보를 문자열로 변환
 * @return	시간 정보를 변환 한 문자열
 */
std::string
LogWriter::getTimeString (const timeval& time) const
{
	char timeString[TIME_STRING_LEN] =
	{ 0, };

	tm *time2 = localtime (&time.tv_sec);	// 초/마이크로초 단위를 년월일시분초로 변경

	sprintf (	timeString,
				"%04d%02d%02d%02d%02d%02d",
				time2->tm_year + 1900,
				time2->tm_mon + 1,
				time2->tm_mday,
				time2->tm_hour,
				time2->tm_min,
				time2->tm_sec);

	return timeString;
}

void
LogWriter::write (	const char head[],
					const char tag[],
					const char str[])
{
	std::string message = "";
	std::string timeString;
	timeval curTime;

	chkFile ();

	gettimeofday (&curTime, NULL);

	timeString = getTimeString (curTime);

	message = message
				+ head
				+ "-["
				+ timeString
				+ "]-"
				+ tag
				+ ':'
				+ str
				+ '\n';
	fputs (message.c_str (), mLogFile);
}

void
LogWriter::info (	const char tag[],
					const char str[])
{
	write ("I", tag, str);
}

void
LogWriter::warning (const char tag[],
					const char str[])
{
	write ("W", tag, str);
}

void
LogWriter::error (	const char tag[],
					const char str[])
{
	write ("E", tag, str);
}

void
LogWriter::create (	const char path[],	///< 로그 생성 위치
					const char prefix[]	///< 로그 파일명 접두사
					)
{
	mLogWriter = new LogWriter (path, prefix);

	// segment fault 예외 발생 처리 핸들 등록
	struct sigaction sigsegv;

	sigsegv.sa_handler = errorHandler;
	sigemptyset (&sigsegv.sa_mask);
	sigsegv.sa_flags = SA_ONSTACK;

	sigaction (SIGSEGV, &sigsegv, 0);
}

void
LogWriter::destroy (void)
{
	delete mLogWriter;
}

LogWriter&
LogWriter::getInstance (void)
{
	return *mLogWriter;
}

void
LogWriter::errorHandler (int errno	///< 에러코드
)
{
	int funcCnt;
	void* funcList[NUM_OF_ERR_FUNC_STACK_LIST];
	char** funcSymbols;
	char errMsg[ERR_MSG_LEN] =
	{ 0, };

	sprintf (errMsg, "Segment fault occured - [0x%08x]", errno);
	LogWriter::getInstance ().error ("System", errMsg);
	LogWriter::getInstance ().error ("System", "Function stack ->");

	funcCnt = backtrace (funcList, NUM_OF_ERR_FUNC_STACK_LIST);
	funcSymbols = backtrace_symbols (funcList, funcCnt);
	for(int cnt = 0;cnt < funcCnt;++cnt)
	{
		sprintf (errMsg, "(%03d) - %s", cnt, funcSymbols[cnt]);
		LogWriter::getInstance ().error ("System", errMsg);
	}

	free (funcSymbols);

	LogWriter::destroy ();
	exit (errno);
}

}
