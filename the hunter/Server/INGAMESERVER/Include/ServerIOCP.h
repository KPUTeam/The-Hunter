#pragma once
#include "Network\ZEROIocp.h"

class CServerIOCP :
	public CZEROIocp
{
public:
	CServerIOCP();
	~CServerIOCP();

protected:
	virtual VOID	OnIoRead(VOID* object, DWORD dataLength);
	virtual VOID	OnIoWrote(VOID* object, DWORD dataLength);
	virtual VOID	OnIoConnected(VOID* object);
	virtual VOID	OnIoDisconnected(VOID* object);

protected:
	void UserLogin(class CUserSession* pSession, BYTE* pPacket);
	void UserPos(class CUserSession* pSession, BYTE* pPacket);
	void MonsterPos(class CUserSession* pSession, BYTE* pPacket);
	void UserMove(class CUserSession* pSession, BYTE* pPacket);
	void UserRotate(class CUserSession* pSession, BYTE* pPacket);
	void UserHeadRotate(class CUserSession* pSession, BYTE* pPacket);
	void UserShoot(class CUserSession* pSession, BYTE* pPacket);
};

