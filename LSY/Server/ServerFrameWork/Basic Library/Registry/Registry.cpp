#include "Registry.h"



CRegistry::CRegistry()
{
	mRootKey = NULL;
	mIsOpened = FALSE;
}


CRegistry::~CRegistry()
{
}

// 처음 키를 열때 사용한다.
BOOL CRegistry::Open(HKEY rootKey, LPCTSTR subKey)
{
	if (!subKey)
		return FALSE;

	// 이미 열려있을 경우 실패처리
	if (mIsOpened)
		return FALSE;

	// 지정된 키를 열어서 mRootKey 변수에 저장한다.
	if (RegOpenKey(rootKey, subKey, &mRootKey) != ERROR_SUCCESS)
		return FALSE;

	// 앞으로 사용할 키를 열었는지 판단하는 변수
	// 값을 true로 세팅함
	mIsOpened = TRUE;

	return TRUE;
}

// 키를 닫을 때 사용한다.
BOOL CRegistry::Close()
{
	if (RegCloseKey(mRootKey) != ERROR_SUCCESS)
		return FALSE;

	// 키를 닫으면서 FALSE로 세팅해줌
	mIsOpened = FALSE;
	
	return TRUE;
}

// 키를 생성할 때 사용하는 함수
BOOL CRegistry::CreateKey(HKEY rootKey, LPCTSTR subKey)
{
	// rootKey를 기준으로 생성할 subKey를 입력받아 키를 생성한다.
	if (!subKey)
		return FALSE;

	// 이미 열려있을 경우는 실패
	if (mIsOpened)
		return FALSE;

	// 레지스트리 키를 생성하는 API 함수를 호출한다.
	if (RegCreateKey(rootKey, subKey, &mRootKey) != ERROR_SUCCESS)
		return FALSE;

	// OpenKey외에 생성하면서 자동으로 열린 형태로 세팅한다.
	mIsOpened = TRUE;

	return TRUE;
}

// 키를 삭제할 때 사용하는 함수
BOOL CRegistry::DeleteKey(HKEY rootKey, LPCTSTR subKey)
{
	if (!subKey)
		return FALSE;

	// 레지스트리 키를 삭제하는 API 함수를 호출한다.
	if (RegDeleteKey(rootKey, subKey) != ERROR_SUCCESS)
		return FALSE;

	return TRUE;
}

// 키와 달리 Create할 필요 없이 SetValue를 할 때 값이 없을 경우 설정된다.
BOOL CRegistry::SetValue(LPCTSTR valueName, LPCTSTR value)
{
	// TCHAR형의 문자열 데이터를 레지스트리에 입력할 때 사용합니다.
	if (!valueName || !value)
		return FALSE;

	if (!mIsOpened)
		return FALSE;

	// REG_SZ 형을 데이터를 입력한다.
	if (RegSetValueEx(mRootKey, valueName, 0, REG_SZ, (BYTE*)value,
		(DWORD)_tcslen(value) * sizeof(TCHAR)) != ERROR_SUCCESS)
		return FALSE;

	return TRUE;
}

// DWORD형 값을 지정하는 함수
BOOL CRegistry::SetValue(LPCTSTR valueName, DWORD value)
{
	if (!valueName)
		return FALSE;

	if (!mIsOpened)
		return FALSE;

	// REG_DWORD 형태로 저장한다.
	if (RegSetValueEx(mRootKey, valueName, 0, REG_DWORD, (BYTE*)&value, sizeof(DWORD)) != ERROR_SUCCESS)
		return FALSE;

	return TRUE;
}

// 별로 쓸 일이 없는 MultiSZ형의 문자열을 사용할 때 쓰는 함수
BOOL CRegistry::SetValueForMultiSz(LPCTSTR valueName, LPCTSTR value, DWORD byteLength)
{
	if (!valueName || !value)
		return FALSE;

	if (!mIsOpened)
		return FALSE;

	// REG_MULTi_SZ형 데이터를 입력합니다.
	if (RegSetValueEx(mRootKey, valueName, 0, REG_MULTI_SZ, (BYTE*)value, byteLength) != ERROR_SUCCESS)
		return FALSE;

	return TRUE;
}

// TCHAR형의 문자열 데이터를 읽어오는 함수
BOOL CRegistry::GetValue(LPCTSTR valueName, LPCTSTR value, LPDWORD bufferLength)
{
	DWORD valueType = 0;

	if (!valueName || !value || !bufferLength)
		return FALSE;

	if (!mIsOpened)
		return FALSE;

	// 문자열 데이터를 읽어온다.
	// bufferLength에 저장된 길이만큼 읽어온다.
	if (RegQueryValueEx(mRootKey, valueName, 0, &valueType, (BYTE*)value, bufferLength) != ERROR_SUCCESS)
		return FALSE;

	return TRUE;
}

// DWORD형 데이터를 읽어오는 함수
BOOL CRegistry::GetValue(LPCTSTR valueName, LPDWORD value)
{
	DWORD valueType = 0;
	DWORD BufferLength = sizeof(DWORD);

	if (!valueName || !value)
		return FALSE;

	if (!mIsOpened)
		return FALSE;

	// TCHAR형 문자열을 가져온 것과 같은 형태로 값을 가져온다.
	if (RegQueryValueEx(mRootKey, valueName, 0, &valueType, (BYTE*)value, &BufferLength) != ERROR_SUCCESS)
		return FALSE;

	return TRUE;
}

BOOL CRegistry::Deletevalue(LPCTSTR valueName)
{
	if (!valueName)
		return FALSE;

	if (!mIsOpened)
		return FALSE;

	// 레지스트리의 값을 삭제하는 API 함수를 호출한다.
	if (RegDeleteValue(mRootKey, valueName) != ERROR_SUCCESS)
		return FALSE;

	return 0;
}
