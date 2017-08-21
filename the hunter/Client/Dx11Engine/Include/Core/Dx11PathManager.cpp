#include "Dx11PathManager.h"

DX11_USING

DX11_SINGLE_DEFINITION(CDx11PathManager)

CDx11PathManager::CDx11PathManager()
{
	SetTypeName<CDx11PathManager>();
}

CDx11PathManager::~CDx11PathManager()
{
}

bool CDx11PathManager::Init()
{
	TCHAR	strPath[MAX_PATH] = {};

	// 현재 동작하고 있는 디렉토리 경로를 얻어온다.
	// .exe파일로 실행했을때, 컴파일해서 실행했을 때 경로가 다름.
	GetCurrentDirectory(MAX_PATH, strPath);

	lstrcat(strPath, L"//..//..//");

	//경로를 맵에 저장한다.
	m_mapPath.insert(make_pair(DX11_SOLUTION_PATH, strPath));

	// 리소시즈 폴더
	// CreatePath(L"Resources\\", DX11_RESOURCES_PATH);

	//쉐이더가 들어있는 폴더의 경로를 생성한다.
	CreatePath( L"Resources\\Shader\\", DX11_SHADER_PATH);

	//텍스처가 들어있는 폴더의 경로를 생성한다.
	CreatePath(L"Resources\\Textures\\", DX11_TEXTURE_PATH);

	//매쉬가 들어있는 폴더의 경로를 생성한다.
	CreatePath(L"Resources\\Mesh\\", DX11_MESH_PATH);

	return true;
}

const wchar_t * CDx11PathManager::CreatePath(
	wchar_t * pPath,
	const string & strKey, 
	const string & strBaseKey)
{
	if (FindPath(strKey))
		return NULL;

	//저장한 경로를 불러온다.
	wstring strPath = FindPath(strBaseKey);

	//경로에 pPath를 더하여 최종 경로를 만든다.
	strPath += pPath;

	//맵에 추가한다.
	m_mapPath.insert(make_pair(strKey, strPath));

	return strPath.c_str();
}

const wchar_t * CDx11PathManager::FindPath(const string & strKey)
{
	unordered_map<string, wstring>::iterator iter = m_mapPath.find(strKey);

	if (iter == m_mapPath.end())
		return NULL;

	return iter->second.c_str();
}

const char * CDx11PathManager::FindPathToMultiByte(const string & strKey)
{
	unordered_map<string, wstring>::iterator	iter = m_mapPath.find(strKey);

	if (iter == m_mapPath.end())
		return NULL;

	memset(m_strConvert, 0, MAX_PATH);

	WideCharToMultiByte(CP_ACP, 0, iter->second.c_str(), -1, m_strConvert,
		iter->second.length(), 0, 0);

	return m_strConvert;
}

void CDx11PathManager::SetEnginePath(TCHAR * pPath)
{
	m_mapPath.insert(make_pair("EnginePath", pPath));

	wstring	strPath = pPath;
	strPath += L"Resources";
	m_mapPath.insert(make_pair("EngineResourcesPath", strPath));

	strPath.clear();
	strPath = pPath;
	strPath += L"Resources\\Textures\\";
	m_mapPath.insert(make_pair("EngineTexturesPath", strPath));
}
