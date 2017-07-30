#include "stdafx.h"
#include "NetworkManager.h"

CNetworkManager* CNetworkManager::m_pInst = NULL;

CNetworkManager::CNetworkManager() :
	m_pEventSelect(NULL),
	m_hSocket(NULL)
{
#ifdef _DEBUG
	SetCurrentDirectory("../../Client/Bin_Debug/");
#else
	SetCurrentDirectory("../../Client/Bin");
#endif // _DEBUG

	WSADATA	wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
}


CNetworkManager::~CNetworkManager()
{
	delete m_pEventSelect;
	closesocket(m_hSocket);
	WSACleanup();
}

SOCKET CNetworkManager::GetSocket() const
{
	return m_hSocket;
}

bool CNetworkManager::Init(char * pIP, short sPort, CMainFrame * pFrame)
{
	m_hSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	memset(&m_tAddr, 0, sizeof(m_tAddr));

	m_tAddr.sin_family = AF_INET;
	m_tAddr.sin_port = htons(sPort);
	m_tAddr.sin_addr.s_addr = inet_addr(pIP);

	connect(m_hSocket, (SOCKADDR*)&m_tAddr, sizeof(m_tAddr));

	m_pEventSelect = new CLoginNetwork;
	m_pEventSelect->SetFrame(pFrame);
	m_pEventSelect->Begin(m_hSocket);

	return true;
}
