//#pragma once
//
//class CZEROMemoryPool
//{
//public:
//	CZEROMemoryPool(void);
//	~CZEROMemoryPool(void);
//};

#pragma once

#include "ZEROMultiThreadSync.h"

template <class T, int ALLOC_BLOCK_SIZE = 50>
class CZEROMemoryPool : public CZEROMultiThreadSync<T>
{
private:
	static UCHAR	*m_FreePointer;

public:
	static VOID* operator new(std::size_t allocLength)
	{
		CZEROThreadSync Sync;		

		assert(sizeof(T) == allocLength);
		assert(sizeof(T) >= sizeof(UCHAR*));

		if (!m_FreePointer)
			allocBlock();

		UCHAR *ReturnPointer = m_FreePointer;
		m_FreePointer = *reinterpret_cast<UCHAR**>(ReturnPointer);

		return ReturnPointer;
	}

	static VOID	operator delete(VOID* deletePointer)
	{
		CThreadSync Sync;

		*reinterpret_cast<UCHAR**>(deletePointer) = m_FreePointer;
		m_FreePointer = static_cast<UCHAR*>(deletePointer);
	}

private:
	static VOID	allocBlock()
	{
		m_FreePointer		= new UCHAR[sizeof(T) * ALLOC_BLOCK_SIZE];

		UCHAR **Current = reinterpret_cast<UCHAR **>(m_FreePointer);
		UCHAR *Next		= m_FreePointer;

		for (INT i=0;i<ALLOC_BLOCK_SIZE-1;++i)
		{
			Next		+= sizeof(T);
			*Current	= Next;
			Current		= reinterpret_cast<UCHAR**>(Next);
		}

		*Current = 0;
	}



protected:
	~CZEROMemoryPool()
	{
	}
};

template <class T, int ALLOC_BLOCK_SIZE>
UCHAR* CZEROMemoryPool<T, ALLOC_BLOCK_SIZE>::m_FreePointer;
