#include "IniFile.h"



CIniFile::CIniFile()
{
	ZeroMemory(mFileName, sizeof(mFileName));
}


CIniFile::~CIniFile()
{
}

// Ini 파일을 처음 오픈하는 함수이다.
// 파라미터는 꼭 TCHAR형으로 받는다.
BOOL CIniFile::Open(LPCTSTR fileName)
{
	// 파일 이름이 잘못 들어온 경우 실패한다.
	if (!fileName)
		return FALSE;

	// 미리 정의되어 있는 MAX_PATH를 이용해서 복사한다
	_tcsncpy(mFileName, fileName, MAX_PATH);

	return TRUE;
}

BOOL CIniFile::Close()
{
	return TRUE;
}

// 일반 문자열 형을 쓸 때 사용한다.
BOOL CIniFile::SetValue(LPCTSTR keyName, LPCTSTR valueName, LPCTSTR value)
{
	if (!keyName || !valueName || !value)
		return FALSE;

	WritePrivateProfileString(keyName, valueName, value, mFileName);

	return TRUE;
}

// DWORD형 값을 쓸 때 사용한다.
BOOL CIniFile::SetValue(LPCTSTR keyName, LPCTSTR valueName, DWORD value)
{
	if (!keyName || !valueName)
		return FALSE;

	// 형 변환 함수를 이용해서 값을 변환한다.
	TCHAR Value[16] = { 0, };
	_itot(value, Value, 10);

	WritePrivateProfileString(keyName, valueName, Value, mFileName);

	return TRUE;
}

// FLOAT형 데이터를 쓸 때 사용한다.
BOOL CIniFile::SetValue(LPCTSTR keyName, LPCTSTR valueName, FLOAT value)
{
	if (!keyName || !valueName)
		return FALSE;

	TCHAR Value[16] = { 0, };
	_stprintf(Value, _T("%f"), value);

	WritePrivateProfileString(keyName, valueName, Value, mFileName);

	return TRUE;
}

// 문자열을 가져오는 함수이다.
BOOL CIniFile::GetValue(LPCTSTR KeyName, LPCTSTR valueName, LPTSTR value, LPDWORD bufferLength)
{
	if (!KeyName || !valueName || !value || !bufferLength)
		return FALSE;

	*bufferLength = GetPrivateProfileString(KeyName, valueName, _T(""), value,
		*bufferLength, mFileName);

	return TRUE;
}

// DWROD형 값을 가져오는 함수이다.
// Key, valueName을 입력하면 value를 출력한다.
BOOL CIniFile::GetValue(LPCTSTR KeyName, LPCTSTR valueName, LPDWORD value)
{
	// 문자열 형태의 파라미터는 꼭 값을 체크해준다.
	if (!KeyName || !valueName || !value)
		return FALSE;

	*value = GetPrivateProfileInt(KeyName, valueName, 0, mFileName);

	return TRUE;
}

// FLOAT형 값을 가져오는 함수이다.
BOOL CIniFile::GetValue(LPCTSTR KeyName, LPCTSTR valueName, FLOAT * value)
{
	if (!KeyName || !valueName || !value)
		return FALSE;

	TCHAR Value[16] = { 0, };

	// FLOAT형을 따로 가져오는 함수가 없기 때문에 일단 문자열 형태로 가져와서 변환을 해준다.
	GetPrivateProfileString(KeyName, valueName, _T(""), Value, 16, mFileName);

	*value = (FLOAT)_tstof(Value);

	return TRUE;
}
