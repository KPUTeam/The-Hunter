#include "NetworkManager.h"
#include "NetSession.h"
#include "Iocp.h"
#include "ConnectDB.h"

DEFINITION_SINGLE(CNetworkManager)

CNetworkManager::CNetworkManager()
	:m_pIocp(NULL),
	 m_pListen(NULL)
{
	WSADATA	data;
	WSAStartup(MAKEWORD(2, 2), &data);

	if (!LoadDBdata())
		cout << "DB에 있는 테이블 정보를 얻어오지 못하였습니다." << endl;
	else cout << "테이블 정보를 가져왔습니다." << endl;
}


CNetworkManager::~CNetworkManager()
{
	Safe_Delete_Map(m_mapMember);
	SAFE_DELETE(m_pIocp);
	Safe_Delete_Map(m_mapSession);
	WSACleanup();
}

HANDLE CNetworkManager::GetCompletionPort() const
{
	return m_pIocp->GetPort();
}

bool CNetworkManager::DestroySession(SOCKET hSocket)
{
	unordered_map<SOCKET, class CNetSession*>::iterator	iter = m_mapSession.find(hSocket);

	if (iter == m_mapSession.end())
		return false;

	PMEMBER	pMember = iter->second->GetUserInfo();

	if (pMember)
	{
		pMember->bLogin = false;
		pMember->hSocket = 0;
	}

	SAFE_DELETE(iter->second);

	m_mapSession.erase(iter);

	return true;
}

void CNetworkManager::SendAllSession(PPACKET pPacket)
{
	unordered_map<SOCKET, class CNetSession*>::iterator	iter;
	unordered_map<SOCKET, class CNetSession*>::iterator	iterEnd = m_mapSession.end();

	for (iter = m_mapSession.begin(); iter != iterEnd; ++iter)
	{
		if (!iter->second->GetUserInfo())
			continue;

		iter->second->Write(pPacket);
	}
}

bool CNetworkManager::LoadDBdata()
{
	CConnectDB* p_DB = GET_SINGLE(CConnectDB);
	if (!p_DB->DBconnect())
		return false;

	SAFE_DELETE(p_DB);
	
	return true;
}

bool CNetworkManager::Init()
{
	// 서버용 세션을 만들어준다.
	m_pListen = new CNetSession;

	if (!m_pListen->CreateListen())
	{
		SAFE_DELETE(m_pListen);
		return false;
	}

	m_mapSession.insert(make_pair(m_pListen->GetSocket(), m_pListen));

	m_pIocp = new CIocp;

	m_pIocp->CreateIOCP();
	
	return true;
}

void CNetworkManager::Run()
{
	while (true)
	{
		CNetSession*	pSession = m_pListen->Accept();

		m_mapSession.insert(make_pair(pSession->GetSocket(), pSession));
	}
}

bool CNetworkManager::AddMember(char * pID, char * pPass)
{
//	if (FindMember(pID))
//		return false;

	PMEMBER	pMember = new MEMBER;

	strcpy_s(pMember->strID, pID);
	strcpy_s(pMember->strPass, pPass);

	m_mapMember.insert(make_pair(pID, pMember));

	char Query[50] = {};

	sprintf(Query, "Insert into user_info values('%s', '%s')", pID, pPass);

	CConnectDB* pCDB = GET_SINGLE(CConnectDB);
	pCDB->DBconnect();
	if (!pCDB->InputQuery(Query))
		return false;
	//	pCDB->ShowResult();
	DESTROY_SINGLE(CConnectDB);

	return true;
}

PMEMBER CNetworkManager::FindMember(char * pID)
{
	unordered_map<string, PMEMBER>::iterator	iter = m_mapMember.find(pID);

	if (iter == m_mapMember.end())
		return NULL;

	return iter->second;
}
