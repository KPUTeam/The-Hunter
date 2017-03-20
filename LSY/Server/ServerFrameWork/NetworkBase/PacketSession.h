#pragma once
#include "NetworkSession.h"

typedef struct _READ_PACKET_INFO
{
	CHAR	RemoteAddress[14]; // 데이터를 받은 주소
	USHORT	RemotePort;		   // 데이터를 받은 포트
	DWORD	PacketNumber;	   // 받은 패킷 번호
} READ_PACKET_INFO;

class CPacketSession : public CNetworkSession
{
public:
	CPacketSession();
	virtual ~CPacketSession();

private:
	BYTE						mPacketBuffer[MAX_BUFFER_LENGTH * 3]; // NetworkSession 클래스에서 받은 데이터를 잠시 패킷 확인을 위해서 저장하는 버퍼
	INT							mRemainLength;
	DWORD						mCurrentPacketNumber;
	DWORD						mLastReadPacketNumber;
	CCircularQueue				WriteQueue;

	vector<READ_PACKET_INFO>	mLastReadPacketInfoVectorForUDP;

public:
	BOOL	Begin();
	BOOL	End();

	BOOL	ReadPacketForIocp(DWORD readLength);
	BOOL	ReadPacketForEventSelect();

	BOOL	ReadFromPacketForIocp(LPSTR remoteAddress, USHORT& remotePort, DWORD readLength);
	BOOL	ReadFromPacketForEventSelect(LPSTR remoteAddress, USHORT& remotePort);

	BOOL	WritePacket(DWORD protocol, const BYTE* packet, DWORD packetLength);
	BOOL	WriteToPacket(LPCSTR remoteAddress, USHORT remotePort, DWORD protocol, const BYTE* packet, DWORD packetLength);
	BOOL	WriteComplete();

	BOOL	ResetUDP();

	BOOL	GetPacket(DWORD& protocol, BYTE* packet, DWORD& packetLength);
	BOOL	GetPacket(LPSTR remoteAddress, USHORT remotePort, DWORD& protocol, BYTE* packet, DWORD& packetLength);
};

