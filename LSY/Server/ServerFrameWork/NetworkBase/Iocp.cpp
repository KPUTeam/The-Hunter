#include "Iocp.h"

DWORD WINAPI WorkerThreadCallback(LPVOID parameter)
{
	CIocp* Owner = (CIocp*)parameter;
	Owner->WorkerThreadCallback();

	return 0;
}

CIocp::CIocp()
	:mIocpHandle(NULL),
	 mWorkerThreadCount(0),
	 mStartUpEventHandle(NULL)
{
}


CIocp::~CIocp()
{
}

bool CIocp::Begin()
{
	mIocpHandle = NULL;

	SYSTEM_INFO	SystemInfo;
	GetSystemInfo(&SystemInfo);

	// CPU 개수의 2배만큼의 WorkerThread를 만듬
	mWorkerThreadCount = SystemInfo.dwNumberOfProcessors * 2;

	// 입출력 포트를 새로 생성함
	mIocpHandle = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);

	if (!mIocpHandle)
		return false;

	// 시작을 관리하는 이벤트
	mStartUpEventHandle = CreateEvent(0, false, false, 0);
	if (mStartUpEventHandle == NULL)
	{
		End();
		return false;
	}

	for (DWORD i = 0; i < mWorkerThreadCount; ++i)
	{
		HANDLE WorkerThread = CreateThread(NULL, 0, ::WorkerThreadCallback, this, 0, NULL);
		mWorkerThreadVector.push_back(WorkerThread);

		// 하나의 스레드마다 생성이 완료될 때까지 대기한다.
		WaitForSingleObject(mStartUpEventHandle, INFINITE);
	}

	return true;
}

bool CIocp::End()
{
	// IOCP를 이용해서 각 WorkerThread마다 종료를 요청함
	for (DWORD i = 0; i < mWorkerThreadVector.size(); ++i)
		PostQueuedCompletionStatus(mIocpHandle, 0, 0, NULL);

	// 각 WorkerThread가 종료되기를 가디려 핸들을 닫아줌
	for (DWORD i = 0; i < mWorkerThreadVector.size(); ++i)
	{
		// 한 WorkerThread가 완전히 종료될 때까지 대기한다.
		WaitForSingleObject(mWorkerThreadVector[i], INFINITE);

		CloseHandle(mWorkerThreadVector[i]);
	}

	if (mIocpHandle)
		CloseHandle(mIocpHandle);

	mWorkerThreadVector.clear();

	// 시작을 관리하는 이벤트를 종료함
	if (mStartUpEventHandle)
		CloseHandle(mStartUpEventHandle);

	return true;
}

bool CIocp::RegisterSocketToIocp(SOCKET socket, ULONG_PTR completionKey)
{
	if (!socket || !completionKey)
		return false;

	// Begin에서 생성했던 mIocpHandle에 입출력 완료 포트를 연결함
	mIocpHandle = CreateIoCompletionPort((HANDLE)socket, mIocpHandle, completionKey, 0);

	if (!mIocpHandle)
		return false;

	return true;
}

void CIocp::WorkerThreadCallback()
{
	bool Successed					= false;
	DWORD NumberOfByteTransfered	= 0;    // 몇 바이트의 IO가 발생했는지 확인
	void* CompletionKey				= NULL;
	OVERLAPPED* Overlapped			= NULL;
	OVERLAPPED_EX* OverlappedEx		= NULL;
	void* Object					= NULL; // 소켓 핸들에 매치되는 개체 포인터 값

	while (true)
	{
		// workerThread를 시작할 수 있다고 알려주는 이벤트
		SetEvent(mStartUpEventHandle);

		Successed = GetQueuedCompletionStatus(
			mIocpHandle,
			&NumberOfByteTransfered,
			(LPDWORD)&CompletionKey,
			&Overlapped,	// Read, Write 등에 사용된 Overlapped 값
			INFINITE);

		if (!CompletionKey)
			return;

		OverlappedEx = (OVERLAPPED_EX*) Overlapped;
		Object = OverlappedEx->Object;

		if (!Successed || (Successed && !NumberOfByteTransfered))
		{
			if (OverlappedEx->IoType == IO_ACCEPT)
				OnIoConnected(Object);
			else
				OnIoDisconnected(Object);

			continue;
		}

		switch (OverlappedEx->IoType)
		{
		case IO_READ:
			OnIoRead(Object, NumberOfByteTransfered);
			break;

		case IO_WRITE:
			OnIoWrite(Object, NumberOfByteTransfered);
			break;
		}
	}
}
