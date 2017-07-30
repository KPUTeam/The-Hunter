#pragma once

#include "ZEROMultiThreadSync.h"

#define	MAX_BUFFER_LENGTH	1024

#ifdef _SERVER_SIDE
#define MAX_QUEUE_LENGTH	50
#else
#define MAX_QUEUE_LENGTH	1000
#endif

typedef struct _QUEUE_DATA
{
	VOID*	Object;
	BYTE	Data[MAX_BUFFER_LENGTH];
	DWORD	DataLength;

	CHAR	RemoteAddress[32];
	USHORT	RemotePort;

	DWORD	Protocol;
} QUEUE_DATA;

class CZEROCircularQueue : 
	public CZEROMultiThreadSync<CZEROCircularQueue>
{
public:
	CZEROCircularQueue(void);
	~CZEROCircularQueue(void);

private:
	QUEUE_DATA	m_Queue[MAX_QUEUE_LENGTH];
	DWORD		m_QueueHead;
	DWORD		m_QueueTail;

public:
	BOOL		Begin(VOID);
	BOOL		End(VOID);

	BYTE*		Push(VOID* object, BYTE* data, DWORD dataLength);
	BYTE*		Push(VOID* object, BYTE* data, DWORD dataLength, LPCSTR remoteAddress, USHORT remotePort);
	BYTE*		Push(VOID* object, DWORD protocol, BYTE* data, DWORD dataLength);
	BYTE*		Push(VOID* object, DWORD protocol, BYTE* data, DWORD dataLength, LPCSTR remoteAddress, USHORT remotePort);

	BOOL		Pop(VOID** object, BYTE* data, DWORD& dataLength);
	BOOL		Pop(VOID** object, BYTE* data, DWORD& dataLength, LPSTR remoteAddress, USHORT& remotePort);
	BOOL		Pop(VOID** object, DWORD& protocol, BYTE* data, DWORD& dataLength);
	BOOL		Pop(VOID** object, DWORD& protocol, BYTE* data, DWORD& dataLength, LPSTR remoteAddress, USHORT& remotePort); 
	BOOL		Pop(VOID);

	BOOL		IsEmpty(VOID);
};

