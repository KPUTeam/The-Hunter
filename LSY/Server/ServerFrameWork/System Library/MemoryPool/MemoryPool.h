#pragma once
#include "../Include.h"

template <class T, int ALLOC_BLOCK_SIZE = 50>
class CMemoryPool
{
private:
	static UCHAR*	mFreePointer; // MemoryPool을 관리하는 변수


public:
	static VOID* operator new(std::size_t allocLength) //new를 했을 경우 호출된다.
	{
		assert(sizeof(T) == allocLength);
		assert(sizeof(T) >= sizeof(UCHAR*));

		// 만약 더 이상 할당할 수 있는 공간이 없을 경우 새로 할당한다.
		if (!mFreePointer)
			allocBlock();
		UCHAR* ReturnPointer = mFreePointer;

		// mFreePointer에는 리턴하는 블록 앞에 4바이트로 있던 주소가 들어간다.
		mFreePointer = *reinterpret_cast<UCHAR**>(ReturnPointer);

		return ReturnPointer;
	}

	static VOID operator delete(VOID* deletePointer) // delete를 했을 때
	{
		// delete된 블록의 Next에 현재 mFreePointer의 주소를 넣어준다.
		*reinterpret_cast<UCHAR**>(deletePointer) = mFreePointer;
		
		// mFreePointer를 업데이트한다.
		mFreePointer = static_cast<UCHAR*>(deletePointer);
	}

private:
	static VOID	allocBlock()
	{
		// mFreePointer에 사용할 크기의 메모리를 할당한다.
		mFreePointer = new UCHAR[sizeof(T) * ALLOC_BLOCK_SIZE];

		// 할당한 메모리의 첫 블록의 포인터를 Current에 넣는다.
		UCHAR** Current = reinterpret_cast<UCHAR**>(mFreePointer);
		
		// 할당된 메모리의 첫 블록이다.
		UCHAR* Next = mFreePointer;

		for (int i = 0; i < ALLOC_BLOCK_SIZE - 1; ++i)
		{
			Next += sizeof(T);		// 다음 블록을 계산한다.
			*Current = Next;		// 할당된 메모리 앞 4바이트에 다음 블록의 주소를 넣는다.
			Current = reinterpret_cast<UCHAR**>(Next);
		}

		*Current = 0;				// 마지막일 경우 앞에 4바이트는 NULL이다.
	}

protected:
	~CMemoryPool() {}
};

template <class T, int ALLOC_BLOCK_SIZE>
UCHAR* CMemoryPool<T, ALLOC_BLOCK_SIZE>::mFreePointer;

// 서버에서 말하는 메모리 관리라고 하는 것은 정해진 서버 머신 내의 
// 메모리를 최대한 활용해서 최상의 속도가 나오도록 하는것이 목적이다.
// 가장 많이 사용되는 기법들이 MemoryPool, Gabage Collection, Static Memory등이다.
// MemoryPool은 응용 프로그램에서 사용하는 메모리를 미리 할당해 놓고
// 사용자가 new 연산자를 사용하여 개체를 만들 때 미리 할당된 메모리를 제공해주는 형태이다.
// 기본적으로 미리 할당해 놓는다는 개념은 Garbage Collection과 동일하지만,
// MemoryPool의 경우는 new연산자와 delete연산자는 항상 쌍으로 이루어져야 한다.