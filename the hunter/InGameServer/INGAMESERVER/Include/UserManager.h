#pragma once

#include "GameObject/Dx11GameObject.h"

DX11_USING

class CUserManager
{
public:
	CUserManager();
	~CUserManager();

private:
	static CUserManager*	m_pInst;

public:
	static CUserManager*	GetInst()
	{
		if (!m_pInst)	m_pInst = new CUserManager;
		return m_pInst;
	}

	static void DestroyInst()
	{
		if (m_pInst)
		{
			delete m_pInst;
			m_pInst = NULL;
		}
	}

private:
	unordered_map<INT64, CDx11GameObject*> m_mapUser;

public:
	bool Init();
	bool AddUser(INT64 iKey, CDx11GameObject* pObj);
	CDx11GameObject* FindUser(INT64 iKey);
	class CUserScript* FindUserScript(INT64 iKey);
};

