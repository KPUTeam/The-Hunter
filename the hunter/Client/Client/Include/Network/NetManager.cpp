#include "NetManager.h"
#include "UserSession.h"
#include "ZEROStream.h"
#include "../Client.h"

CNetManager* CNetManager::m_pInst = NULL;

CNetManager::CNetManager() :
	m_pEventSelect(NULL),
	m_hSocket(NULL),
	m_pGameSVSession(NULL)
{
	WSADATA	wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
}


CNetManager::~CNetManager()
{
	m_pGameSVSession->End();
	SAFE_DELETE(m_pGameSVSession);

	delete m_pEventSelect;
	closesocket(m_hSocket);
	WSACleanup();
}

CIngameNetwork * CNetManager::GetInGameNetwork()
{
	return m_pEventSelect;
}

void CNetManager::AddLoginObj(CDx11GameObject * pObj)
{
	m_LoginObj.push_back(pObj);
}

SOCKET CNetManager::GetSocket() const
{
	return m_hSocket;
}

bool CNetManager::Init(char * pIP, short sPort)
{
	m_pGameSVSession = new CUserSession;
	m_pGameSVSession->Begin();
	m_pGameSVSession->TcpBind();

	m_pEventSelect = new CIngameNetwork;
	m_pEventSelect->Begin(m_pGameSVSession->GetSocket());
	m_pEventSelect->SetSession(m_pGameSVSession);

	m_pGameSVSession->Connect(pIP, sPort);

	return true;
}

bool CNetManager::Login(const char * pID, const char * pPass)
{
	BYTE	strPacket[PACKET_SIZE] = {};
	CZEROStreamSP	stream;

	stream->SetBuffer(strPacket);

	stream->WriteInt32(strlen(pID));
	stream->WriteBytes((BYTE*)pID, strlen(pID));
	stream->WriteInt32(strlen(pPass));
	stream->WriteBytes((BYTE*)pPass, strlen(pPass));

	m_pGameSVSession->WritePacket(PH_LOGIN, strPacket, stream->GetLength());

	return true;
}

bool CNetManager::Move(const Vec3 & vPos)
{
	return true;
}
