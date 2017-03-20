#pragma once
#include "Include.h"
#include "../Basic Library/CircleQueue/CircularQueue.h"
#include "../System Library/ThreadSync/MultiThreadSync.h"


class CNetworkSession : public CMultiThreadSync<CNetworkSession>
{
public:
	CNetworkSession();
	virtual ~CNetworkSession();

private:
	OVERLAPPED_EX	mAcceptOverlapped;
	OVERLAPPED_EX	mReadOverlapped;
	OVERLAPPED_EX	mWriteOverlapped;

	BYTE			mReadBuffer[MAX_BUFFER_LENGTH];
	SOCKADDR_IN		mUDPRemoteInfo;

	SOCKET			mSocket;

	HANDLE			mReliableUDPThreadHandle;
	HANDLE			mReliableUDPThreadStartupEvent;
	HANDLE			mReliableUDPThreadDestroyEvent;
	HANDLE			mReliableUDPThreadWakeUpEvent;
	HANDLE			mReliableUDPWriteCompleteEvent;

	CCircularQueue	mReliableWriteQueue;

	BOOL			mIsReliableUDPSending;

public:
	BOOL			Begin();
	BOOL			End();

	BOOL			Listen(USHORT port, INT backLog);
	BOOL			Accept(SOCKET listenSocket);
	BOOL			TCPBind();
	BOOL			UDPBind(USHORT port);

	BOOL			GetLocalIP(WCHAR* pIP);
	USHORT			GetLocalPort();

	BOOL			InitializeReadForIocp();
	BOOL			ReadForIocp(BYTE* data, DWORD& dataLength);
	BOOL			ReadForEventSelect(BYTE* data, DWORD& dataLength);

	BOOL			Write(BYTE* data, DWORD dataLength);

	BOOL			InitializeReadFromForIocp();
	BOOL			ReadFromForIocp(LPSTR remoteAddress, USHORT& remotePort, BYTE* data, DWORD& dataLength);
	BOOL			ReadFromForEventSelect(LPSTR remoteAddress, USHORT& remotePort, BYTE* data, DWORD& dataLength);

	BOOL			WriteTo(LPCSTR remoteAddress, USHORT remotePort, BYTE* data, DWORD dataLength);
	BOOL			WriteTo2(LPSTR remoteAddress, USHORT remotePort, BYTE* data, DWORD dataLength);

	BOOL			Connect(LPSTR address, USHORT port);

	SOCKET			GetSocket();

	BOOL			GetRemoteAddressAfterAceept(LPTSTR remoteAddress, USHORT& remotePort);

	void			ReliableUDPThreadCallback();

	inline void		SetUDPWriteCompleteEvent()
	{
		SetEvent(mReliableUDPWriteCompleteEvent);
	}
};


