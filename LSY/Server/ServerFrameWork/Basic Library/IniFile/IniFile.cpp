#include "IniFile.h"



CIniFile::CIniFile()
{
	ZeroMemory(mFileName, sizeof(mFileName));
}


CIniFile::~CIniFile()
{
}

// Ini ������ ó�� �����ϴ� �Լ��̴�.
// �Ķ���ʹ� �� TCHAR������ �޴´�.
BOOL CIniFile::Open(LPCTSTR fileName)
{
	// ���� �̸��� �߸� ���� ��� �����Ѵ�.
	if (!fileName)
		return FALSE;

	// �̸� ���ǵǾ� �ִ� MAX_PATH�� �̿��ؼ� �����Ѵ�
	_tcsncpy(mFileName, fileName, MAX_PATH);

	return TRUE;
}

BOOL CIniFile::Close()
{
	return TRUE;
}

// �Ϲ� ���ڿ� ���� �� �� ����Ѵ�.
BOOL CIniFile::SetValue(LPCTSTR keyName, LPCTSTR valueName, LPCTSTR value)
{
	if (!keyName || !valueName || !value)
		return FALSE;

	WritePrivateProfileString(keyName, valueName, value, mFileName);

	return TRUE;
}

// DWORD�� ���� �� �� ����Ѵ�.
BOOL CIniFile::SetValue(LPCTSTR keyName, LPCTSTR valueName, DWORD value)
{
	if (!keyName || !valueName)
		return FALSE;

	// �� ��ȯ �Լ��� �̿��ؼ� ���� ��ȯ�Ѵ�.
	TCHAR Value[16] = { 0, };
	_itot(value, Value, 10);

	WritePrivateProfileString(keyName, valueName, Value, mFileName);

	return TRUE;
}

// FLOAT�� �����͸� �� �� ����Ѵ�.
BOOL CIniFile::SetValue(LPCTSTR keyName, LPCTSTR valueName, FLOAT value)
{
	if (!keyName || !valueName)
		return FALSE;

	TCHAR Value[16] = { 0, };
	_stprintf(Value, _T("%f"), value);

	WritePrivateProfileString(keyName, valueName, Value, mFileName);

	return TRUE;
}

// ���ڿ��� �������� �Լ��̴�.
BOOL CIniFile::GetValue(LPCTSTR KeyName, LPCTSTR valueName, LPTSTR value, LPDWORD bufferLength)
{
	if (!KeyName || !valueName || !value || !bufferLength)
		return FALSE;

	*bufferLength = GetPrivateProfileString(KeyName, valueName, _T(""), value,
		*bufferLength, mFileName);

	return TRUE;
}

// DWROD�� ���� �������� �Լ��̴�.
// Key, valueName�� �Է��ϸ� value�� ����Ѵ�.
BOOL CIniFile::GetValue(LPCTSTR KeyName, LPCTSTR valueName, LPDWORD value)
{
	// ���ڿ� ������ �Ķ���ʹ� �� ���� üũ���ش�.
	if (!KeyName || !valueName || !value)
		return FALSE;

	*value = GetPrivateProfileInt(KeyName, valueName, 0, mFileName);

	return TRUE;
}

// FLOAT�� ���� �������� �Լ��̴�.
BOOL CIniFile::GetValue(LPCTSTR KeyName, LPCTSTR valueName, FLOAT * value)
{
	if (!KeyName || !valueName || !value)
		return FALSE;

	TCHAR Value[16] = { 0, };

	// FLOAT���� ���� �������� �Լ��� ���� ������ �ϴ� ���ڿ� ���·� �����ͼ� ��ȯ�� ���ش�.
	GetPrivateProfileString(KeyName, valueName, _T(""), Value, 16, mFileName);

	*value = (FLOAT)_tstof(Value);

	return TRUE;
}
