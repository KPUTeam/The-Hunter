#pragma once

#include "ZERONetworkSession.h"

typedef struct _READ_PACKET_INFO
{
	CHAR	RemoteAddress[14];
	USHORT	RemotePort;
	DWORD	PacketNumber;
} READ_PACKET_INFO;

class CZEROPacketSession : public CZERONetworkSession
{
private:
	BYTE								m_PacketBuffer[MAX_BUFFER_LENGTH * 3];
	INT									m_RemainLength;
	DWORD								m_CurrentPacketNumber;
	
	DWORD								m_LastReadPacketNumber;

	CZEROCircularQueue					m_WriteQueue;

	//std::map<std::string, DWORD>		mLastReadPacketInfoMapForUdp;
	vector<READ_PACKET_INFO>			m_LastReadPacketInfoVectorForUdp;

public:
	CZEROPacketSession(void);
	virtual ~CZEROPacketSession(void);

public:
	BOOL		Begin(VOID);
	BOOL		End(VOID);

	BOOL		ReadPacketForIocp(DWORD readLength);
	BOOL		ReadPacketForEventSelect(VOID);

	BOOL		ReadFromPacketForIocp(LPSTR remoteAddress, USHORT& remotePort, DWORD readLength);
	BOOL		ReadFromPacketForEventSelect(LPSTR remoteAddress, USHORT& remotePort);

	BOOL		WritePacket(DWORD protocol, const BYTE* packet, DWORD packetLength);
	BOOL		WriteToPacket(LPCSTR remoteAddress, USHORT remotePort, DWORD protocol, const BYTE* packet, DWORD packetLength);
	BOOL		WriteComplete(VOID);

	BOOL		ResetUdp(VOID);

	BOOL		GetPacket(DWORD& protocol, BYTE* packet, DWORD& packetLength);
	BOOL		GetPacket(LPSTR remoteAddress, USHORT remotePort, DWORD& protocol, BYTE* packet, DWORD& packetLength);
};
