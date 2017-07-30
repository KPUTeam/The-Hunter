#include "Dx11PrototypeManager.h"

DX11_USING

DX11_SINGLE_DEFINITION(CDx11PrototypeManager)

CDx11PrototypeManager::CDx11PrototypeManager()
{
}

CDx11PrototypeManager::~CDx11PrototypeManager()
{
	Safe_Release_Map(m_mapPrototype);
}

bool CDx11PrototypeManager::Init()
{
	return true;
}

bool CDx11PrototypeManager::AddPrototype(const string & strKey,
	CDx11GameObject * pProto)
{
	if (FindPrototype(strKey))
		return false;

	pProto->AddRef();
	m_mapPrototype.insert(make_pair(strKey, pProto));

	return true;
}

CDx11GameObject * CDx11PrototypeManager::CloneObject(const string & strProtoKey)
{
	CDx11GameObject*	pProto = FindPrototype(strProtoKey);

	if (!pProto)
		pProto = CDx11GameObject::Create(strProtoKey, OCT_PROTOTYPE);

	return pProto->Clone();
}

CDx11GameObject * CDx11PrototypeManager::FindPrototype(const string & strKey)
{
	unordered_map<string, class CDx11GameObject*>::iterator	iter = m_mapPrototype.find(strKey);

	if (iter == m_mapPrototype.end())
		return NULL;

	return iter->second;
}
