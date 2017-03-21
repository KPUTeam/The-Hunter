#pragma once

#include <Windows.h>
#include <tchar.h>
#include <iostream>

#define MAX_QUEUE_LENGTH	1023
#define MAX_BUFFER_LENGTH   2048

using namespace std;

typedef struct _QUEUE_DATA
{
	void	*Object;
	BYTE	Data[MAX_BUFFER_LENGTH];
	DWORD	DataLength;

	CHAR	RemoteAddress[32];
	USHORT	RemotePort;

	DWORD	Protocol;
} QUEUE_DATA, *PQUEUE_DATA;