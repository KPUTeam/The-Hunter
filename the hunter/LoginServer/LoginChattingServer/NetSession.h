#pragma once

#include "Global.h"

class CNetSession
{
public:
	CNetSession();
	~CNetSession();

private:
	SOCKET				m_hSocket;
	SOCKADDR_IN			m_tAddr;

	BUFFER				m_tReadBuffer;
	BUFFER				m_tWriteBuffer;

	PACKET				m_tReadPacket;
	PACKET				m_tWritePacket;

	DWORD				m_dwRecvBytes;
	DWORD				m_dwReadFlag;

	DWORD				m_dwWriteBytes;
	DWORD				m_dwWriteFlag;

	PMEMBER				m_pUserInfo;

public:
	SOCKET GetSocket()	const;
	PPACKET GetReadPacket();
	PPACKET GetWritePacket();
	PMEMBER	GetUserInfo()	const;

public:
	bool CreateListen();
	CNetSession* Accept();
	void Read();
	void Write();
	void Write(PPACKET pPacket);
	void Login(PMEMBER pMember);
};

