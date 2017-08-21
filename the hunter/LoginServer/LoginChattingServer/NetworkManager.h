#pragma once

#include "Global.h"

class CNetworkManager
{
	DECLARE_SINGLE(CNetworkManager)

private:
	class CConnectDB*	m_pCDB;

private:
	unordered_map<SOCKET, class CNetSession*>	m_mapSession;
	unordered_map<string, PMEMBER>	m_mapMember;
	class CNetSession*	m_pListen;
	class CIocp*		m_pIocp;

public:
	HANDLE	GetCompletionPort()	const;
	bool DestroySession(SOCKET hSocket);
	void SendAllSession(PPACKET pPacket);

public:
	bool Init();
	void Run();
	bool AddMember(char* pID, char* pPass);
	PMEMBER	FindMember(char* pID);
};

