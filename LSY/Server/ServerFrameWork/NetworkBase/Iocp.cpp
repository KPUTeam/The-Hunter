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

	// CPU ������ 2�踸ŭ�� WorkerThread�� ����
	mWorkerThreadCount = SystemInfo.dwNumberOfProcessors * 2;

	// ����� ��Ʈ�� ���� ������
	mIocpHandle = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);

	if (!mIocpHandle)
		return false;

	// ������ �����ϴ� �̺�Ʈ
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

		// �ϳ��� �����帶�� ������ �Ϸ�� ������ ����Ѵ�.
		WaitForSingleObject(mStartUpEventHandle, INFINITE);
	}

	return true;
}

bool CIocp::End()
{
	// IOCP�� �̿��ؼ� �� WorkerThread���� ���Ḧ ��û��
	for (DWORD i = 0; i < mWorkerThreadVector.size(); ++i)
		PostQueuedCompletionStatus(mIocpHandle, 0, 0, NULL);

	// �� WorkerThread�� ����Ǳ⸦ ����� �ڵ��� �ݾ���
	for (DWORD i = 0; i < mWorkerThreadVector.size(); ++i)
	{
		// �� WorkerThread�� ������ ����� ������ ����Ѵ�.
		WaitForSingleObject(mWorkerThreadVector[i], INFINITE);

		CloseHandle(mWorkerThreadVector[i]);
	}

	if (mIocpHandle)
		CloseHandle(mIocpHandle);

	mWorkerThreadVector.clear();

	// ������ �����ϴ� �̺�Ʈ�� ������
	if (mStartUpEventHandle)
		CloseHandle(mStartUpEventHandle);

	return true;
}

bool CIocp::RegisterSocketToIocp(SOCKET socket, ULONG_PTR completionKey)
{
	if (!socket || !completionKey)
		return false;

	// Begin���� �����ߴ� mIocpHandle�� ����� �Ϸ� ��Ʈ�� ������
	mIocpHandle = CreateIoCompletionPort((HANDLE)socket, mIocpHandle, completionKey, 0);

	if (!mIocpHandle)
		return false;

	return true;
}

void CIocp::WorkerThreadCallback()
{
	bool Successed					= false;
	DWORD NumberOfByteTransfered	= 0;    // �� ����Ʈ�� IO�� �߻��ߴ��� Ȯ��
	void* CompletionKey				= NULL;
	OVERLAPPED* Overlapped			= NULL;
	OVERLAPPED_EX* OverlappedEx		= NULL;
	void* Object					= NULL; // ���� �ڵ鿡 ��ġ�Ǵ� ��ü ������ ��

	while (true)
	{
		// workerThread�� ������ �� �ִٰ� �˷��ִ� �̺�Ʈ
		SetEvent(mStartUpEventHandle);

		Successed = GetQueuedCompletionStatus(
			mIocpHandle,
			&NumberOfByteTransfered,
			(LPDWORD)&CompletionKey,
			&Overlapped,	// Read, Write � ���� Overlapped ��
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
