#pragma once
#include "../Include.h"
#include "CriticalSection.h"

template <class T>
class CMultiThreadSync
{
	friend class CThreadSync;

private:
	static CCriticalSection mSync; // 실제 CRITICAL_SECTION 개체이다.

public:
	class CThreadSync
	{
	public:
		CThreadSync()
		{
			T::mSync.Enter();	// 해당 클래스의 동기화 객체를 Enter한다.
		}

		~CThreadSync()
		{
			T::mSync.Leave();	// 해당 클래스의 동기화 객체를 Leave한다.
		}
	};
};

template <class T>
CCriticalSection CMultiThreadSync<T>::mSync;

// 어떤 객체가 CMultiThreaSync 클래스를 상속받으면 다중 스레드에 대해서 안전하게 된다는 뜻이다.
// 다중 스레드에 대해서 안전한 클래스를 만들기 위해서는
// template <class T>
// class CCircularQueue2 : public CMultiThreadSync<CCircularQueue> 
// 이렇게 클래스를 선언하면 된다.
// 그리고 동기화를 해야 하는 함수나 지역내 CThreadSync Sync 라는 형태로 동기화 변수를 선언해주면 된다.
//BOOL Push(T data)
//{
//	CThreadSync Sync; // 동기화 변수
//	DWORD TempTail = (mQueueTail + 1) % MAX_QUEUE_LENGTH;
//	if (TempTail == mQueueHead)
//		return FALSE;
//
//	CopyMemory(&mQueue[TempTail], &data, sizeof(T));
//	mQueueTail = TempTail;
//	return TRUE;
//}
// 위와 같이 사용하게 되면 Push라는 함수가 끝날 떄까지 다중 스레드 동기화가 이루어지게 된다.
// 즉, 생성자를 이용한 다중 스레드 동기화이다.