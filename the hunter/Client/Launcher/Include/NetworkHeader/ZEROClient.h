#pragma once
#include "zeroclientsession.h"

//#define READ_PACKET(PROTOCOL)\
//	BYTE WriteBuffer[MAX_BUFFER_LENGTH] = {0,};\
//	S_##PROTOCOL Data;\
//	READ_##PROTOCOL(Packet, Data);


class CZEROClient :
	public CZEROClientSession
{
public:
	BOOL BeginTcp(LPSTR remoteAddress, USHORT remotePort);

	VOID End(VOID);

public:
	VOID OnIoConnected()
	{
		_tprintf(_T("접속해쪙 >_< \n"));
	}

	VOID OnIoDisconnected()
	{
		_tprintf(_T("접속끝나쪙 >_< \n"));
	}

public:
	CZEROClient(void);
	virtual ~CZEROClient(void);
};

/*			설명서			*/
/*
1. 시작

CZEROClient* pClientSession = new CZEROClient;

if(!pClientSession->BeginTcp("192.168.0.186", DEFAULT_PORT))
	return 0;



2. 종료

pClientSession->End();

delete pClientSession;



3. 패킷 받기

	
		DWORD dwProtocol = 0;
		DWORD dwPacketLength = 0;
		BYTE Packet[MAX_BUFFER_LENGTH] = {0,};

		pClientSession->ReadPacket(dwProtocol, Packet, dwPacketLength);

		if(dwProtocol == PT_CHAT_SERVER_TO_CLIENT)
		{
			READ_PACKET(PT_CHAT_SERVER_TO_CLIENT);
			
			Data 구조체에 패킷 내용 담겨져 있음.
		}
	


4. 패킷 보내기

BYTE WriteBuffer[MAX_BUFFER_LENGTH] = {0,};

							   프로토콜                    WRITE_프로토콜                변수1,    변수2, ...
pClientSession->WritePacket(PT_JOIN_QUERY, WriteBuffer, WRITE_PT_JOIN_QUERY(WriteBuffer, tempID, tempPass));



5. 헤더추가

#define DEFAULT_PORT 6512

#include "Include.h"
#include "ZEROMultiThreadSync.h"
#include "ZEROMemoryPool.h"
#include "ZEROCircularQueue.h"
#include "ZERONetworkSession.h"
#include "ZEROPacketSession.h"
#include "ZEROEventSelect.h"
#include "ZEROStream.h"
#include "ZEROClientSession.h"
#include "ZEROClient.h"

#include "Protocol/Protocol.h"
#include "Protocol/PT_Structure.h"
#include "Protocol/PT_ReadPacket.h"
#include "Protocol/PT_WritePacket.h"


끝!
*/