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
	return Begin() && Accept(hListen); // ��ü�� �ٽ� �����ϰ� Accept ���·� �����.
									   // Accept�� �ϱ� ���ؼ��� ListenSocket�� �ʿ��ϴ�.
									   // �� ���� CZEROServerIocp�� ����� m_pListen�� GetSocket���� �޾ƿ� �� �ִ�.	
}
