#pragma once
#include "Include.h"

class CIocp
{
public:
	CIocp();
	virtual ~CIocp();

private:
	HANDLE			mIocpHandle; // IOCP ���� �� ���� �ڵ��� IOCP ����� �ϴ� ����
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

