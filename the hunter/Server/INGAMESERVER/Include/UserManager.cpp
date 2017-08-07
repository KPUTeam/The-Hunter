#include "UserManager.h"
#include "UserScript.h"

CUserManager* CUserManager::m_pInst = NULL;

CUserManager::CUserManager()
{
}


CUserManager::~CUserManager()
{
	Safe_Release_Map(m_mapUser);
}

bool CUserManager::Init()
{
	return true;
}

bool CUserManager::AddUser(INT64 iKey, CDx11GameObject * pObj)
{
	CDx11GameObject*	pFindObj = FindUser(iKey);

	if (pFindObj)
	{
		SAFE_RELEASE(pFindObj);
		return false;
	}

	m_mapUser.insert(make_pair(iKey, pObj));
	return true;
}

CDx11GameObject * CUserManager::FindUser(INT64 iKey)
{
	unordered_map<INT64, CDx11GameObject*>::iterator	iter = m_mapUser.find(iKey);

	if (iter == m_mapUser.end())
		return NULL;

	iter->second->AddRef();

	return iter->second;
}

CUserScript * CUserManager::FindUserScript(INT64 iKey)
{
	unordered_map<INT64, CDx11GameObject*>::iterator	iter = m_mapUser.find(iKey);

	if (iter == m_mapUser.end())
		return NULL;

	CUserScript*	pUserScript = (CUserScript*)iter->second->FindComponentFromTypeName("CUserScript");

	return pUserScript;
}
