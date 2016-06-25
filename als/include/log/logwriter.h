/*
 * @author		kjg
 * @date		2015. 7. 17.
 */

#ifndef LOGWRITER_H_
#define LOGWRITER_H_

#include <stdio.h>
#include <string>
#include <sys/time.h>

#define	LOG_WRITE_CYCLES				(60 * 60)		///< 로그 기록 주기 (3600초 = 1시간)
#define	TIME_STRING_LEN				(15)			///< 시간 문자열 최대 길이 (20150510151023)
#define	NUM_OF_ERR_FUNC_STACK_LIST	(100)			///< 에러 발생 시 Stack 리스트 개수
#define	ERR_MSG_LEN					(1024)			///< 에러 메시지 길이

namespace logwriter
{

/**
 * 로그 기록 클래스
 */
class LogWriter
{
private:
	static LogWriter* mLogWriter;	///< 로그 기록 클래스 객체
private:
	timeval mCreateTime;		///< 생성 시간
	FILE* mLogFile;			///< 로그 파일 포인터
	std::string mPath;		///< 로그 생성 위치
	std::string mPrefix;		///< 로그 파일명 접두사
	std::string mFileName;	///< 로그 파일 이름
private:
	LogWriter (	const char path[],
				const char prefix[]);
	virtual
	~LogWriter (void);
private:
	void
	init (void);
	void
	deinit (void);
	void
	open (std::string fileName);
	void
	close (void);
	void
	chkFile (void);
	std::string
	getTimeString (const timeval& time) const;
	void
	write (	const char head[],
			const char tag[],
			const char str[]);
public:
	void
	info (	const char tag[],
			const char str[]);
	void
	warning (	const char tag[],
				const char str[]);
	void
	error (	const char tag[],
			const char str[]);
public:
	static void
	create (const char fileName[],
			const char prefix[]);
	static void
	destroy (void);
	static LogWriter&
	getInstance (void);
	static void
	errorHandler (int errno);
};

}

#endif /* LOGWRITER_H_ */
