#pragma once

#include <iostream>
#include <tchar.h>

#include <winsock2.h>
#include <ws2tcpip.h>
#include <mswsock.h>
#include <windows.h>
#include <dbghelp.h>
#include <assert.h>
#include <time.h>
#include <process.h>

#include <vector>
#include <queue>
#include <list>
#include <map>
#include <string>
#include <utility>

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "mswsock.lib")

#import "C:\\Program Files\\Common Files\\System\\ADO\\msado15.dll" rename("EOF", "EndOfFile") no_namespace

using namespace std;

#define	MAX_BUFFER_LENGTH	1024

#ifdef _SERVER_SIDE
#define MAX_QUEUE_LENGTH	50
#else
#define MAX_QUEUE_LENGTH	1000
#endif

enum IO_TYPE
{
	IO_ACCEPT,
	IO_READ,
	IO_WRITE
};

typedef struct _OVERLAPPED_EX
{
	OVERLAPPED		Overlapped;
	IO_TYPE			IoType;
	VOID			*Object;
}OVERLAPPED_EX;

//#include "Log.h"

#define READ_PACKET(PROTOCOL)\
	BYTE WriteBuffer[MAX_BUFFER_LENGTH] = {0,};\
	S_##PROTOCOL Data;\
	READ_##PROTOCOL(pPacket, Data);
