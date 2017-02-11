#pragma once
#include "../Dx11Object.h"

DX11_BEGIN

class DX11_DLL CDx11GameObjectManager :
	public CDx11Object
{
	DX11_DECLARE_SINGLE(CDx11GameObjectManager)

private:
	unordered_map<string, class CDx11GameObject*>	m_mapPrototype;

public:
	bool Init();
	bool AddPrototype(class CDx11GameObject* pPrototype,
		const string& strKey);
	CDx11GameObject* CreateObject(const string& strKey);

private:
	CDx11GameObject* FindPrototype(const string& strKey);
};

DX11_END