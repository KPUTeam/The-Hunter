#pragma once
#include "IngameNetwork.h"
#include "ZEROCircularQueue.h"
#include "ZEROMultiThreadSync.h"
#include "GameObject/Dx11GameObject.h"
#include "Scene/Dx11Layer.h"

DX11_USING

class CNetManager
{
public:
	CNetManager();
	~CNetManager();

private:
	static CNetManager*	m_pInst;

public:
	static CNetManager* GetInst()
	{
		if (!m_pInst)
			m_pInst = new CNetManager;

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
	class CUserSession*	m_pGameSVSession;
	SOCKET				m_hSocket;
	SOCKADDR_IN			m_tAddr;
	CIngameNetwork*		m_pEventSelect;
	CZEROCircularQueue	m_InGameQueue;
	list<CDx11GameObject*>	m_LoginObj;

public:
	CIngameNetwork*	GetInGameNetwork();

public:
	void AddLoginObj(CDx11GameObject* pObj);
	bool EmptyLoginUser()
	{
		return m_LoginObj.empty();
	}

	void AddLayer(CDx11Layer* pLayer)
	{
		list<CDx11GameObject*>::iterator	iter;

		for (iter = m_LoginObj.begin(); iter != m_LoginObj.end(); ++iter)
		{
			pLayer->AddObject((*iter));
			SAFE_RELEASE((*iter));
		}

		m_LoginObj.clear();
	}

public:
	SOCKET	GetSocket() const;

public:
	bool Init(char* pIP, short sPort);
	bool Login(const char* pID, const char* pPass);
	bool myMove(const Vec3& vPos);
	bool myRotate(const Vec3& vRot);
	bool myHeadRotate(const Vec3& vRot);
	bool myShoot(const Vec3& vDir);
	bool myDeath();
};

