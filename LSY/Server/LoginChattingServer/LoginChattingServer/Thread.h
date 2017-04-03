#pragma once

#include "Global.h"

class CThread
{
public:
	CThread();
	~CThread();

private:
	HANDLE			m_hThread;
	bool			m_bLoop;
	class CIocp*	 m_pIocp;

public:
	bool Create(class CIocp* pIocp);
	void Run();
	void End();

public:
	static unsigned int __stdcall ThreadFunc(void* pArg);
};

