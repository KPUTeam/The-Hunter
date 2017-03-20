#include "Registry.h"



CRegistry::CRegistry()
{
	mRootKey = NULL;
	mIsOpened = FALSE;
}


CRegistry::~CRegistry()
{
}

// ó�� Ű�� ���� ����Ѵ�.
BOOL CRegistry::Open(HKEY rootKey, LPCTSTR subKey)
{
	if (!subKey)
		return FALSE;

	// �̹� �������� ��� ����ó��
	if (mIsOpened)
		return FALSE;

	// ������ Ű�� ��� mRootKey ������ �����Ѵ�.
	if (RegOpenKey(rootKey, subKey, &mRootKey) != ERROR_SUCCESS)
		return FALSE;

	// ������ ����� Ű�� �������� �Ǵ��ϴ� ����
	// ���� true�� ������
	mIsOpened = TRUE;

	return TRUE;
}

// Ű�� ���� �� ����Ѵ�.
BOOL CRegistry::Close()
{
	if (RegCloseKey(mRootKey) != ERROR_SUCCESS)
		return FALSE;

	// Ű�� �����鼭 FALSE�� ��������
	mIsOpened = FALSE;
	
	return TRUE;
}

// Ű�� ������ �� ����ϴ� �Լ�
BOOL CRegistry::CreateKey(HKEY rootKey, LPCTSTR subKey)
{
	// rootKey�� �������� ������ subKey�� �Է¹޾� Ű�� �����Ѵ�.
	if (!subKey)
		return FALSE;

	// �̹� �������� ���� ����
	if (mIsOpened)
		return FALSE;

	// ������Ʈ�� Ű�� �����ϴ� API �Լ��� ȣ���Ѵ�.
	if (RegCreateKey(rootKey, subKey, &mRootKey) != ERROR_SUCCESS)
		return FALSE;

	// OpenKey�ܿ� �����ϸ鼭 �ڵ����� ���� ���·� �����Ѵ�.
	mIsOpened = TRUE;

	return TRUE;
}

// Ű�� ������ �� ����ϴ� �Լ�
BOOL CRegistry::DeleteKey(HKEY rootKey, LPCTSTR subKey)
{
	if (!subKey)
		return FALSE;

	// ������Ʈ�� Ű�� �����ϴ� API �Լ��� ȣ���Ѵ�.
	if (RegDeleteKey(rootKey, subKey) != ERROR_SUCCESS)
		return FALSE;

	return TRUE;
}

// Ű�� �޸� Create�� �ʿ� ���� SetValue�� �� �� ���� ���� ��� �����ȴ�.
BOOL CRegistry::SetValue(LPCTSTR valueName, LPCTSTR value)
{
	// TCHAR���� ���ڿ� �����͸� ������Ʈ���� �Է��� �� ����մϴ�.
	if (!valueName || !value)
		return FALSE;

	if (!mIsOpened)
		return FALSE;

	// REG_SZ ���� �����͸� �Է��Ѵ�.
	if (RegSetValueEx(mRootKey, valueName, 0, REG_SZ, (BYTE*)value,
		(DWORD)_tcslen(value) * sizeof(TCHAR)) != ERROR_SUCCESS)
		return FALSE;

	return TRUE;
}

// DWORD�� ���� �����ϴ� �Լ�
BOOL CRegistry::SetValue(LPCTSTR valueName, DWORD value)
{
	if (!valueName)
		return FALSE;

	if (!mIsOpened)
		return FALSE;

	// REG_DWORD ���·� �����Ѵ�.
	if (RegSetValueEx(mRootKey, valueName, 0, REG_DWORD, (BYTE*)&value, sizeof(DWORD)) != ERROR_SUCCESS)
		return FALSE;

	return TRUE;
}

// ���� �� ���� ���� MultiSZ���� ���ڿ��� ����� �� ���� �Լ�
BOOL CRegistry::SetValueForMultiSz(LPCTSTR valueName, LPCTSTR value, DWORD byteLength)
{
	if (!valueName || !value)
		return FALSE;

	if (!mIsOpened)
		return FALSE;

	// REG_MULTi_SZ�� �����͸� �Է��մϴ�.
	if (RegSetValueEx(mRootKey, valueName, 0, REG_MULTI_SZ, (BYTE*)value, byteLength) != ERROR_SUCCESS)
		return FALSE;

	return TRUE;
}

// TCHAR���� ���ڿ� �����͸� �о���� �Լ�
BOOL CRegistry::GetValue(LPCTSTR valueName, LPCTSTR value, LPDWORD bufferLength)
{
	DWORD valueType = 0;

	if (!valueName || !value || !bufferLength)
		return FALSE;

	if (!mIsOpened)
		return FALSE;

	// ���ڿ� �����͸� �о�´�.
	// bufferLength�� ����� ���̸�ŭ �о�´�.
	if (RegQueryValueEx(mRootKey, valueName, 0, &valueType, (BYTE*)value, bufferLength) != ERROR_SUCCESS)
		return FALSE;

	return TRUE;
}

// DWORD�� �����͸� �о���� �Լ�
BOOL CRegistry::GetValue(LPCTSTR valueName, LPDWORD value)
{
	DWORD valueType = 0;
	DWORD BufferLength = sizeof(DWORD);

	if (!valueName || !value)
		return FALSE;

	if (!mIsOpened)
		return FALSE;

	// TCHAR�� ���ڿ��� ������ �Ͱ� ���� ���·� ���� �����´�.
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

	// ������Ʈ���� ���� �����ϴ� API �Լ��� ȣ���Ѵ�.
	if (RegDeleteValue(mRootKey, valueName) != ERROR_SUCCESS)
		return FALSE;

	return 0;
}
