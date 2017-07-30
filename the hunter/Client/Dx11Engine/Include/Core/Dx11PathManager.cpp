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

	// ���� �����ϰ� �ִ� ���丮 ��θ� ���´�.
	// .exe���Ϸ� ����������, �������ؼ� �������� �� ��ΰ� �ٸ�.
	GetCurrentDirectory(MAX_PATH, strPath);

	lstrcat(strPath, L"//..//..//");

	//��θ� �ʿ� �����Ѵ�.
	m_mapPath.insert(make_pair(DX11_SOLUTION_PATH, strPath));

	// ���ҽ��� ����
	// CreatePath(L"Resources\\", DX11_RESOURCES_PATH);

	//���̴��� ����ִ� ������ ��θ� �����Ѵ�.
	CreatePath( L"Resources\\Shader\\", DX11_SHADER_PATH);

	//�ؽ�ó�� ����ִ� ������ ��θ� �����Ѵ�.
	CreatePath(L"Resources\\Textures\\", DX11_TEXTURE_PATH);

	//�Ž��� ����ִ� ������ ��θ� �����Ѵ�.
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

	//������ ��θ� �ҷ��´�.
	wstring strPath = FindPath(strBaseKey);

	//��ο� pPath�� ���Ͽ� ���� ��θ� �����.
	strPath += pPath;

	//�ʿ� �߰��Ѵ�.
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
