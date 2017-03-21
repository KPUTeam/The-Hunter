#pragma once

#include "../Include.h"

class CLog
{
public:
	static BOOL	WriteLog(LPTSTR data, ...)
	{
		SYSTEMTIME SystemTime;
		TCHAR	   CurrentDate[32]              = { 0, };
		TCHAR	   CurrentFileName[MAX_PATH]    = { 0, };
		FILE*	   FilePtr					    = NULL;
		TCHAR	   DebugLog[MAX_BUFFER_LENGTH]	= { 0, };

		va_list		ap;
		TCHAR	    Log[MAX_BUFFER_LENGTH] = { 0, };

		va_start(ap, data);
		_vstprintf(Log, data, ap);
		va_end(ap);

		GetLocalTime(&SystemTime);
		_sntprintf(CurrentDate, 32, _T("%d-%d-%d %d:%d:%d"),
			SystemTime.wYear,
			SystemTime.wMonth,
			SystemTime.wDay,
			SystemTime.wHour,
			SystemTime.wMinute,
			SystemTime.wSecond);

		_sntprintf(CurrentFileName, MAX_PATH, _T("LOG_%d-%d-%d %d.log"),
			SystemTime.wYear,
			SystemTime.wMonth,
			SystemTime.wDay,
			SystemTime.wHour);

		FilePtr = _tfopen(CurrentFileName, _T("a"));
		if (!FilePtr)
			return FALSE;

		_ftprintf(FilePtr, _T("[%s] %s\n"), CurrentDate, Log);
		_sntprintf(DebugLog, MAX_BUFFER_LENGTH, _T("[%s] %s \n"), CurrentDate, Log);

		fflush(FilePtr);

		fclose(FilePtr);

		OutputDebugString(DebugLog);
		_tprintf(_T("%s"), DebugLog);

		return TRUE;
	}

	static BOOL WriteLogNoDate(LPTSTR data, ...)
	{
		SYSTEMTIME	SystemTime;
		TCHAR		CurrentDate[32]				= { 0, };
		TCHAR		CurrentFileName[MAX_PATH]	= { 0, };
		FILE*		FilePtr						= NULL;
		TCHAR		DebugLog[MAX_BUFFER_LENGTH] = { 0, };

		va_list		ap;
		TCHAR		Log[MAX_BUFFER_LENGTH]		= { 0, };

		va_start(ap, data);
		_vstprintf(Log, data, ap);
		va_end(ap);

		GetLocalTime(&SystemTime);
		_sntprintf(CurrentDate, MAX_PATH, _T("%d-%d-%d %d:%d:%d"),
			SystemTime.wYear,
			SystemTime.wMonth,
			SystemTime.wDay,
			SystemTime.wHour);

		_sntprintf(CurrentFileName, MAX_PATH, _T("LOG_%d-%d-%d %d.log"),
			SystemTime.wYear,
			SystemTime.wMonth,
			SystemTime.wDay,
			SystemTime.wHour);

		FilePtr = _tfopen(CurrentFileName, _T("a"));
		if (!FilePtr)
			return FALSE;

		_ftprintf(FilePtr, _T("%s"), Log);
		_sntprintf(DebugLog, MAX_BUFFER_LENGTH, _T("%s"), Log);

		fflush(FilePtr);

		fclose(FilePtr);

		OutputDebugString(DebugLog);
		_tprintf(_T("%s"), DebugLog);

		return TRUE;
	}

};

// 서버프로그래밍 = 로그 라는 말이 있을 정도로 로그는 서버 프로그래밍에서 가장 중요하다.
// 특히 다중 스레드 환경에서는 동작 확인을 할 수 있는 가장 좋은 수단이 로그이다.
// 여기서는 시간 기준으로 로그를 남기는것으로 코드를 작성한다.