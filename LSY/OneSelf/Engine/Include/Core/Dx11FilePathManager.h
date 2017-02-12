#pragma once
#include "../Dx11Object.h"

DX11_BEGIN

class DX11_DLL CDx11FilePathManager :
	public CDx11Object
{
	DX11_DECLARE_SINGLE(CDx11FilePathManager)

private:
	unordered_map<string, wstring>		m_mapPath;

public:
	bool Init();
	const wchar_t* CreatePath(const string& strKey, wchar_t* pPath,
		const string& strBaseKey = ROOT_PATH);
	const wchar_t* FindPath(const string& strKey);
};

DX11_END
