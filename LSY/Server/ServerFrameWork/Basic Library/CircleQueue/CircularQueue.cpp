#include "CircularQueue.h"
#include "../Log/Log.h"

CCircularQueue::CCircularQueue()
{
	memset(mQueue, 0, sizeof(mQueue));

	mQueueHead = 0;
	mQueueTail = 0;
}

CCircularQueue::~CCircularQueue()
{
}

BOOL CCircularQueue::Begin()
{
	memset(mQueue, 0, sizeof(mQueue));
	mQueueHead = mQueueTail = 0;

	return TRUE;
}

BOOL CCircularQueue::End() 
{
	return TRUE; 
}

BYTE * CCircularQueue::Push(void * object, BYTE * data, DWORD dataLength)// �����͸� �Է��� �� ����ϴ� �Լ��̴�.
{
	CThreadSync	Sync;

	//	CLog::WriteLog(_T("Queue Status : Head (%d) Tail (%d) \n"), mQueueHead, mQueueTail);

	if (!object || !data)
		return NULL;

	// ���� ���ο� �����͸� �Է��� ��ġ�� TempTail�� �����Ѵ�.
	DWORD TempTail = (mQueueTail + 1) % MAX_QUEUE_LENGTH;

	if (TempTail == mQueueHead) // ������ Head�� TempTail�� ���� ��� ť�� ������ �����̹Ƿ� ���� ���Ѵ�.
		return NULL;

	mQueue[TempTail].Object = object;
	mQueue[TempTail].DataLength = dataLength;

	memcpy(mQueue[TempTail].Data, data, dataLength); // �����͸� �Է��Ѵ�.

	mQueueTail = TempTail; // ����� TempTail�� ���� ���� Tail���� ����ִ� mQueueTail�� �Է��Ѵ�.
	
	return mQueue[TempTail].Data;
}

BYTE * CCircularQueue::Push(void * object, BYTE * data, DWORD dataLength, LPCSTR remoteAddress, USHORT remotePort)
{
	CThreadSync	Sync;

	//	CLog::WriteLog(_T("Queue Status : Head (%d) Tail (%d) \n"), mQueueHead, mQueueTail);

	if (!object || !data)
		return NULL;

	DWORD TempTail = (mQueueTail + 1) % MAX_QUEUE_LENGTH;

	if (TempTail == mQueueHead) 
		return NULL;

	mQueue[TempTail].Object = object;
	mQueue[TempTail].DataLength = dataLength;
	mQueue[TempTail].RemotePort = remotePort;

	memset(mQueue[TempTail].RemoteAddress, 0, sizeof(mQueue[TempTail].RemoteAddress));
	strncpy(mQueue[TempTail].RemoteAddress, remoteAddress, sizeof(mQueue[TempTail].RemoteAddress));

	memset(mQueue[TempTail].Data, 0, sizeof(mQueue[TempTail].Data));
	memcpy(mQueue[TempTail].Data, data, dataLength); 

	mQueueTail = TempTail; 
	
	return mQueue[TempTail].Data;
}

BYTE * CCircularQueue::Push(void * object, DWORD protocol, BYTE * data, DWORD dataLength)
{
	CThreadSync	Sync;

	//	CLog::WriteLog(_T("Queue Status : Head (%d) Tail (%d) \n"), mQueueHead, mQueueTail);

	if (!object || !data)
		return NULL;

	DWORD TempTail = (mQueueTail + 1) % MAX_QUEUE_LENGTH;

	if (TempTail == mQueueHead) 
		return NULL;

	mQueue[TempTail].Object = object;
	mQueue[TempTail].DataLength = dataLength;
	mQueue[TempTail].Protocol = protocol;

	memset(mQueue[TempTail].Data, 0, sizeof(mQueue[TempTail].Data));
	memcpy(mQueue[TempTail].Data, data, dataLength);

	mQueueTail = TempTail; 

	return mQueue[TempTail].Data;
}

BYTE * CCircularQueue::Push(void * object, DWORD protocol, BYTE * data, DWORD dataLength, LPCSTR remoteAddress, USHORT remotePort)
{
	CThreadSync	Sync;

	//	CLog::WriteLog(_T("Queue Status : Head (%d) Tail (%d) \n"), mQueueHead, mQueueTail);

	if (!object || !data)
		return NULL;

	DWORD TempTail = (mQueueTail + 1) % MAX_QUEUE_LENGTH;

	if (TempTail == mQueueHead)
		return NULL;

	mQueue[TempTail].Object = object;
	mQueue[TempTail].DataLength = dataLength;
	mQueue[TempTail].RemotePort = remotePort;
	mQueue[TempTail].Protocol = protocol;

	memset(mQueue[TempTail].RemoteAddress, 0, sizeof(mQueue[TempTail].RemoteAddress));
	strncpy(mQueue[TempTail].RemoteAddress, remoteAddress, sizeof(mQueue[TempTail].RemoteAddress));

	memset(mQueue[TempTail].Data, 0, sizeof(mQueue[TempTail].Data));
	memcpy(mQueue[TempTail].Data, data, dataLength);

	mQueueTail = TempTail;

	return mQueue[TempTail].Data;
}

BOOL CCircularQueue::Pop() // �����͸� ����� �� ����ϴ� �Լ��̴�.
{
	CThreadSync	Sync;

	//CLog::WriteLog(_T("Queue Status  : Head (%d) Tail (%d)\n"), mQueueHead, mQueueTail);

	if (mQueueHead == mQueueTail) // HEAD�� TAiL�� ������ Ȯ���Ͽ� ť�� ����ִ��� Ȯ���Ѵ�.
		return FALSE;

	// ���� �������� ���� HEAD�� ���� �̿��ؼ� TempHead�� ����.
	DWORD TempHead = (mQueueHead + 1) % MAX_QUEUE_LENGTH;

//	CopyMemory(&data, &mQueue[TempHead], sizeof(T)); // ���� ���´�.

	// ����� TempHead�� ���� ���� HEAD���� ����ִ� mQueueHead�� �Է��Ѵ�.
	mQueueHead = TempHead;

	return TRUE;
}

BOOL CCircularQueue::Pop(void ** object, BYTE * data, DWORD & dataLength)
{
	CThreadSync	Sync;

	//CLog::WriteLog(_T("Queue Status  : Head (%d) Tail (%d)\n"), mQueueHead, mQueueTail);

	if (!object || !data)
		return FALSE;

	if (mQueueHead == mQueueTail)
		return FALSE;

	DWORD TempHead = (mQueueHead + 1) % MAX_QUEUE_LENGTH;

	dataLength = mQueue[TempHead].DataLength;
	*object    = mQueue[TempHead].Object;

	memcpy(data, mQueue[TempHead].Data, mQueue[TempHead].DataLength);

	mQueueHead = TempHead;

	return 0;
}

BOOL CCircularQueue::Pop(void ** object, BYTE * data, DWORD & dataLength, LPSTR remoteAddress, USHORT & remotePort)
{
	CThreadSync	Sync;

	//CLog::WriteLog(_T("Queue Status  : Head (%d) Tail (%d)\n"), mQueueHead, mQueueTail);

	if (!object || !data)
		return FALSE;

	if (mQueueHead == mQueueTail)
		return FALSE;

	DWORD TempHead = (mQueueHead + 1) % MAX_QUEUE_LENGTH;

	dataLength = mQueue[TempHead].DataLength;
	*object    = mQueue[TempHead].Object;
	remotePort = mQueue[TempHead].RemotePort;

	strncpy(remoteAddress, mQueue[TempHead].RemoteAddress, sizeof(mQueue[TempHead].RemoteAddress));
	memcpy(data, mQueue[TempHead].Data, mQueue[TempHead].DataLength);

	mQueueHead = TempHead;

	return 0;
}

BOOL CCircularQueue::Pop(void ** object, DWORD & protocol, BYTE * data, DWORD & dataLength)
{
	CThreadSync	Sync;

	//CLog::WriteLog(_T("Queue Status  : Head (%d) Tail (%d)\n"), mQueueHead, mQueueTail);

	if (!object || !data)
		return FALSE;

	if (mQueueHead == mQueueTail)
		return FALSE;

	DWORD TempHead = (mQueueHead + 1) % MAX_QUEUE_LENGTH;

	dataLength = mQueue[TempHead].DataLength;
	*object    = mQueue[TempHead].Object;
	protocol   = mQueue[TempHead].Protocol;

	memcpy(data, mQueue[TempHead].Data, mQueue[TempHead].DataLength);

	mQueueHead = TempHead;

	return 0;
}

BOOL CCircularQueue::Pop(void ** object, DWORD & protocol, BYTE * data, DWORD & dataLength, LPSTR remoteAddress, USHORT & remotePort)
{
	CThreadSync	Sync;

	//CLog::WriteLog(_T("Queue Status  : Head (%d) Tail (%d)\n"), mQueueHead, mQueueTail);

	if (!object || !data)
		return FALSE;

	if (mQueueHead == mQueueTail)
		return FALSE;

	DWORD TempHead = (mQueueHead + 1) % MAX_QUEUE_LENGTH;

	dataLength = mQueue[TempHead].DataLength;
	*object    = mQueue[TempHead].Object;
	protocol   = mQueue[TempHead].Protocol;
	remotePort = mQueue[TempHead].RemotePort;
	
	strncpy(remoteAddress, mQueue[TempHead].RemoteAddress, sizeof(mQueue[TempHead].RemoteAddress));
	memcpy(data, mQueue[TempHead].Data, mQueue[TempHead].DataLength);

	mQueueHead = TempHead;

	return 0;
}


BOOL CCircularQueue::IsEmpty()
{
	CThreadSync Sync;
	
	if (mQueueHead == mQueueTail)	
		return TRUE;

	return FALSE;
}