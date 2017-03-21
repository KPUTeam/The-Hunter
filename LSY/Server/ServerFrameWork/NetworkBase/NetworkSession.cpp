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
	mReliableUDPThreadHandle		= NULL;	// 같은 패킷을 받을 떄까지 계속 전송해주는 스레드 핸들이다.
	mReliableUDPThreadStartupEvent  = NULL; // ReliableUDPThread의 시작을 알리는 이벤트
	mReliableUDPThreadDestroyEvent  = NULL; // ReliableUDPThread의 종료를 알리는 이벤트
	mReliableUDPThreadWakeUpEvent   = NULL; // ReliableUDPThread를 깨울때 사용하는 이벤트
	mReliableUDPWriteCompleteEvent  = NULL; // 상대가 패킷을 받아 더 이상 보낼 필요가 없을 때 사용하는 이벤트

	mIsReliableUDPSending			= FALSE; // 현재 보내고 있는 Reliable Data가 있는지를 확인한다.

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
	// 앞으로 우리가 사용하게 될 기본적은 SOCKET 핸들과
	// 스레드를 관리하는 핸들 및 각종 이벤트를 초기화함

	CThreadSync	Sync; // 다중 스레드 동기화

	if (mSocket)
		return FALSE;
	
	// 변수 초기화
	memset(mReadBuffer, 0, sizeof(mReadBuffer));
	// UDP를 사용할 때 데이터를 보낼 상대 정보를 저장하기 위한 변수
	memset(&mUDPRemoteInfo, 0, sizeof(mUDPRemoteInfo));

	mSocket							= NULL; // 앞으로 계속 사용하게 될 소켓 핸들
	mReliableUDPThreadHandle		= NULL; // ReliableUDP 메인 스레드 해들
	mReliableUDPThreadStartupEvent  = NULL; // 스레드가 시작됨을 알려주는 이벤트
	mReliableUDPThreadDestroyEvent  = NULL; // 스레드가 종료되었음을 알려주는 이벤트
	mReliableUDPThreadWakeUpEvent   = NULL;
	mReliableUDPWriteCompleteEvent  = NULL; // 스레드 내 한개의 데이터 전송이 완료되었을때 
											// 발생하는 이벤트
	mIsReliableUDPSending			= FALSE;// 현재 데이터를 전송중인지 확인하는 FLAG 변수

	return TRUE;
}

BOOL CNetworkSession::End()
{
	CThreadSync	Sync; // 다중 스레드 동기화

	if (!mSocket)
		return FALSE;

	shutdown(mSocket, SD_BOTH); // 해당 소켓의 READ/WRITE를 멈춤
	closesocket(mSocket);		// 해당 소켓을 닫는다.

	mSocket = NULL;

	if (mReliableUDPThreadHandle)
	{
		// 스레드를 종료하는 이벤트를 발생시킨다.
		SetEvent(mReliableUDPThreadDestroyEvent);
		// 스레드가 종료될때까지 무한대로 대기한다.
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

	// Reliable UDP로 보낼 데이터를 저장하는 WriteQueue를 비운다.
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

	// LINGET는 소켓을 Close할 때 영향을 미치는 함수로
	// 현재 보내고 있거나 받는 데이터가 있을 경우 
	// 그 데이터를 모두 보낸 후 소켓을 Clost하게 하는 옵션이다.
	// 사용법은 setsockopt 함수로 설정을 하고
	// l_onfoo 값을 1로 만들어주면 동작하는데
	// 소켓을 안전하게 종료하기 위해서 꼭 필요한 옵션이지만
	// 보내고 받는 데이터가 많은 경우 종료에 많은 시간이 걸림....
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

	if (!listenSocket) // listen이 먼저 실행되지 않으면 실행되지 않는다.
		return FALSE;

	if (mSocket)
		return FALSE;

	// Accept에 사용할 소켓을 미리 생성한다.
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

	// Accept함수 대신에 AcceptEx 함수를 사용한 이유
	// 일반적으로 while문을 사용한 WSAAccept는 하나의 Accept가 완료되기 전에
	// 다른 연결에 대한 Accept를 할 수가 없다. 그래서 
	// 갑자기 많은 접속이 몰릴 경우 접속이 실패될 수가 있다.
	// AcceptEx는 WsaAccept와 다르게 while문으로 처리하는 것이 아니라
	// AcceptEx당 하나의 소켓을 미리 만들어두고
	// Accept 요청이 있을 때 만들어둔 소켓을 활용하는 방식이다.
	// AcceptEx는 Accept를 받으면서 한 번 데이터를 받을 수 있다는 것이다.
	// 즉, WSAAccept로 연결을 허락하고, 초기 받기를 한 번 시도하는 것으로
	// 상태를 마무리하는 일반적인 방법과 달리 AcceptEx는 한 번에 진행할 수 있다.
	
	return TRUE;;
}

// TCP를 사용할 경우 호출
BOOL CNetworkSession::TCPBind()
{
	CThreadSync	Sync;

	// mSocket이 NULL이 아닐 경우 이미 소켓이 생성되어 Bind 되어있는 상태이므로 다시 TCPBind함수를 쓸 수 없다.
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

	// UDP로 사용할 소켓을 생성해주고 지정된 포트를 바인딩해주는 역할을 한다.
	// 그리고 내부에서는 ReliableUDPThread를 사용하기 위한 이벤트들과
	// ReliableUDPThread 자신을 생성한다.
	// ReliableUDPThread가 생성 완료될때가지 함수를 대기시킨다.
	// 이렇게 대기시키는 이유는 스레드가 생성되고 ReliableUDP를 전송할 준비가 된 상태에서
	// 함수를 완료시키기 위함이다.
	// CircularQueue는 ReliableUDPThread가 어떠한 데이터를 보내고 있을 떄
	// 다음으로 보내야 할 데이터를 저장하고 있는 역할을 하게 된다.
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

BOOL CNetworkSession::InitializeReadForIocp() // IOCP를 사용할 때 사용하는 초기 받기
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
		1,	// 버퍼 개수를 지정한다.
		&ReadBytes,
		&ReadFlag,
		&mReadOverlapped.Overlapped, // ReadOverlapped를 사용한다.
		NULL);


	if (ReturnVaue == SOCKET_ERROR && WSAGetLastError() != WSA_IO_PENDING && WSAGetLastError() != WSAEWOULDBLOCK)
	{
		End();

		return FALSE;
	}

	// IOCP는 구조상 데이터를 받을 때 미리 Read 함수를 호출해 놓은 뒤
	// 실제로 Read IO가 발생했을때 그것을 IOCP의 WorkerThread에서 받아서
	// 처리하게 하는 구조를 가지고 있다.
	// IOCP라는 것은 소켓이라는 하나의 개체에 어떠한 변화가
	// 일어났는지를 확인하고 알려주는 하나의 방식이다.
	// IOCP는 소켓을 IOCP에 등록하고 초기 받기 작업을 하게 되면
	// 그때부터 소켓에 데이터가 들어오거나 연결이 종료되었을때
	// IOCP를 통해서 IO를 전달받게 된다.

	return TRUE;
}

BOOL CNetworkSession::ReadForIocp(BYTE * data, DWORD & dataLength)
{
	// 초기 받기를 한 후 실제 데이터를 정해진 버퍼에 복사하는 함수
	CThreadSync	Sync;

	if (!mSocket)
		return FALSE;

	if (!data || dataLength <= 0)
		return FALSE;

	memcpy(data, mReadBuffer, dataLength);

	return TRUE;
	
	// IOCP에서 IO_READ가 발생해서 데이터를 확일할 때 사용하는 함수
	// ReadForIocp 함수는 IOCP에서 실제 데이터가 mReadBuffer로 들어올 떄
	// IO가 그 내용이 어떤 것인지 확인하는 함수
}

BOOL CNetworkSession::ReadForEventSelect(BYTE * data, DWORD & dataLength)
{
	// IOCP가 아닌 이벤트 방식에서 사용할 Read 함수
	CThreadSync	Sync;

	if (!mSocket)
		return FALSE;

	if (!data)
		return FALSE;

	WSABUF	WsaBuf;
	DWORD	ReadBytes = 0;
	DWORD	ReadFlag  = 0;

	WsaBuf.buf = (CHAR*)mReadBuffer;	// 버퍼 포인터를 입력
	WsaBuf.len = MAX_BUFFER_LENGTH;		// 버퍼 크기를 입력

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

	// EventSelect를 이용할 경우 데이터가 왔다는 신호가 발생했을때 
	// WSARecv를 호출하면 받은 데이터를 그 즉시 읽어 올 수 있다.
	// IOCP는 미리 WSARecv를 호출해 놓고 있지 않으면 데이터를 받을 수 없을 뿐만아니라
	// IOCP에서 신호가 왔을 때 WSARecv로 데이터를 받는 것이 아닌
	// 미리 호출해 놓았던 WSARecv의 버퍼에서 데이터를 확인할 수 있다.
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
	// IOCP에서 사용할 UDP 초기 받기
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
		(SOCKADDR*)&mUDPRemoteInfo, // WSARecv와 다른 인자... 데이터를 받은 주소 정보가 들어감
		&RemoteAddressInfoSize,     // 주소 길이
		&mReadOverlapped.Overlapped,
		NULL);

	if (ReturnValue == SOCKET_ERROR && WSAGetLastError() != WSA_IO_PENDING &&
		WSAGetLastError() != WSAEWOULDBLOCK)
	{
		End();

		return FALSE;
	}

	return TRUE;

	// IOCP 특성인 초기 받기를 한 번 실행해야 한다.
	// TCP에서 사용하는 InitializeReadForIOCP와 다른 점은
	// 받은 데이터를 보낸 주소 정보를 따로 저장한다.
	// 데이터를 보낸 상대의 주소를 mUDPRemoteInfo에 저장해
	// 나중에 ReliableUDP에서 사용하게 된다.
}

BOOL CNetworkSession::ReadFromForIocp(LPSTR remoteAddress, USHORT & remotePort, BYTE * data, DWORD & dataLength)
{
	// IOCP에서 실제로 받은 데이터를 확인하는 함수
	CThreadSync	Sync;

	if (!mSocket)
		return FALSE;

	if (!data || !dataLength <= 0)
		return FALSE;

	// WSARecvFrom에서 데이터를 받기 위해서 등록했던 mReadBuffer의 데이터를 복사
	memcpy(data, mReadBuffer, dataLength);

	strcpy(remoteAddress, inet_ntoa(mUDPRemoteInfo.sin_addr));
	remotePort = ntohs(mUDPRemoteInfo.sin_port);

	USHORT	Ack = 0;
	memcpy(&Ack, mReadBuffer, sizeof(USHORT)); // 받은 데이터의 앞부분 2바이트를 확인후

	if (Ack == 9999)
	{
		// 앞의 2바이트가 9999일 경우 데이터를 잘 받은 응답 패킷으로 인식
		SetEvent(mReliableUDPWriteCompleteEvent);

		return FALSE;
	}

	else
	{
		// 새로운 데이터일 경우
		Ack = 9999;
		// 데이터를 보낸 주소로 Ack 2바이트를 전송
		WriteTo2(remoteAddress, remotePort, (BYTE*)&Ack, sizeof(USHORT));
	}

	// 여기서 중요한것은 9999는 WriteTo가 아니라 WriteTo2를 이용해서
	// 큐에 넣지 않고 바로 보낸다는 것이다. 
	// 패킷으로 만드는 단계도 생략하고 9999 2바이트만 바로 보낸다.
	return TRUE;
}

BOOL CNetworkSession::ReadFromForEventSelect(LPSTR remoteAddress, USHORT & remotePort, BYTE * data, DWORD & dataLength)
{
	// EventSelect 방식을 사용할 때의 ReadFrom
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
	// 데이터를 큐에 넣는 역할을 하는 함수이다.
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

	// 보내야할 데이터를 큐에 넣어주는 역할과 새로운 데이터가 들어왔을 때
	// ReliableUDPThread가 쉬고 있으면 깨워주는 역할을 하게 된다.
}

BOOL CNetworkSession::WriteTo2(LPSTR remoteAddress, USHORT remotePort, BYTE * data, DWORD dataLength)
{
	// 실제로 WSASendTo를 이용해서 데이터를 전송하는 역할을 하는 함수
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
			// Write일 경우 Pop을 해주고
			if (mReliableWriteQueue.Pop(&Object, Data, DataLength, RemoteAddress, RemotePort))
			{
				// 데이터가 있을 경우
				// 실제 Write를 해주고 WaitForSingleObject를 해준다.
				// 받았을 때 그 SetEvent를 해주면 풀린다.
RETRY:
				// WriteTo2 함수를 이용해서 상대에게 데이터를 받았다는 응답이 오기 전까지 계속 데이터를 전송
				// 응답이 오면 큐에서 다음 데이터를 꺼내 전송
				if (!WriteTo2(RemoteAddress, RemotePort, Data, DataLength))
					return;

				DWORD Result = WaitForSingleObject(mReliableUDPWriteCompleteEvent, 10);

				// 받았을 경우 다음 데이터 처리
				if (Result == WAIT_OBJECT_0)
					goto NEXT_DATA;
				else
					goto RETRY;
			}

			else // WriteTO 함수에서 현재 보내는 데이터가 있는지 없는지를 확인하기 위한 FLAG 변수
				mIsReliableUDPSending = FALSE; // 큐에 더 이상 보낼 데이터가 없을 경우

			break;
		}
	}
}
