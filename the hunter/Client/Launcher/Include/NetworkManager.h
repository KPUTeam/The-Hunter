#pragma once

#include "LauncherValue.h"
#include "LoginNetwork.h"

#define PORT		9000
#define PACKET_SIZE	1024

enum PACKET_HEADER
{
	PH_NONE,
	PH_JOIN,
	PH_LOGIN,
	PH_LOGOUT,
	PH_LOGINFAILED,
	PH_MSG
};

typedef struct _tagPacket
{
	PACKET_HEADER	eHeader;
	int iSize;
	char strPacket[PACKET_SIZE];

	_tagPacket() :
		eHeader(PH_NONE),
		iSize(0)
	{
		memset(strPacket, 0, PACKET_SIZE);
	}
}PACKET, *PPACKET;

#define ID_SIZE		64
#define PASS_SIZE	64

#if defined(DEBUG) | defined(_DEBUG)
#pragma comment(lib, "ZEROLowLib.lib")
#else
#pragma comment(lib, "ZEROLowlib")
#endif

class CNetworkManager
{
public:
	CNetworkManager();
	~CNetworkManager();

private:
	static CNetworkManager* m_pInst;

public:
	static CNetworkManager*	GetInst()
	{
		if (!m_pInst)
			m_pInst = new CNetworkManager;

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
	SOCKET			m_hSocket;
	SOCKADDR_IN		m_tAddr;
	CLoginNetwork*	m_pEventSelect;

public:
	SOCKET	GetSocket() const;

public:
	bool Init(char* pIP, short sPort, class CMainFrame* pFrame);
};

