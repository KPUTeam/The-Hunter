#pragma once
#include "../Dx11Object.h"

DX11_BEGIN

class DX11_DLL CDx11PathManager :
	public CDx11Object
{
	DX11_SINGLE_DECLARE(CDx11PathManager)

private:
	unordered_map<string, wstring>		m_mapPath;
	char		m_strConvert[MAX_PATH];

public:
	bool Init();

	const wchar_t * CreatePath(
		wchar_t * pPath,
		const string & strKey,
		const string & strBaseKey = DX11_SOLUTION_PATH);

	const wchar_t* FindPath(const string& strKey);
	const char* FindPathToMultiByte(const string& strKey);
	void SetEnginePath(TCHAR* pPath);
};

DX11_END
