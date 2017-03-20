#pragma once
#include "../Include.h"
#include "../../System Library/ThreadSync/MultiThreadSync.h"


class CCircularQueue : public CMultiThreadSync<CCircularQueue>
{
private:
	QUEUE_DATA		mQueue[MAX_QUEUE_LENGTH]; // 실제 큐에 들어가는 데이터가 저장되는 메모리 영역
	DWORD			mQueueHead;				  // HEAD 역할을 하는 변수
	DWORD			mQueueTail;				  // TAIL 역할을 하는 변수

public:
	CCircularQueue();
	~CCircularQueue();

public:
	BOOL		Begin();
	BOOL		End();

	BYTE*		Push(void* object, BYTE* data, DWORD dataLength);
	BYTE*		Push(void* object, BYTE* data, DWORD dataLength, LPCSTR remoteAddress, USHORT remotePort);
	BYTE*		Push(void* object, DWORD protocol, BYTE* data, DWORD dataLength);
	BYTE*		Push(void* object, DWORD protocol, BYTE* data, DWORD dataLength, LPCSTR remoteAddress, USHORT remotePort);

	BOOL		Pop();
	BOOL		Pop(void** object, BYTE* data, DWORD& dataLength);
	BOOL		Pop(void** object, BYTE* data, DWORD& dataLength, LPSTR remoteAddress, USHORT& remotePort);
	BOOL		Pop(void** object, DWORD& protocol, BYTE* data, DWORD& dataLength);
	BOOL		Pop(void** object, DWORD& protocol, BYTE* data, DWORD& dataLength, LPSTR remoteAddress, USHORT& remotePort);

	BOOL		IsEmpty();
};

// 원형큐는 하나의 큐를 생성하고 정해진 길이만큼 처음부터 끝까지 계속 반복하면서
// 데이터를 입/출력하는 기능을 하게 된다.
// HEAD와 TAIL의 위치값을 저장하는 변수를 지정하고
// 데이터를 입력할때는 TAIL의 위치를 증가시켜 새로운 데이터가 들어올 위치를 설정해주고
// 데이터를 가져올 떄는 HEAD의 값을 이용해 데이터를 가져온 후 HEAD의 위치 값을 증가시켜
// 다음 데이터를 가져올 위치를 설정하게 된다.

/*
데이터로 사용할 구조체를 하나 선언한다.
typedef struct _QUEUE_DATA
{
	INT   iData1;
	BYTE  aData2[100];
} QUEUE_DATA;

// 큐 객체를 생성한다.
CCircularQueue<QUEUE_DATA> Queue;
Queue.Begin();

// 입력할 데이터를 정의한다.
QUEUE_DATA PushData;
PushData.iData1 = 10;
ZeroMemory(PushData.aData2, sizeof(PushData.aData2));

// 데이터를 입력한다.
Queue.Push(PushData);

// 출력할 데이터를 정의한다.
Queue_DATA PopData;

// 데이터를 출력한다.
Queue.Pop(PopData);

*/

