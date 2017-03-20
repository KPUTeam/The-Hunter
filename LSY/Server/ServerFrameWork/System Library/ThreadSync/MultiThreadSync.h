#pragma once
#include "../Include.h"
#include "CriticalSection.h"

template <class T>
class CMultiThreadSync
{
	friend class CThreadSync;

private:
	static CCriticalSection mSync; // ���� CRITICAL_SECTION ��ü�̴�.

public:
	class CThreadSync
	{
	public:
		CThreadSync()
		{
			T::mSync.Enter();	// �ش� Ŭ������ ����ȭ ��ü�� Enter�Ѵ�.
		}

		~CThreadSync()
		{
			T::mSync.Leave();	// �ش� Ŭ������ ����ȭ ��ü�� Leave�Ѵ�.
		}
	};
};

template <class T>
CCriticalSection CMultiThreadSync<T>::mSync;

// � ��ü�� CMultiThreaSync Ŭ������ ��ӹ����� ���� �����忡 ���ؼ� �����ϰ� �ȴٴ� ���̴�.
// ���� �����忡 ���ؼ� ������ Ŭ������ ����� ���ؼ���
// template <class T>
// class CCircularQueue2 : public CMultiThreadSync<CCircularQueue> 
// �̷��� Ŭ������ �����ϸ� �ȴ�.
// �׸��� ����ȭ�� �ؾ� �ϴ� �Լ��� ������ CThreadSync Sync ��� ���·� ����ȭ ������ �������ָ� �ȴ�.
//BOOL Push(T data)
//{
//	CThreadSync Sync; // ����ȭ ����
//	DWORD TempTail = (mQueueTail + 1) % MAX_QUEUE_LENGTH;
//	if (TempTail == mQueueHead)
//		return FALSE;
//
//	CopyMemory(&mQueue[TempTail], &data, sizeof(T));
//	mQueueTail = TempTail;
//	return TRUE;
//}
// ���� ���� ����ϰ� �Ǹ� Push��� �Լ��� ���� ������ ���� ������ ����ȭ�� �̷������ �ȴ�.
// ��, �����ڸ� �̿��� ���� ������ ����ȭ�̴�.