#pragma once

#include <WinSock2.h>
#include <WS2tcpip.h>

#pragma comment(lib, "ws2_32")

#if defined(DEBUG) | defined(_DEBUG)
#pragma comment(lib, "ZEROLowLib.lib")
#else
#pragma comment(lib, "ZEROLowLib")
#endif

#define PORT	9001
#define	PACKET_SIZE 1024
#define SERVER_IP	"127.0.0.1"

#define ID_SIZE	64
#define PASS_SIZE	64

enum PACKET_HEADER
{
	PH_NONE,
	PH_CONNECT,
	PH_DISCONNECT,
	PH_LOGIN,
	PH_LOGIN_FAILED,
	PH_LOGIN_SUCCEEDED,
	PH_USER_LOGIN,
	PH_MOVE,
	PH_MY_MOVE,
	PH_OTHER_MOVE
};

enum PACKET_MOVE
{
	PM_NONE,
	PM_LEFT,
	PM_RIGHT,
	PM_FRONT,
	PM_BACK
};

enum NET_OBJECT_TYPE
{
	NOT_USER
};

typedef struct _tagPacket
{
	int iTotalSize;
	int iNumber;
	PACKET_HEADER	eHeader;
	char strPacket[PACKET_SIZE];
}PACKET, *PPACKET;