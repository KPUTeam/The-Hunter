#pragma once
#include "../Include.h"
#include "../../System Library/ThreadSync/MultiThreadSync.h"


class CCircularQueue : public CMultiThreadSync<CCircularQueue>
{
private:
	QUEUE_DATA		mQueue[MAX_QUEUE_LENGTH]; // ���� ť�� ���� �����Ͱ� ����Ǵ� �޸� ����
	DWORD			mQueueHead;				  // HEAD ������ �ϴ� ����
	DWORD			mQueueTail;				  // TAIL ������ �ϴ� ����

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

// ����ť�� �ϳ��� ť�� �����ϰ� ������ ���̸�ŭ ó������ ������ ��� �ݺ��ϸ鼭
// �����͸� ��/����ϴ� ����� �ϰ� �ȴ�.
// HEAD�� TAIL�� ��ġ���� �����ϴ� ������ �����ϰ�
// �����͸� �Է��Ҷ��� TAIL�� ��ġ�� �������� ���ο� �����Ͱ� ���� ��ġ�� �������ְ�
// �����͸� ������ ���� HEAD�� ���� �̿��� �����͸� ������ �� HEAD�� ��ġ ���� ��������
// ���� �����͸� ������ ��ġ�� �����ϰ� �ȴ�.

/*
�����ͷ� ����� ����ü�� �ϳ� �����Ѵ�.
typedef struct _QUEUE_DATA
{
	INT   iData1;
	BYTE  aData2[100];
} QUEUE_DATA;

// ť ��ü�� �����Ѵ�.
CCircularQueue<QUEUE_DATA> Queue;
Queue.Begin();

// �Է��� �����͸� �����Ѵ�.
QUEUE_DATA PushData;
PushData.iData1 = 10;
ZeroMemory(PushData.aData2, sizeof(PushData.aData2));

// �����͸� �Է��Ѵ�.
Queue.Push(PushData);

// ����� �����͸� �����Ѵ�.
Queue_DATA PopData;

// �����͸� ����Ѵ�.
Queue.Pop(PopData);

*/

