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

BYTE * CCircularQueue::Push(void * object, BYTE * data, DWORD dataLength)// 데이터를 입력할 때 사용하는 함수이다.
{
	CThreadSync	Sync;

	//	CLog::WriteLog(_T("Queue Status : Head (%d) Tail (%d) \n"), mQueueHead, mQueueTail);

	if (!object || !data)
		return NULL;

	// 먼저 새로운 데이터를 입력할 위치를 TempTail에 저장한다.
	DWORD TempTail = (mQueueTail + 1) % MAX_QUEUE_LENGTH;

	if (TempTail == mQueueHead) // 현재의 Head와 TempTail이 같을 경우 큐가 가득찬 상태이므로 넣지 못한다.
		return NULL;

	mQueue[TempTail].Object = object;
	mQueue[TempTail].DataLength = dataLength;

	memcpy(mQueue[TempTail].Data, data, dataLength); // 데이터를 입력한다.

	mQueueTail = TempTail; // 사용한 TempTail의 값을 실제 Tail값이 들어있는 mQueueTail에 입력한다.
	
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

BOOL CCircularQueue::Pop() // 데이터를 출력할 때 사용하는 함수이다.
{
	CThreadSync	Sync;

	//CLog::WriteLog(_T("Queue Status  : Head (%d) Tail (%d)\n"), mQueueHead, mQueueTail);

	if (mQueueHead == mQueueTail) // HEAD와 TAiL이 같은지 확인하여 큐가 비어있는지 확인한다.
		return FALSE;

	// 값을 가져오기 위해 HEAD의 값을 이용해서 TempHead를 얻어낸다.
	DWORD TempHead = (mQueueHead + 1) % MAX_QUEUE_LENGTH;

//	CopyMemory(&data, &mQueue[TempHead], sizeof(T)); // 값을 얻어온다.

	// 사용한 TempHead의 값을 실제 HEAD값이 들어있는 mQueueHead에 입력한다.
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