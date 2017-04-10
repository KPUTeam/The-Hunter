#pragma once

#include <WinSock2.h>
#include <iostream>
#include <unordered_map>
#include <list>
#include <vector>
#include <mysql.h>
#include <process.h>

using namespace std;

#pragma comment(lib, "libmySQL.lib")
#pragma comment(lib, "ws2_32")

#define	PORT		9000
#define PACKET_SIZE	1024

#define DB_HOST "127.0.0.1"
#define DB_USER "root"
#define DB_PASS "qwer4321"
#define DB_NAME "The_Hunter"

enum PACKET_HEADER
{
	PH_NONE,
	PH_JOIN,
	PH_LOGIN,
	PH_LOGOUT,
	PH_MSG
};

enum TRANSFER_MODE
{
	TM_READ = 1,
	TM_WRITE
};

typedef struct _tagBuffer
{
	OVERLAPPED		tOverlapped;
	WSABUF			tWsaBuf;
	TRANSFER_MODE	eMode;
}BUFFER, *PBUFFER;

typedef struct _tagPacket
{
	PACKET_HEADER	eHeader;
	int	iSize;
	char strPacket[PACKET_SIZE];

	_tagPacket() :
		eHeader(PH_NONE),
		iSize(0)
	{
		memset(strPacket, 0, PACKET_SIZE);
	}
}PACKET, *PPACKET;

#define		ID_SIZE		64
#define		PASS_SIZE	64

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



#define SAFE_DELETE(p)		if(p)	{delete p; p = NULL; }

template <typename T>
void Safe_Delete_Map(T& p)
{
	T::iterator	iter = p.begin();
	T::iterator	iterEnd = p.end();

	while (iter != iterEnd)
	{
		SAFE_DELETE(iter->second);
		++iter;
	}

	p.clear();
}

template <typename T>
void Safe_Delete_VecList(T& p)
{
	T::iterator	iter = p.begin();
	T::iterator iterEnd = p.end();

	while (iter != iterEnd)
	{
		SAFE_DELETE(*iter);
		++iter;
	}

	p.clear();
}

#define DECLARE_SINGLE(Type)\
		private:\
			static Type*	m_pInst;\
		public:\
			static Type*	GetInst()\
			{\
				if(!m_pInst)\
					m_pInst = new Type;\
				return m_pInst;\
			}\
			static void DestroyInst()\
			{\
				SAFE_DELETE(m_pInst);\
			}\
		public:\
			Type();\
			~Type();\

#define DEFINITION_SINGLE(Type)	Type* Type::m_pInst = NULL;
#define GET_SINGLE(Type)	Type::GetInst()
#define DESTROY_SINGLE(Type)	Type::DestroyInst()

