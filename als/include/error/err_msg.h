/*
 * @author		kjg
 * @date		2015. 6. 24.
 */

#ifndef ERR_MSG_H_
#define ERR_MSG_H_

#include <stdio.h>
#include <string.h>
#include <new>

namespace error
{

#define	MAX_ERR_STRING_LEN	(255)		///< 에러 메시지 최대 길이

/**
 * 에러 메시지 전달 클래스
 */
class ErrMsg
{
private:
	char* mErrString;		///< 에러 메시지
	int mErrCode;
private:
	inline
	ErrMsg (int errCode);
	inline virtual
	~ErrMsg (void);
private:
	inline bool
	setErrString (const char string[]);
	inline void
	delErrString (void);
public:
	inline const char*
	getErrString (void);
	inline int
	getErrCode (void);
public:
	inline static ErrMsg*
	createErrMsg (const char string[]);
	inline static ErrMsg*
	createErrMsg (	const char string[],
					int errCode);
	inline static void
	destroyErrMsg (ErrMsg* errMsg);
};

/**
 * 생성자
 */
ErrMsg::ErrMsg (int errCode)
		: mErrString (NULL),
			mErrCode (errCode)
{

}

/**
 * 소멸자
 */
ErrMsg::~ErrMsg (void)
{
	delErrString ();
}

/**
 * 에러 메시지 문자열 할당 및 메시지 설정
 * @return 에러 메시지 문자열 할당 및 설정에 성공하면 true, 할당에 실패하면 false
 */
bool
ErrMsg::setErrString (const char string[]	///< 에러 메시지
)
{
	bool ret = false;

	do
	{
		size_t length = strnlen (string, MAX_ERR_STRING_LEN);
		if ( length == 0 )
		{
			break;
		}

		try
		{
			mErrString = new char[length + 1];	// 문자열 + '\0' 길이
		}
		catch( std::bad_alloc& exception )
		{
			throw exception;		// 에러 클래스에서 에러 발생 시 할당 실패 예외 던짐
		}

		memcpy (mErrString, string, length + 1);

		ret = true;
	}
	while( 0 );

	return ret;
}

/**
 * 에러 메시지 문자열 할당 해제
 */
void
ErrMsg::delErrString (void)
{
	delete[] mErrString;
}

/**
 * 에러 메시지 가져오기
 * @return 저장 된 에러메시지
 */
const char*
ErrMsg::getErrString (void)
{
	return mErrString;
}

int
ErrMsg::getErrCode (void)
{
	return mErrCode;
}

/**
 * 에러 메시지 전달 객체 생성
 * @return 생성한 에러 메시지 전달 객체
 */
ErrMsg*
ErrMsg::createErrMsg (const char string[]	///< 에러 메시지
)
{
	return createErrMsg (string, (-1));
}

ErrMsg*
ErrMsg::createErrMsg (	const char string[],
						int errCode)
{
	ErrMsg* errMsg = NULL;

	errMsg = new ErrMsg (errCode);
	if ( errMsg->setErrString (string) == false )
	{
		delete errMsg;
		errMsg = NULL;
	}

	return errMsg;
}

/**
 * 에러 메시지 전달 객체 제거
 */
void
ErrMsg::destroyErrMsg (ErrMsg* errMsg			///< 에러 메시지 전달 객체
)
{
	delete errMsg;
}

}
;

#endif /* ERR_MSG_H_ */
