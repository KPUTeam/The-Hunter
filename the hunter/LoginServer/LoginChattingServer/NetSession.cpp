#include "NetSession.h"
#include "NetworkManager.h"

CNetSession::CNetSession()
{
	m_pUserInfo = NULL;
	m_hSocket = NULL;
	memset(&m_tAddr, 0, sizeof(m_tAddr));
}

CNetSession::~CNetSession()
{
	if (m_hSocket)
	{
		cout << m_hSocket << " Client Close" << endl;
		closesocket(m_hSocket);
	}

}

SOCKET CNetSession::GetSocket() const
{
	return m_hSocket;
}

PPACKET CNetSession::GetReadPacket()
{
	return &m_tReadPacket;
}

PPACKET CNetSession::GetWritePacket()
{
	return &m_tWritePacket;
}

PMEMBER CNetSession::GetUserInfo() const
{
	return m_pUserInfo;
}



bool CNetSession::CreateListen()
{
	m_hSocket = WSASocket(PF_INET, SOCK_STREAM, IPPROTO_TCP,
		NULL, 0, WSA_FLAG_OVERLAPPED);
	// Non-Blocking 모드로 만들어준다.
	//u_long		iMode = 1;
	//ioctlsocket(m_hSocket, FIONBIO, &iMode);

	m_tAddr.sin_family = AF_INET;
	m_tAddr.sin_port = htons(PORT);
	m_tAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	bind(m_hSocket, (SOCKADDR*)&m_tAddr, sizeof(m_tAddr));

	listen(m_hSocket, 5);

	return true;
}

CNetSession * CNetSession::Accept()
{
	SOCKADDR_IN		tAddr = {};
	int	iLength = sizeof(tAddr);

	SOCKET		hSocket;
	hSocket = accept(m_hSocket, (SOCKADDR*)&tAddr, &iLength);

	cout << "IP주소: " << inet_ntoa(tAddr.sin_addr) << ", ";
	cout << "포트번호: " << ntohs(tAddr.sin_port) << " 에서 접속하였습니다." << endl;
	
	CNetSession*	pSession = new CNetSession;

	pSession->m_hSocket = hSocket;
	memcpy(&pSession->m_tAddr, &tAddr, sizeof(tAddr));

	// 소켓과 IOCP를 연결한다.
	HANDLE		hComPort = GET_SINGLE(CNetworkManager)->GetCompletionPort();
	CreateIoCompletionPort((HANDLE)hSocket, hComPort, (DWORD)pSession, 0);

	pSession->Read();

	return pSession;
}

void CNetSession::Read()
{
	memset(&m_tReadBuffer.tOverlapped, 0, sizeof(OVERLAPPED));
	m_tReadBuffer.tWsaBuf.len = PACKET_SIZE + 8;
	m_tReadBuffer.tWsaBuf.buf = (char*)&m_tReadPacket;
	m_tReadBuffer.eMode = TM_READ;
	m_dwRecvBytes = 0;
	m_dwReadFlag = 0;

	WSARecv(m_hSocket, &m_tReadBuffer.tWsaBuf, 1, &m_dwRecvBytes,
		&m_dwReadFlag, &m_tReadBuffer.tOverlapped, NULL);
}

void CNetSession::Write()
{
	memset(&m_tWriteBuffer.tOverlapped, 0, sizeof(OVERLAPPED));
	m_tWriteBuffer.tWsaBuf.len = m_tWritePacket.iSize + 8;
	m_tWriteBuffer.tWsaBuf.buf = (char*)&m_tWritePacket;
	m_tWriteBuffer.eMode = TM_WRITE;
	m_dwWriteBytes = 0;
	m_dwWriteFlag = 0;
}

void CNetSession::Write(PPACKET pPacket)
{
	memcpy(&m_tWritePacket, pPacket, pPacket->iSize + 8);

	memset(&m_tWriteBuffer.tOverlapped, 0, sizeof(OVERLAPPED));
	m_tWriteBuffer.tWsaBuf.len = m_tWritePacket.iSize + 8;
	m_tWriteBuffer.tWsaBuf.buf = (char*)&m_tWritePacket;
	m_tWriteBuffer.eMode = TM_WRITE;
	m_dwWriteBytes = 0;
	m_dwWriteFlag = 0;

	WSASend(m_hSocket, &m_tWriteBuffer.tWsaBuf, 1, &m_dwWriteBytes,
		m_dwWriteFlag, &m_tWriteBuffer.tOverlapped, NULL);
}

void CNetSession::Login(PMEMBER pMember)
{
	m_pUserInfo = pMember;
}
