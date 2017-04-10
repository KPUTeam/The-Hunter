#include "Dx11Scheduler.h"

DX11_USING

DX11_DEFINITION_SINGLE(CDx11Scheduler)

CDx11Scheduler::CDx11Scheduler()
{
}


CDx11Scheduler::~CDx11Scheduler()
{
	Safe_Delete_VecList(m_Schedule);
}

bool CDx11Scheduler::Init()
{
	return true;
}

int CDx11Scheduler::Update(float fTime)
{
	list<PSCHEDULEINFO>::iterator	iter;
	list<PSCHEDULEINFO>::iterator	iterEnd = m_Schedule.end();

	for (iter = m_Schedule.begin(); iter != iterEnd;)
	{
		// 해당 스케쥴의 시간을 계속 더해준다.
		(*iter)->fTime += fTime;

		// 해당 스케쥴이 가지고 있는 제한 시간을 초과했을 때,
		if ((*iter)->fTime >= (*iter)->fLimitTime)
		{
			// 시간을 0으로 초기화하고 함수를 실행
			(*iter)->fTime -= (*iter)->fLimitTime;
			(*iter)->func(fTime);

			// 일회용일 때는 스케쥴을 한번 실행했으니 삭제
			if ((*iter)->eOption == SO_ONCE)
			{
				SAFE_DELETE((*iter));
				iter = m_Schedule.erase(iter);
			}
			// 루프일 때는 다음 스케쥴로 넘어감
			else
				++iter;
		}
		// 제한시간에 도달하지 못했으면 다음 스케쥴로 넘어감
		else
			++iter;
	}
	return 0;
}

bool CDx11Scheduler::DeleteSchedule(void * pObj)
{
	list<PSCHEDULEINFO>::iterator	iter;
	list<PSCHEDULEINFO>::iterator	iterEnd = m_Schedule.end();

	for (iter = m_Schedule.begin(); iter != iterEnd; ++iter)
	{
		// 해당 스케쥴과 같다면 삭제하고 true를 리턴
		if ((*iter)->pObj == pObj)
		{
			SAFE_DELETE((*iter));
			m_Schedule.erase(iter);
			return true;
		}
	}
	return false;
}
