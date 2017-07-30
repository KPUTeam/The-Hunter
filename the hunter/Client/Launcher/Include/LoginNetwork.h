#pragma once
#include "NetworkHeader\ZEROEventSelect.h"

class CLoginNetwork :
	public CZEROEventSelect
{
public:
	CLoginNetwork();
	~CLoginNetwork();

private:
	class CMainFrame*	m_pMainFrame;

public:
	void SetFrame(class CMainFrame* pFrame);

public:
	virtual void OnIoRead();
	virtual void OnIoConnected();
	virtual void OnIoDisconnected();
};

