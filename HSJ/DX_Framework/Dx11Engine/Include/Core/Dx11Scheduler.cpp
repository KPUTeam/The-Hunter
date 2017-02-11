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
		(*iter)->fTime += fTime;

		if ((*iter)->fTime >= (*iter)->fLimitTime)
		{
			(*iter)->fTime -= (*iter)->fLimitTime;
			(*iter)->func(fTime);

			if ((*iter)->eOption == SO_ONCE)
			{
				SAFE_DELETE((*iter));
				iter = m_Schedule.erase(iter);
			}

			else
				++iter;
		}

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
		if ((*iter)->pObj == pObj)
		{
			SAFE_DELETE((*iter));
			m_Schedule.erase(iter);
			return true;
		}
	}

	return false;
}
