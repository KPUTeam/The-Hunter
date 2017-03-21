#pragma once
#include "NetworkSession.h"

typedef struct _READ_PACKET_INFO
{
	CHAR	RemoteAddress[14]; // �����͸� ���� �ּ�
	USHORT	RemotePort;		   // �����͸� ���� ��Ʈ
	DWORD	PacketNumber;	   // ���� ��Ŷ ��ȣ
} READ_PACKET_INFO;

class CPacketSession : public CNetworkSession
{
public:
	CPacketSession();
	virtual ~CPacketSession();

private:
	BYTE						mPacketBuffer[MAX_BUFFER_LENGTH * 3]; // NetworkSession Ŭ�������� ���� �����͸� ��� ��Ŷ Ȯ���� ���ؼ� �����ϴ� ����
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

