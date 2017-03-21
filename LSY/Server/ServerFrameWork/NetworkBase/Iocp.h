#pragma once
#include "Include.h"

class CIocp
{
public:
	CIocp();
	virtual ~CIocp();

private:
	HANDLE			mIocpHandle; // IOCP 생성 및 소켓 핸들의 IOCP 등록을 하는 역할
	DWORD			mWorkerThreadCount;

	vector<HANDLE>	mWorkerThreadVector;

	HANDLE			mStartUpEventHandle;

protected:
	virtual void	OnIoRead(void* object, DWORD dataLength)	= 0;
	virtual void	OnIoWrite(void* object, DWORD dataLength)	= 0;
	virtual void	OnIoConnected(void* object)					= 0;
	virtual void	OnIoDisconnected(void* object)				= 0;

public:
	bool	Begin();
	bool	End();
	bool	RegisterSocketToIocp(SOCKET socket, ULONG_PTR completionKey);

	void	WorkerThreadCallback();
};

