#pragma once
#include "ZEROMultiThreadSync.h"

class CZEROIocp: public CZEROMultiThreadSync<CZEROIocp>
{
private:
	HANDLE				m_IocpHandle;
	DWORD				m_WorkerThreadCount;

	vector<HANDLE>		m_WorkerThreadVector;

	HANDLE				m_StartupEventHandle;

public:
	CZEROIocp(void);
	virtual ~CZEROIocp(void);

protected:
	virtual VOID		OnIoRead(VOID *object, DWORD dataLength)	= 0;
	virtual VOID		OnIoWrote(VOID *object, DWORD dataLength)	= 0;
	virtual VOID		OnIoConnected(VOID *object)					= 0;
	virtual VOID		OnIoDisconnected(VOID *object)				= 0;

public:
	BOOL				Begin(VOID);
	BOOL				End(VOID);

	BOOL				RegisterSocketToIocp(SOCKET socket, ULONG_PTR completionKey);

	VOID				WorkerThreadCallback(VOID);
};
