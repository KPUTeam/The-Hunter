#include "Dx11FilePathManager.h"

DX11_USING

DX11_DEFINITION_SINGLE(CDx11FilePathManager)

CDx11FilePathManager::CDx11FilePathManager()
{
	SetTypeName<CDx11FilePathManager>();
}

CDx11FilePathManager::~CDx11FilePathManager()
{
}

bool CDx11FilePathManager::Init()
{
	TCHAR	strPath[MAX_PATH] = {};

	GetCurrentDirectory(MAX_PATH, strPath);

	int	iLength = lstrlen(strPath);

	for (int i = iLength - 1; i >= 0; --i)
	{
		if (strPath[i] == '/' || strPath[i] == '\\')
		{
			memset(&strPath[i + 1], 0, sizeof(TCHAR) * (MAX_PATH - (i + 1)));
			break;
		}
	}

#ifdef _DEBUG
	lstrcat(strPath, L"Bin_Debug\\");
#else
	lstrcat(strPath, L"Bin\\");
#endif // _DEBUG

	m_mapPath.insert(make_pair(ROOT_PATH, strPath));

	CreatePath(SHADER_PATH, L"Shader\\");
	CreatePath(TEXTURE_PATH, L"Textures\\");
	
	return true;
}

const wchar_t * CDx11FilePathManager::CreatePath(const string & strKey, 
	wchar_t * pPath, const string & strBaseKey)
{
	if (FindPath(strKey))
		return NULL;

	wstring	strPath = FindPath(strBaseKey);
	strPath += pPath;

	m_mapPath.insert(make_pair(strKey, strPath));

	return strPath.c_str();
}

const wchar_t * CDx11FilePathManager::FindPath(const string & strKey)
{
	unordered_map<string, wstring>::iterator	iter = m_mapPath.find(strKey);

	if (iter == m_mapPath.end())
		return NULL;

	return iter->second.c_str();

	return nullptr;
}
