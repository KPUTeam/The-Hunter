#pragma once
#include "../Include.h"

class CIniFile
{
private:
	TCHAR mFileName[MAX_PATH];

public:
	CIniFile();
	virtual ~CIniFile();

public:
	BOOL Open(LPCTSTR fileName);
	BOOL Close();

	BOOL SetValue(LPCTSTR keyName, LPCTSTR valueName, LPCTSTR value);
	BOOL SetValue(LPCTSTR keyName, LPCTSTR valueName, DWORD value);
	BOOL SetValue(LPCTSTR keyName, LPCTSTR valueName, FLOAT value);

	BOOL GetValue(LPCTSTR KeyName, LPCTSTR valueName, LPTSTR value, LPDWORD bufferLength);
	BOOL GetValue(LPCTSTR KeyName, LPCTSTR valueName, LPDWORD value);
	BOOL GetValue(LPCTSTR KeyName, LPCTSTR valueName, FLOAT* value);
};

// Ini 파일을 컨트롤 할 수 있는 IniFile클래스
// Ini 파일에서 데이터를 가져올 때 모든 데이터는 문자열로 가져오는 것을 기본으로 한다.
// 이것을 _stprintf나 _tstof등의 함수로 원하는 형으로 변경하는 작업을 하게 된다.