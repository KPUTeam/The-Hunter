#pragma once
#include "../Include.h"

template <class T, int ALLOC_BLOCK_SIZE = 50>
class CMemoryPool
{
private:
	static UCHAR*	mFreePointer; // MemoryPool�� �����ϴ� ����


public:
	static VOID* operator new(std::size_t allocLength) //new�� ���� ��� ȣ��ȴ�.
	{
		assert(sizeof(T) == allocLength);
		assert(sizeof(T) >= sizeof(UCHAR*));

		// ���� �� �̻� �Ҵ��� �� �ִ� ������ ���� ��� ���� �Ҵ��Ѵ�.
		if (!mFreePointer)
			allocBlock();
		UCHAR* ReturnPointer = mFreePointer;

		// mFreePointer���� �����ϴ� ��� �տ� 4����Ʈ�� �ִ� �ּҰ� ����.
		mFreePointer = *reinterpret_cast<UCHAR**>(ReturnPointer);

		return ReturnPointer;
	}

	static VOID operator delete(VOID* deletePointer) // delete�� ���� ��
	{
		// delete�� ����� Next�� ���� mFreePointer�� �ּҸ� �־��ش�.
		*reinterpret_cast<UCHAR**>(deletePointer) = mFreePointer;
		
		// mFreePointer�� ������Ʈ�Ѵ�.
		mFreePointer = static_cast<UCHAR*>(deletePointer);
	}

private:
	static VOID	allocBlock()
	{
		// mFreePointer�� ����� ũ���� �޸𸮸� �Ҵ��Ѵ�.
		mFreePointer = new UCHAR[sizeof(T) * ALLOC_BLOCK_SIZE];

		// �Ҵ��� �޸��� ù ����� �����͸� Current�� �ִ´�.
		UCHAR** Current = reinterpret_cast<UCHAR**>(mFreePointer);
		
		// �Ҵ�� �޸��� ù ����̴�.
		UCHAR* Next = mFreePointer;

		for (int i = 0; i < ALLOC_BLOCK_SIZE - 1; ++i)
		{
			Next += sizeof(T);		// ���� ����� ����Ѵ�.
			*Current = Next;		// �Ҵ�� �޸� �� 4����Ʈ�� ���� ����� �ּҸ� �ִ´�.
			Current = reinterpret_cast<UCHAR**>(Next);
		}

		*Current = 0;				// �������� ��� �տ� 4����Ʈ�� NULL�̴�.
	}

protected:
	~CMemoryPool() {}
};

template <class T, int ALLOC_BLOCK_SIZE>
UCHAR* CMemoryPool<T, ALLOC_BLOCK_SIZE>::mFreePointer;

// �������� ���ϴ� �޸� ������� �ϴ� ���� ������ ���� �ӽ� ���� 
// �޸𸮸� �ִ��� Ȱ���ؼ� �ֻ��� �ӵ��� �������� �ϴ°��� �����̴�.
// ���� ���� ���Ǵ� ������� MemoryPool, Gabage Collection, Static Memory���̴�.
// MemoryPool�� ���� ���α׷����� ����ϴ� �޸𸮸� �̸� �Ҵ��� ����
// ����ڰ� new �����ڸ� ����Ͽ� ��ü�� ���� �� �̸� �Ҵ�� �޸𸮸� �������ִ� �����̴�.
// �⺻������ �̸� �Ҵ��� ���´ٴ� ������ Garbage Collection�� ����������,
// MemoryPool�� ���� new�����ڿ� delete�����ڴ� �׻� ������ �̷������ �Ѵ�.