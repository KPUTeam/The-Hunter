#pragma once

#include "ZEROMultiThreadSync.h"
#include "ZEROCircularQueue.h"

class CZERONetworkSession : public CZEROMultiThreadSync<CZERONetworkSession>
{
public:
	CZERONetworkSession(void);
	virtual ~CZERONetworkSession(void);

private:
	OVERLAPPED_EX		m_AcceptOverlapped;
	OVERLAPPED_EX		m_ReadOverlapped;
	OVERLAPPED_EX		m_WriteOverlapped;

	BYTE				m_ReadBuffer[MAX_BUFFER_LENGTH];

	SOCKET				m_Socket;

	// Udp 관련 변수들
	SOCKADDR_IN			m_UdpRemoteInfo;

	HANDLE				m_ReliableUdpThreadHandle;
	HANDLE				m_ReliableUdpThreadStartupEvent;
	HANDLE				m_ReliableUdpThreadDestroyEvent;
	HANDLE				m_ReliableUdpThreadWakeUpEvent;
	HANDLE				m_ReliableUdpWriteCompleteEvent;

	CZEROCircularQueue	m_ReliableWriteQueue;

	BOOL				m_IsReliableUdpSending;

public:
	// 기본 기능 함수들
	BOOL				Begin(VOID);
	BOOL				End(VOID);

	BOOL				Listen(USHORT port, INT backLog);
	BOOL				Accept(SOCKET listenSocket);
	BOOL				TcpBind(VOID);
	BOOL				UdpBind(USHORT port);

	BOOL				GetLocalIP(WCHAR* pIP);
	USHORT				GetLocalPort(VOID);

	BOOL				InitializeReadForIocp(VOID);
	BOOL				ReadForIocp(BYTE* data, DWORD& dataLength);
	BOOL				ReadForEventSelect(BYTE* data, DWORD& dataLength);

	BOOL				Write(BYTE* data, DWORD dataLength);

	// Udp 관련 함수들
	BOOL				InitializeReadFromForIocp(VOID);
	BOOL				ReadFromForIocp(LPSTR remoteAddress, USHORT& remotePort, BYTE* data, DWORD& dataLength);
	BOOL				ReadFromForEventSelect(LPSTR remoteAddress, USHORT& remotePort, BYTE* data, DWORD& dataLength);
	
	BOOL				WriteTo(LPCSTR remoteAddress, USHORT remotePort, BYTE* data, DWORD dataLength);
	BOOL				WriteTo2(LPCSTR remoteAddress, USHORT remotePort, BYTE* data, DWORD dataLength);

	// Udp 및 기타 함수들
	BOOL				Connect(LPSTR address, USHORT port);

	SOCKET				GetSocket(VOID);

	BOOL				GetRemoteAddressAfterAccept(LPTSTR remoteAddress, USHORT& remotePort); 

	VOID				ReliableUdpThreadCallback(VOID);

	inline VOID			SetUdpWriteCompleteEvent(VOID)
	{
		SetEvent(m_ReliableUdpWriteCompleteEvent);
	}
	


};
