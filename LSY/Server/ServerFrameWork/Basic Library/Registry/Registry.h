#pragma once

#include "../Include.h"

class CRegistry
{
private:
	HKEY mRootKey;
	BOOL mIsOpened;

public:
	CRegistry();
	virtual ~CRegistry();

public:
	BOOL Open(HKEY rootKey, LPCTSTR subKey);
	BOOL Close();

	BOOL CreateKey(HKEY rootKey, LPCTSTR subKey);
	BOOL DeleteKey(HKEY rootKey, LPCTSTR subKey);

	BOOL SetValue(LPCTSTR valueName, LPCTSTR value);
	BOOL SetValue(LPCTSTR valueName, DWORD value);
	BOOL SetValueForMultiSz(LPCTSTR valueName, LPCTSTR value, DWORD byteLength);

	BOOL GetValue(LPCTSTR valueName, LPCTSTR value, LPDWORD bufferLength);
	BOOL GetValue(LPCTSTR valueName, LPDWORD value);

	BOOL Deletevalue(LPCTSTR valueName);
};

// 레지스트리를 관리하는 클래스