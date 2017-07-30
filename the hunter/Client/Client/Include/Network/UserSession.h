#pragma once
#include "ZEROPacketSession.h"

class CUserSession :
	public CZEROPacketSession
{
public:
	CUserSession();
	~CUserSession();

protected:
	BOOL	m_bConnect;
	INT64	m_iSerialNumber;

public:
	void SetConnected(BOOL bConnect);
	void SetSerialNumber(INT64 iSerialNumber);

public:
	INT64	GetSerialNumber() const;

public:
	BOOL Begin(VOID);
	BOOL End(VOID);
	BOOL Restart(SOCKET hListen);
};

