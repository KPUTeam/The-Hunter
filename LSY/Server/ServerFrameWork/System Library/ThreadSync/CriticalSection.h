#pragma once
#include "../Include.h"

class CCriticalSection
{
private:
	CRITICAL_SECTION	mSync; // CRITICAL_SECTION °³Ã¼

public:
	CCriticalSection()
	{
		InitializeCriticalSection(&mSync);
	}

	~CCriticalSection()
	{
		DeleteCriticalSection(&mSync);
	}

	inline void Enter()
	{
		EnterCriticalSection(&mSync);
	}

	inline void Leave()
	{
		LeaveCriticalSection(&mSync);
	}
};