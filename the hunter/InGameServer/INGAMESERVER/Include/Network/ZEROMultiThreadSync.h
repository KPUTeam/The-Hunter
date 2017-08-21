#pragma once

#include "ZEROCriticalSection.h"

template <typename T>
class CZEROMultiThreadSync
{

friend class CZEROThreadSync;

private:
	static CZEROCriticalSection m_Sync;

public:
	class CZEROThreadSync
	{
	public:
		CZEROThreadSync(VOID)
		{
			T::m_Sync.Enter();
		}

		~CZEROThreadSync(VOID)
		{
			T::m_Sync.Leave();
		}
	};
};

template <typename T>
CZEROCriticalSection CZEROMultiThreadSync<T>::m_Sync;
