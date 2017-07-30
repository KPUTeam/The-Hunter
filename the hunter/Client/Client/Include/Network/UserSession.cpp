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
	return Begin() && Accept(hListen); // 개체를 다시 시작하고 Accept 상태로 만든다.
									   // Accept를 하기 위해서는 ListenSocket이 필요하다.
									   // 이 값은 CZEROServerIocp의 멤버인 m_pListen의 GetSocket으로 받아올 수 있다.	
}
