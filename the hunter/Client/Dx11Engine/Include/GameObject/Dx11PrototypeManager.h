#pragma once

#include "Dx11GameObject.h"

DX11_BEGIN

class DX11_DLL CDx11PrototypeManager :
	public CDx11Object
{
	DX11_SINGLE_DECLARE(CDx11PrototypeManager)

private:
	unordered_map<string, class CDx11GameObject*>	m_mapPrototype;

private:
	CDx11GameObject* FindPrototype(const string& strKey);

public:
	bool Init();
	bool AddPrototype(const string& strKey, CDx11GameObject* pProto);
	template <typename T>
	CDx11Component* AddComponent(const string& strKey,
		const string& strTag = "Component")
	{
		CDx11GameObject*	pProto = FindPrototype(strKey);

		if (!pProto)
			return NULL;

		CDx11Component* pCom = pProto->AddComponent<T>(strTag);

		return pCom;
	}

	CDx11GameObject* CloneObject(const string& strProtoKey);
};

DX11_END
