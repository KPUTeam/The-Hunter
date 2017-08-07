#pragma once
#include "Network\ZEROPacketSession.h"
#include "Network\ZEROStream.h"
#include "ServerIOCP.h"
#include "NetworkManager.h"
#include "UserScript.h"

class CUserSession	:
	public CZEROPacketSession
{
public:
	CUserSession();
	~CUserSession();

protected:
	BOOL	m_bConnect;
	INT64	m_iSerialNumber;

	Vec3	m_Pos;
	Vec3	m_Scale;
	Vec3	m_Rot;

public:
	void SetConnected(BOOL bConnect);
	void SetSerialNumber(INT64 iSerialNumber);

public:
	void SetUserPos(Vec3 vPos);
	void SetUserScale(Vec3 vScale);
	void SetUserRotation(Vec3 vRot);

	Vec3 GetUserPos();
	Vec3 GetUserScale();
	Vec3 GetUserRotation();

public:
	INT64	GetSerialNumber() const;

public:
	BOOL	Begin(VOID);
	BOOL	End(VOID);
	BOOL	Restart(SOCKET hListen);
};

