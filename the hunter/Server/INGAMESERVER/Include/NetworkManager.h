#pragma once

#include "Network.h"
#include "ServerIOCP.h"
#include "Network\ZERONetworkSession.h"
#include "Gameobject/Dx11GameObject.h"

DX11_USING

class CNetworkManager
{
public:
	CNetworkManager();
	~CNetworkManager();

private:
	static CNetworkManager* m_pInst;

public:
	static CNetworkManager* GetInst()
	{
		if (!m_pInst)	m_pInst = new CNetworkManager;
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
	CZERONetworkSession*	m_pListenSession;
	CServerIOCP*			m_pIOCP;
	class CLogic*					m_pLogic;

private:
	int m_iCurrentTime;
	int m_iSameTime;
	int m_iLastTime;
	INT64	m_iSerialNumber;

private:
	unordered_map<string, PMEMBER>	m_mapMember;
	unordered_map<SOCKET, class CUserSession*> m_mapLoginUser;
	vector<class CUserSession*>		m_vecConnectSession;

public:
	bool Init(short sPort);
	void Run();

public:
	CZERONetworkSession*	GetListenSession() const;
	size_t GetConnectCount() const;

	bool AddLoginUser(class CUserSession* pUser, char* pID, char* pPass);
	bool DisconnectUser(class CUserSession* pUser);
	CUserSession* FindUser(SOCKET hSocket);
	INT64	CreateSerialNumber(int iObjType);
	PMEMBER	FindMember(const string& strID);

public:
	void SendConnectAllUser(class CUserSession* pUser, CDx11GameObject* pObj);
	void SendMoveAllUser(class CUserSession* pUser, Vec3 vPos);
	void SendRotAllUser(class CUserSession* pUser, Vec3 vRot);
	void SendHeadRotAllUSer(class CUserSession* pUser, Vec3 vRot);
	void SendGameStart();
	void SendShootAllUser(class CUserSession* pUser, Vec3 vDir);
};

