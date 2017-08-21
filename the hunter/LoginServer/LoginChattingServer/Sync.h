#pragma once

#include "Global.h"

class CSync
{
private:
	CRITICAL_SECTION*	m_pCrt;

public:
	CSync(CRITICAL_SECTION* pCrt)
	{
		m_pCrt = pCrt;
		EnterCriticalSection(m_pCrt);
	}

	~CSync()
	{
		LeaveCriticalSection(m_pCrt);
	}
};

