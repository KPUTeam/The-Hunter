#include "NetworkSession.h"
#include "../System Library/ThreadSync/CriticalSection.h"
#include "../System Library/ThreadSync/MultiThreadSync.h"
#include "../Basic Library/CircleQueue/CircularQueue.h"

DWORD WINAPI ReliableUDPThreadCallback(LPVOID parameter)
{
	CNetworkSession* Owner = (CNetworkSession*)parameter;
	Owner->ReliableUDPThreadCallback();

	return 0;
}


CNetworkSession::CNetworkSession()
{
	memset(&mAcceptOverlapped, 0, sizeof(mAcceptOverlapped));
	memset(&mReadOverlapped, 0, sizeof(mReadOverlapped));
	memset(&mWriteOverlapped, 0, sizeof(mWriteOverlapped));

	memset(mReadBuffer, 0, sizeof(mReadBuffer));
	memset(&mUDPRemoteInfo, 0, sizeof(mUDPRemoteInfo));

	mSocket							= NULL;
	mReliableUDPThreadHandle		= NULL;	// ���� ��Ŷ�� ���� ������ ��� �������ִ� ������ �ڵ��̴�.
	mReliableUDPThreadStartupEvent  = NULL; // ReliableUDPThread�� ������ �˸��� �̺�Ʈ
	mReliableUDPThreadDestroyEvent  = NULL; // ReliableUDPThread�� ���Ḧ �˸��� �̺�Ʈ
	mReliableUDPThreadWakeUpEvent   = NULL; // ReliableUDPThread�� ���ﶧ ����ϴ� �̺�Ʈ
	mReliableUDPWriteCompleteEvent  = NULL; // ��밡 ��Ŷ�� �޾� �� �̻� ���� �ʿ䰡 ���� �� ����ϴ� �̺�Ʈ

	mIsReliableUDPSending			= FALSE; // ���� ������ �ִ� Reliable Data�� �ִ����� Ȯ���Ѵ�.

	mAcceptOverlapped.IoType		= IO_ACCEPT;
	mReadOverlapped.IoType			= IO_READ;
	mWriteOverlapped.IoType			= IO_WRITE;

	mAcceptOverlapped.Object		= this;
	mReadOverlapped.Object			= this;
	mWriteOverlapped.Object			= this;
}


CNetworkSession::~CNetworkSession()
{
}

BOOL CNetworkSession::Begin()
{
	// ������ �츮�� ����ϰ� �� �⺻���� SOCKET �ڵ��
	// �����带 �����ϴ� �ڵ� �� ���� �̺�Ʈ�� �ʱ�ȭ��

	CThreadSync	Sync; // ���� ������ ����ȭ

	if (mSocket)
		return FALSE;
	
	// ���� �ʱ�ȭ
	memset(mReadBuffer, 0, sizeof(mReadBuffer));
	// UDP�� ����� �� �����͸� ���� ��� ������ �����ϱ� ���� ����
	memset(&mUDPRemoteInfo, 0, sizeof(mUDPRemoteInfo));

	mSocket							= NULL; // ������ ��� ����ϰ� �� ���� �ڵ�
	mReliableUDPThreadHandle		= NULL; // ReliableUDP ���� ������ �ص�
	mReliableUDPThreadStartupEvent  = NULL; // �����尡 ���۵��� �˷��ִ� �̺�Ʈ
	mReliableUDPThreadDestroyEvent  = NULL; // �����尡 ����Ǿ����� �˷��ִ� �̺�Ʈ
	mReliableUDPThreadWakeUpEvent   = NULL;
	mReliableUDPWriteCompleteEvent  = NULL; // ������ �� �Ѱ��� ������ ������ �Ϸ�Ǿ����� 
											// �߻��ϴ� �̺�Ʈ
	mIsReliableUDPSending			= FALSE;// ���� �����͸� ���������� Ȯ���ϴ� FLAG ����

	return TRUE;
}

BOOL CNetworkSession::End()
{
	CThreadSync	Sync; // ���� ������ ����ȭ

	if (!mSocket)
		return FALSE;

	shutdown(mSocket, SD_BOTH); // �ش� ������ READ/WRITE�� ����
	closesocket(mSocket);		// �ش� ������ �ݴ´�.

	mSocket = NULL;

	if (mReliableUDPThreadHandle)
	{
		// �����带 �����ϴ� �̺�Ʈ�� �߻���Ų��.
		SetEvent(mReliableUDPThreadDestroyEvent);
		// �����尡 ����ɶ����� ���Ѵ�� ����Ѵ�.
		WaitForSingleObject(mReliableUDPThreadHandle, INFINITE);
		CloseHandle(mReliableUDPThreadHandle);
	}

	if (mReliableUDPThreadDestroyEvent)
		CloseHandle(mReliableUDPThreadDestroyEvent);

	if (mReliableUDPThreadStartupEvent)
		CloseHandle(mReliableUDPThreadStartupEvent);

	if (mReliableUDPThreadWakeUpEvent)
		CloseHandle(mReliableUDPThreadWakeUpEvent);

	if (mReliableUDPWriteCompleteEvent)
		CloseHandle(mReliableUDPWriteCompleteEvent);

	// Reliable UDP�� ���� �����͸� �����ϴ� WriteQueue�� ����.
	mReliableWriteQueue.End();

	return TRUE;
}

BOOL CNetworkSession::Listen(USHORT port, INT backLog)
{
	CThreadSync	Sync;

	if (port <= 0 || backLog <= 0)
		return FALSE;

	if (!mSocket)
		return FALSE;

	SOCKADDR_IN	ListenSocketInfo;

	ListenSocketInfo.sin_family           = AF_INET;
	ListenSocketInfo.sin_port             = htons(port);
	ListenSocketInfo.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(mSocket, (sockaddr*)&ListenSocketInfo, sizeof(SOCKADDR_IN)) == SOCKET_ERROR)
	{
		End();
		return FALSE;
	}

	if (listen(mSocket, backLog) == SOCKET_ERROR)
	{
		End();
		return FALSE;
	}

	// LINGET�� ������ Close�� �� ������ ��ġ�� �Լ���
	// ���� ������ �ְų� �޴� �����Ͱ� ���� ��� 
	// �� �����͸� ��� ���� �� ������ Clost�ϰ� �ϴ� �ɼ��̴�.
	// ������ setsockopt �Լ��� ������ �ϰ�
	// l_onfoo ���� 1�� ������ָ� �����ϴµ�
	// ������ �����ϰ� �����ϱ� ���ؼ� �� �ʿ��� �ɼ�������
	// ������ �޴� �����Ͱ� ���� ��� ���ῡ ���� �ð��� �ɸ�....
	LINGER	Linger;
	Linger.l_onoff  = 1;
	Linger.l_linger = 0;

	if (setsockopt(mSocket, SOL_SOCKET, SO_LINGER, (char*)&Linger, sizeof(LINGER)) == SOCKET_ERROR)
	{
		End();
		return FALSE;
	}

	return TRUE;
}

BOOL CNetworkSession::Accept(SOCKET listenSocket)
{
	CThreadSync Sync;

	if (!listenSocket) // listen�� ���� ������� ������ ������� �ʴ´�.
		return FALSE;

	if (mSocket)
		return FALSE;

	// Accept�� ����� ������ �̸� �����Ѵ�.
	mSocket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);

	if (mSocket == INVALID_SOCKET)
	{
		End();
		return FALSE;
	}

//	BOOL NoDelay = TRUE;
//	setsockopt(mSocket, IPPROTO_TCP, TCP_NODELAY, (const char FAR*)&NoDelay, sizeof(NoDelay));

	if (!AcceptEx(listenSocket,
		mSocket,
		mReadBuffer,
		0,
		sizeof(sockaddr_in) + 16,
		sizeof(sockaddr_in) + 16,
		NULL,
		&mAcceptOverlapped.Overlapped))
	{
		if (WSAGetLastError() != ERROR_IO_PENDING && WSAGetLastError() != WSAEWOULDBLOCK)
		{
			End();
			return FALSE;
		}
	}

	// Accept�Լ� ��ſ� AcceptEx �Լ��� ����� ����
	// �Ϲ������� while���� ����� WSAAccept�� �ϳ��� Accept�� �Ϸ�Ǳ� ����
	// �ٸ� ���ῡ ���� Accept�� �� ���� ����. �׷��� 
	// ���ڱ� ���� ������ ���� ��� ������ ���е� ���� �ִ�.
	// AcceptEx�� WsaAccept�� �ٸ��� while������ ó���ϴ� ���� �ƴ϶�
	// AcceptEx�� �ϳ��� ������ �̸� �����ΰ�
	// Accept ��û�� ���� �� ������ ������ Ȱ���ϴ� ����̴�.
	// AcceptEx�� Accept�� �����鼭 �� �� �����͸� ���� �� �ִٴ� ���̴�.
	// ��, WSAAccept�� ������ ����ϰ�, �ʱ� �ޱ⸦ �� �� �õ��ϴ� ������
	// ���¸� �������ϴ� �Ϲ����� ����� �޸� AcceptEx�� �� ���� ������ �� �ִ�.
	
	return TRUE;;
}

// TCP�� ����� ��� ȣ��
BOOL CNetworkSession::TCPBind()
{
	CThreadSync	Sync;

	// mSocket�� NULL�� �ƴ� ��� �̹� ������ �����Ǿ� Bind �Ǿ��ִ� �����̹Ƿ� �ٽ� TCPBind�Լ��� �� �� ����.
	if (mSocket)
		return FALSE;

	mSocket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);

	if (mSocket == INVALID_SOCKET)
		return FALSE;

//	BOOL NoDelay = TRUE;
//	setsockopt(mSocket, IPPROTO_TCP, TCP_NODELAY, (const char FAR*)&NoDelay, sizeof(NoDelay));
	return 0;
}

BOOL CNetworkSession::UDPBind(USHORT port)
{
	CThreadSync	Sync;

	if (mSocket)
		return FALSE;

	SOCKADDR_IN	RemoteAddressInfo;

	RemoteAddressInfo.sin_family			= AF_INET;
	RemoteAddressInfo.sin_port				= htons(port);
	RemoteAddressInfo.sin_addr.S_un.S_addr = htonl(INADDR_ANY);

	mSocket = WSASocket(AF_INET, SOCK_DGRAM, IPPROTO_UDP, NULL, 0, WSA_FLAG_OVERLAPPED);

	if (mSocket == INVALID_SOCKET)
		return FALSE;

	if (bind(mSocket, (sockaddr*)&RemoteAddressInfo, sizeof(SOCKADDR_IN)))
	{
		End();

		return FALSE;
	}

	// mReliableUDPThreadDestroyEvent
	mReliableUDPThreadDestroyEvent = CreateEvent(0, FALSE, FALSE, 0);
	if (mReliableUDPThreadDestroyEvent == NULL)
	{
		End();

		return FALSE;
	}

	// mReliableUDPThreadStartupEvent
	mReliableUDPThreadStartupEvent = CreateEvent(0, FALSE, FALSE, 0);
	if (mReliableUDPThreadStartupEvent == NULL)
	{
		End();

		return FALSE;
	}

	// mReliableUDPThreadWakeUpEvent
	mReliableUDPThreadWakeUpEvent = CreateEvent(0, FALSE, FALSE, 0);
	if (mReliableUDPThreadWakeUpEvent == NULL)
	{
		End();

		return FALSE;
	}

	// mReliableUDPWriteCompleteEvent
	mReliableUDPWriteCompleteEvent = CreateEvent(0, FALSE, FALSE, 0);
	if (mReliableUDPWriteCompleteEvent == NULL)
	{
		End();

		return FALSE;
	}

	if (!mReliableWriteQueue.Begin())
	{
		End();

		return FALSE;
	}

	DWORD ReliableUDPThreadID = 0;
	mReliableUDPThreadHandle = CreateThread(NULL, 0, ::ReliableUDPThreadCallback, this, 0, &ReliableUDPThreadID);

	WaitForSingleObject(mReliableUDPThreadStartupEvent, INFINITE);

	return TRUE;

	// UDP�� ����� ������ �������ְ� ������ ��Ʈ�� ���ε����ִ� ������ �Ѵ�.
	// �׸��� ���ο����� ReliableUDPThread�� ����ϱ� ���� �̺�Ʈ���
	// ReliableUDPThread �ڽ��� �����Ѵ�.
	// ReliableUDPThread�� ���� �Ϸ�ɶ����� �Լ��� ����Ų��.
	// �̷��� ����Ű�� ������ �����尡 �����ǰ� ReliableUDP�� ������ �غ� �� ���¿���
	// �Լ��� �Ϸ��Ű�� �����̴�.
	// CircularQueue�� ReliableUDPThread�� ��� �����͸� ������ ���� ��
	// �������� ������ �� �����͸� �����ϰ� �ִ� ������ �ϰ� �ȴ�.
}

BOOL CNetworkSession::GetLocalIP(WCHAR * pIP)
{
	CThreadSync	Sync;

	if (!mSocket)
		return FALSE;

	CHAR	Name[256] = { 0, };
	gethostname(Name, sizeof(Name));

	PHOSTENT	host = gethostbyname(Name);
	if (host)
	{
		if (MultiByteToWideChar(CP_ACP, 0, inet_ntoa(*(struct in_addr*)*host->h_addr_list), -1, pIP, 32) > 0)
			return TRUE;
	}

	return FALSE;
}

USHORT CNetworkSession::GetLocalPort()
{
	CThreadSync	Sync;

	if (!mSocket)
		return 0;

	SOCKADDR_IN	Addr;
	ZeroMemory(&Addr, sizeof(Addr));

	INT AddrLength = sizeof(Addr);
	if (getsockname(mSocket, (sockaddr*)&Addr, &AddrLength) != SOCKET_ERROR)
		return ntohs(Addr.sin_port);

	return 0;
}

BOOL CNetworkSession::InitializeReadForIocp() // IOCP�� ����� �� ����ϴ� �ʱ� �ޱ�
{
	CThreadSync	Sync;

	if (!mSocket)
		return FALSE;

	WSABUF	WsaBuf;
	DWORD	ReadBytes = 0;
	DWORD	ReadFlag  = 0;

	WsaBuf.buf = (CHAR*)mReadBuffer;
	WsaBuf.len = MAX_BUFFER_LENGTH;

	INT	ReturnVaue = WSARecv(mSocket,
		&WsaBuf,
		1,	// ���� ������ �����Ѵ�.
		&ReadBytes,
		&ReadFlag,
		&mReadOverlapped.Overlapped, // ReadOverlapped�� ����Ѵ�.
		NULL);


	if (ReturnVaue == SOCKET_ERROR && WSAGetLastError() != WSA_IO_PENDING && WSAGetLastError() != WSAEWOULDBLOCK)
	{
		End();

		return FALSE;
	}

	// IOCP�� ������ �����͸� ���� �� �̸� Read �Լ��� ȣ���� ���� ��
	// ������ Read IO�� �߻������� �װ��� IOCP�� WorkerThread���� �޾Ƽ�
	// ó���ϰ� �ϴ� ������ ������ �ִ�.
	// IOCP��� ���� �����̶�� �ϳ��� ��ü�� ��� ��ȭ��
	// �Ͼ������ Ȯ���ϰ� �˷��ִ� �ϳ��� ����̴�.
	// IOCP�� ������ IOCP�� ����ϰ� �ʱ� �ޱ� �۾��� �ϰ� �Ǹ�
	// �׶����� ���Ͽ� �����Ͱ� �����ų� ������ ����Ǿ�����
	// IOCP�� ���ؼ� IO�� ���޹ް� �ȴ�.

	return TRUE;
}

BOOL CNetworkSession::ReadForIocp(BYTE * data, DWORD & dataLength)
{
	// �ʱ� �ޱ⸦ �� �� ���� �����͸� ������ ���ۿ� �����ϴ� �Լ�
	CThreadSync	Sync;

	if (!mSocket)
		return FALSE;

	if (!data || dataLength <= 0)
		return FALSE;

	memcpy(data, mReadBuffer, dataLength);

	return TRUE;
	
	// IOCP���� IO_READ�� �߻��ؼ� �����͸� Ȯ���� �� ����ϴ� �Լ�
	// ReadForIocp �Լ��� IOCP���� ���� �����Ͱ� mReadBuffer�� ���� ��
	// IO�� �� ������ � ������ Ȯ���ϴ� �Լ�
}

BOOL CNetworkSession::ReadForEventSelect(BYTE * data, DWORD & dataLength)
{
	// IOCP�� �ƴ� �̺�Ʈ ��Ŀ��� ����� Read �Լ�
	CThreadSync	Sync;

	if (!mSocket)
		return FALSE;

	if (!data)
		return FALSE;

	WSABUF	WsaBuf;
	DWORD	ReadBytes = 0;
	DWORD	ReadFlag  = 0;

	WsaBuf.buf = (CHAR*)mReadBuffer;	// ���� �����͸� �Է�
	WsaBuf.len = MAX_BUFFER_LENGTH;		// ���� ũ�⸦ �Է�

	int	ReturnValue = WSARecv(mSocket,
		&WsaBuf,
		1,
		&ReadBytes,
		&ReadFlag,
		&mReadOverlapped.Overlapped,
		NULL);

	if (ReturnValue == SOCKET_ERROR && WSAGetLastError() != WSA_IO_PENDING && WSAGetLastError() != WSAEWOULDBLOCK)
	{
		End();

		return FALSE;
	}

	memcpy(data, mReadBuffer, ReadBytes);
	dataLength = ReadBytes;

	return TRUE;

	// EventSelect�� �̿��� ��� �����Ͱ� �Դٴ� ��ȣ�� �߻������� 
	// WSARecv�� ȣ���ϸ� ���� �����͸� �� ��� �о� �� �� �ִ�.
	// IOCP�� �̸� WSARecv�� ȣ���� ���� ���� ������ �����͸� ���� �� ���� �Ӹ��ƴ϶�
	// IOCP���� ��ȣ�� ���� �� WSARecv�� �����͸� �޴� ���� �ƴ�
	// �̸� ȣ���� ���Ҵ� WSARecv�� ���ۿ��� �����͸� Ȯ���� �� �ִ�.
}

BOOL CNetworkSession::Write(BYTE * data, DWORD dataLength)
{
	CThreadSync	Sync;

	if (!mSocket)
		return FALSE;

	if (!data || dataLength <= 0)
		return FALSE;

	WSABUF	WsaBuf;
	DWORD	WriteBytes = 0;
	DWORD	WriteFlag = 0;

	WsaBuf.buf = (CHAR*)data;
	WsaBuf.len = dataLength;

	INT	ReturnValue = WSASend(mSocket,
		&WsaBuf,
		1,
		&WriteBytes,
		WriteFlag,
		&mWriteOverlapped.Overlapped,
		NULL);

	if (ReturnValue == SOCKET_ERROR && WSAGetLastError() != WSA_IO_PENDING &&
		WSAGetLastError() != WSAEWOULDBLOCK)
	{
		End();

		return FALSE;
	}


	return TRUE;
}

BOOL CNetworkSession::InitializeReadFromForIocp()
{
	// IOCP���� ����� UDP �ʱ� �ޱ�
	CThreadSync	Sync;

	if (!mSocket)
		return FALSE;

	WSABUF	WsaBuf;
	DWORD	ReadBytes = 0;
	DWORD	ReadFlag = 0;
	INT		RemoteAddressInfoSize = sizeof(mUDPRemoteInfo);

	WsaBuf.buf = (CHAR*)mReadBuffer;
	WsaBuf.len = MAX_BUFFER_LENGTH;

	INT		ReturnValue = WSARecvFrom(mSocket,
		&WsaBuf,
		1,
		&ReadBytes,
		&ReadFlag,
		(SOCKADDR*)&mUDPRemoteInfo, // WSARecv�� �ٸ� ����... �����͸� ���� �ּ� ������ ��
		&RemoteAddressInfoSize,     // �ּ� ����
		&mReadOverlapped.Overlapped,
		NULL);

	if (ReturnValue == SOCKET_ERROR && WSAGetLastError() != WSA_IO_PENDING &&
		WSAGetLastError() != WSAEWOULDBLOCK)
	{
		End();

		return FALSE;
	}

	return TRUE;

	// IOCP Ư���� �ʱ� �ޱ⸦ �� �� �����ؾ� �Ѵ�.
	// TCP���� ����ϴ� InitializeReadForIOCP�� �ٸ� ����
	// ���� �����͸� ���� �ּ� ������ ���� �����Ѵ�.
	// �����͸� ���� ����� �ּҸ� mUDPRemoteInfo�� ������
	// ���߿� ReliableUDP���� ����ϰ� �ȴ�.
}

BOOL CNetworkSession::ReadFromForIocp(LPSTR remoteAddress, USHORT & remotePort, BYTE * data, DWORD & dataLength)
{
	// IOCP���� ������ ���� �����͸� Ȯ���ϴ� �Լ�
	CThreadSync	Sync;

	if (!mSocket)
		return FALSE;

	if (!data || !dataLength <= 0)
		return FALSE;

	// WSARecvFrom���� �����͸� �ޱ� ���ؼ� ����ߴ� mReadBuffer�� �����͸� ����
	memcpy(data, mReadBuffer, dataLength);

	strcpy(remoteAddress, inet_ntoa(mUDPRemoteInfo.sin_addr));
	remotePort = ntohs(mUDPRemoteInfo.sin_port);

	USHORT	Ack = 0;
	memcpy(&Ack, mReadBuffer, sizeof(USHORT)); // ���� �������� �պκ� 2����Ʈ�� Ȯ����

	if (Ack == 9999)
	{
		// ���� 2����Ʈ�� 9999�� ��� �����͸� �� ���� ���� ��Ŷ���� �ν�
		SetEvent(mReliableUDPWriteCompleteEvent);

		return FALSE;
	}

	else
	{
		// ���ο� �������� ���
		Ack = 9999;
		// �����͸� ���� �ּҷ� Ack 2����Ʈ�� ����
		WriteTo2(remoteAddress, remotePort, (BYTE*)&Ack, sizeof(USHORT));
	}

	// ���⼭ �߿��Ѱ��� 9999�� WriteTo�� �ƴ϶� WriteTo2�� �̿��ؼ�
	// ť�� ���� �ʰ� �ٷ� �����ٴ� ���̴�. 
	// ��Ŷ���� ����� �ܰ赵 �����ϰ� 9999 2����Ʈ�� �ٷ� ������.
	return TRUE;
}

BOOL CNetworkSession::ReadFromForEventSelect(LPSTR remoteAddress, USHORT & remotePort, BYTE * data, DWORD & dataLength)
{
	// EventSelect ����� ����� ���� ReadFrom
	CThreadSync	Sync;

	if (!mSocket)
		return FALSE;

	if (!data)
		return FALSE;

	if (!mSocket)
		return FALSE;

	WSABUF	WsaBuf;
	DWORD	ReadBytes = 0;
	DWORD	ReadFlag = 0;
	INT		RemoteAddressInfoSize = sizeof(mUDPRemoteInfo);

	WsaBuf.buf = (CHAR*)mReadBuffer;
	WsaBuf.len = MAX_BUFFER_LENGTH;

	INT	ReturnValue = WSARecvFrom(mSocket,
		&WsaBuf,
		1,
		&ReadBytes,
		&ReadFlag,
		(SOCKADDR*)&mUDPRemoteInfo,
		&RemoteAddressInfoSize,
		&mReadOverlapped.Overlapped,
		NULL);

	if (ReturnValue == SOCKET_ERROR && WSAGetLastError() != WSA_IO_PENDING &&
		WSAGetLastError() != WSAEWOULDBLOCK)
	{
		End();

		return FALSE;
	}

	memcpy(data, mReadBuffer, ReadBytes);
	dataLength = ReadBytes;

	strcpy(remoteAddress, inet_ntoa(mUDPRemoteInfo.sin_addr));
	remotePort = ntohs(mUDPRemoteInfo.sin_port);

	USHORT	Ack = 0;
	memcpy(&Ack, mReadBuffer, sizeof(USHORT));

	if (Ack == 9999)
	{
		SetEvent(mReliableUDPWriteCompleteEvent);

		return FALSE;
	}

	else
	{
		Ack = 9999;
		WriteTo2(remoteAddress, remotePort,(BYTE*) &Ack, sizeof(USHORT));
	}

	return TRUE;
}

BOOL CNetworkSession::WriteTo(LPCSTR remoteAddress, USHORT remotePort, BYTE * data, DWORD dataLength)
{
	// �����͸� ť�� �ִ� ������ �ϴ� �Լ��̴�.
	CThreadSync	Sync;

	if (!mSocket)
		return FALSE;

	if (!remoteAddress || remotePort <= 0 || !data || dataLength <= 0)
		return FALSE;

	if (!mReliableWriteQueue.Push(this, data, dataLength, remoteAddress, remotePort))
		return FALSE;

	if (!mIsReliableUDPSending)
	{
		mIsReliableUDPSending = TRUE;
		SetEvent(mReliableUDPThreadWakeUpEvent);
	}

	return TRUE;

	// �������� �����͸� ť�� �־��ִ� ���Ұ� ���ο� �����Ͱ� ������ ��
	// ReliableUDPThread�� ���� ������ �����ִ� ������ �ϰ� �ȴ�.
}

BOOL CNetworkSession::WriteTo2(LPSTR remoteAddress, USHORT remotePort, BYTE * data, DWORD dataLength)
{
	// ������ WSASendTo�� �̿��ؼ� �����͸� �����ϴ� ������ �ϴ� �Լ�
	CThreadSync	Sync;

	if (!mSocket)
		return FALSE;

	if (!remoteAddress || remotePort <= 0 || !data || dataLength <= 0)
		return FALSE;

	WSABUF	WsaBuf;
	DWORD	WriteBytes = 0;
	DWORD	WriteFlag = 0;

	SOCKADDR_IN	RemoteAddressInfo;
	INT			RemoteAddressInfoSize			= sizeof(RemoteAddressInfo);

	WsaBuf.buf = (CHAR*)data;
	WsaBuf.len = dataLength;

	RemoteAddressInfo.sin_family = AF_INET;
	RemoteAddressInfo.sin_addr.S_un.S_addr = inet_addr(remoteAddress);
	RemoteAddressInfo.sin_port = htons(remotePort);

	INT		ReturnValue = WSASendTo(mSocket,
		&WsaBuf,
		1,
		&WriteBytes,
		WriteFlag,
		(SOCKADDR*)&RemoteAddressInfo,
		RemoteAddressInfoSize,
		&mWriteOverlapped.Overlapped,
		NULL);

	if (ReturnValue == SOCKET_ERROR && WSAGetLastError() != WSA_IO_PENDING &&
		WSAGetLastError() != WSAEWOULDBLOCK)
	{
		End();

		return FALSE;
	}


	return TRUE;
}

BOOL CNetworkSession::Connect(LPSTR address, USHORT port)
{
	CThreadSync	Sync;

	if (!address || port <= 0)
		return FALSE;

	if (!mSocket)
		return FALSE;

	SOCKADDR_IN	RemoteAddressInfo;

	RemoteAddressInfo.sin_family	  = AF_INET;
	RemoteAddressInfo.sin_port		  = htons(port);
	RemoteAddressInfo.sin_addr.s_addr = inet_addr(address);

	if (WSAConnect(mSocket, (sockaddr*)&RemoteAddressInfo, sizeof(SOCKADDR_IN), NULL, NULL, NULL, NULL) == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSAEWOULDBLOCK)
		{
			End();
			return FALSE;
		}
	}

	return TRUE;
}

SOCKET CNetworkSession::GetSocket()
{
	CThreadSync Sync;

	return mSocket;
}

BOOL CNetworkSession::GetRemoteAddressAfterAceept(LPTSTR remoteAddress, USHORT & remotePort)
{
	CThreadSync	Sync;

	if (!remoteAddress)
		return FALSE;

	sockaddr_in	*Local		= NULL;
	INT			LocalLength = 0;

	sockaddr_in	*Remote		 = NULL;
	INT			RemoteLength = 0;

	GetAcceptExSockaddrs(mReadBuffer,
		0,
		sizeof(sockaddr_in) + 16,
		sizeof(sockaddr_in) + 16,
		(sockaddr **)&Local,
		&LocalLength,
		(sockaddr **)&Remote,
		&RemoteLength);

	CHAR	TempRemoteAddress[32] = { 0, };
	strcpy(TempRemoteAddress, inet_ntoa(Remote->sin_addr));

	MultiByteToWideChar(CP_ACP,
		0,
		TempRemoteAddress,
		-1,
		remoteAddress,
		32);

	remotePort = ntohs(Remote->sin_port);

	return TRUE;
}

void CNetworkSession::ReliableUDPThreadCallback()
{
	DWORD	EventID					= 0;
	HANDLE	ThreadEvents[2]			= { mReliableUDPThreadDestroyEvent, mReliableUDPThreadWakeUpEvent };

	CHAR	RemoteAddress[32]		= { 0, };
	USHORT	RemotePort				= 0;
	BYTE	Data[MAX_BUFFER_LENGTH] = { 0, };
	DWORD	DataLength				= 0;
	void	*Object					= NULL;

	while (TRUE)
	{
		SetEvent(mReliableUDPThreadStartupEvent);

		EventID = WaitForMultipleObjects(2, ThreadEvents, FALSE, INFINITE);
		switch (EventID)
		{
		case WAIT_OBJECT_0:
			return;

		case WAIT_OBJECT_0 + 1:
NEXT_DATA:
			// Write�� ��� Pop�� ���ְ�
			if (mReliableWriteQueue.Pop(&Object, Data, DataLength, RemoteAddress, RemotePort))
			{
				// �����Ͱ� ���� ���
				// ���� Write�� ���ְ� WaitForSingleObject�� ���ش�.
				// �޾��� �� �� SetEvent�� ���ָ� Ǯ����.
RETRY:
				// WriteTo2 �Լ��� �̿��ؼ� ��뿡�� �����͸� �޾Ҵٴ� ������ ���� ������ ��� �����͸� ����
				// ������ ���� ť���� ���� �����͸� ���� ����
				if (!WriteTo2(RemoteAddress, RemotePort, Data, DataLength))
					return;

				DWORD Result = WaitForSingleObject(mReliableUDPWriteCompleteEvent, 10);

				// �޾��� ��� ���� ������ ó��
				if (Result == WAIT_OBJECT_0)
					goto NEXT_DATA;
				else
					goto RETRY;
			}

			else // WriteTO �Լ����� ���� ������ �����Ͱ� �ִ��� �������� Ȯ���ϱ� ���� FLAG ����
				mIsReliableUDPSending = FALSE; // ť�� �� �̻� ���� �����Ͱ� ���� ���

			break;
		}
	}
}
