#include "Dx11GameObjectManager.h"
#include "Dx11GameObject.h"

DX11_USING

DX11_DEFINITION_SINGLE(CDx11GameObjectManager)

CDx11GameObjectManager::CDx11GameObjectManager()
{
	SetTypeName<CDx11GameObjectManager>();
}

CDx11GameObjectManager::~CDx11GameObjectManager()
{
	Safe_Release_Map(m_mapPrototype);
}

bool CDx11GameObjectManager::Init()
{
	return true;
}

bool CDx11GameObjectManager::AddPrototype(CDx11GameObject * pPrototype,
	const string& strKey)
{
	if (FindPrototype(strKey))
		return false;

	m_mapPrototype.insert(make_pair(strKey, pPrototype));
	pPrototype->AddRef();

	return true;
}

CDx11GameObject * CDx11GameObjectManager::CreateObject(const string & strKey)
{
	CDx11GameObject*	pPrototype = FindPrototype(strKey);

	if (!pPrototype)
		pPrototype = CDx11GameObject::Create(strKey, OT_PROTOTYPE);

	return pPrototype->Clone();
}

CDx11GameObject * CDx11GameObjectManager::FindPrototype(const string & strKey)
{
	unordered_map<string, class CDx11GameObject*>::iterator	iter = m_mapPrototype.find(strKey);

	if (iter == m_mapPrototype.end())
		return NULL;

	return iter->second;
}
