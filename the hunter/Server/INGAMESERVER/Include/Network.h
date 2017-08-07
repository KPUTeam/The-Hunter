#pragma once

#include "Network\Include.h"

#ifdef _DEBUG
#pragma comment(lib, "Dx11Engine_Debug")
#else
#pragma comment(lib, "Dx11Engine")
#endif

#pragma comment(lib, "libfbxsdk-md")

#if defined(DEBUG) | defined(_DEBUG)
#pragma comment(lib, "ZEROLowLib.lib")
#else
#pragma comment(lib, "ZEROLowLib")
#endif

#define ID_SIZE 64
#define PASS_SIZE 64
#define PORT 9001
#define MAX_USER 5
#define PACKET_SIZE 1024

enum SERIAL_OBJECT_TYPE
{
	SOT_USER
};

enum PACKET_HEADER
{
	PH_NONE,
	PH_CONNECT,
	PH_DISCONNECT,
	PH_LOGIN,
	PH_LOGIN_FAILED,
	PH_LOGIN_SUCCEEDED,
	PH_MY_INIT,
	PH_USER_LOGIN,
	PH_MONSTER_POS,
	PH_CONNECTED_USER,
	PH_START_GAME,
	PH_MY_MOVE,
	PH_MY_MOVE_SUCCEEDED,
	PH_MY_ROTATE,
	PH_MY_ROTATE_SUCCEEDED,
	PH_MY_HEAD_ROTATE,
	PH_MY_HEAD_ROTATE_SUCCEEDED,
	PH_MY_SHOOT,
	PH_MY_SHOOT_SUCCEEDED
};

typedef struct _tagMember
{
	char		strID[ID_SIZE];
	char		strPass[PASS_SIZE];
	bool		bLogin;
	SOCKET		hSocket;

	_tagMember()
	{
		memset(strID, 0, ID_SIZE);
		memset(strPass, 0, PASS_SIZE);
		bLogin = false;
	}
}MEMBER, *PMEMBER;

