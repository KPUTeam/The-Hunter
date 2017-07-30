#pragma once

#include "Include.h"

class CZEROCriticalSection
{
private:
	CRITICAL_SECTION	m_Sync;

public:
	CZEROCriticalSection(void)
	{
		InitializeCriticalSection(&m_Sync);
	}

	~CZEROCriticalSection(void)
	{
		DeleteCriticalSection(&m_Sync);
	}

	inline VOID Enter(VOID)
	{
		EnterCriticalSection(&m_Sync);
	}

	inline VOID Leave(VOID)
	{
		LeaveCriticalSection(&m_Sync);
	}
};
