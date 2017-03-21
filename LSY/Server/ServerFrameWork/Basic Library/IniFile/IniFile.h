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

// Ini ������ ��Ʈ�� �� �� �ִ� IniFileŬ����
// Ini ���Ͽ��� �����͸� ������ �� ��� �����ʹ� ���ڿ��� �������� ���� �⺻���� �Ѵ�.
// �̰��� _stprintf�� _tstof���� �Լ��� ���ϴ� ������ �����ϴ� �۾��� �ϰ� �ȴ�.