#include "UserSession.h"



CUserSession::CUserSession()
{
}


CUserSession::~CUserSession()
{
}

void CUserSession::SetConnected(BOOL bConnect)
{
	m_bConnect = bConnect;
}

void CUserSession::SetSerialNumber(INT64 iSerialNumber)
{
	m_iSerialNumber = iSerialNumber;
}

void CUserSession::SetUserPos(Vec3 vPos)
{
	m_Pos = vPos;
}

void CUserSession::SetUserScale(Vec3 vScale)
{
	m_Scale = vScale;
}

void CUserSession::SetUserRotation(Vec3 vRot)
{
	m_Rot = vRot;
}

Vec3 CUserSession::GetUserPos()
{
	return m_Pos;
}

Vec3 CUserSession::GetUserScale()
{
	return m_Scale;
}

Vec3 CUserSession::GetUserRotation()
{
	return m_Rot;
}

INT64 CUserSession::GetSerialNumber() const
{
	return m_iSerialNumber;
}

BOOL CUserSession::Begin(VOID)
{
	m_bConnect = FALSE;
	return CZEROPacketSession::Begin();
}

BOOL CUserSession::End(VOID)
{
	return CZEROPacketSession::End();
}

BOOL CUserSession::Restart(SOCKET hListen)
{
	CZEROThreadSync	Sync;
	End();
	return Begin() && Accept(hListen);
}
