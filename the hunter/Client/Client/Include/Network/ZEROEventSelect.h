#pragma once

#include "Include.h"

class CZEROEventSelect
{
public:
	CZEROEventSelect(void);
	virtual ~CZEROEventSelect(void);

private:
	HANDLE			m_SelectEventHandle;
	HANDLE			m_StartupEventHandle;
	HANDLE			m_DestroyEventHandle;

	HANDLE			m_SelectThreadHandle;

	SOCKET			m_Socket;

protected:
	virtual VOID	OnIoRead(VOID)			= 0;
	//virtual VOID	OnIoWrote(VOID)			= 0;
	virtual VOID	OnIoConnected(VOID)		= 0;
	virtual VOID	OnIoDisconnected(VOID)	= 0;

public:
	BOOL			Begin(SOCKET socket);
	BOOL			End(VOID);

	VOID			SelectThreadCallback(VOID);

};
