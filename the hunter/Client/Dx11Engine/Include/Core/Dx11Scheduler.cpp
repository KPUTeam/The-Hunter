#include "Dx11Scheduler.h"

DX11_USING

DX11_SINGLE_DEFINITION(CDx11Scheduler)

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

		// �ش� �������� �ð��� ��� �����ش�.
		(*iter)->fTime += fTime;

		// �ش� �������� ������ �ִ� ���� �ð��� �ʰ����� ��,
		if ((*iter)->fTime >= (*iter)->fLimitTime)
		{
			// �ð��� 0���� �ʱ�ȭ�ϰ� �Լ��� ����
			(*iter)->fTime -= (*iter)->fLimitTime;
			(*iter)->func(fTime);

			// ��ȸ���� ���� �������� �ѹ� ���������� ����
			if ((*iter)->eOption == SO_ONCE)
			{
				SAFE_DELETE((*iter));
				iter = m_Schedule.erase(iter);
			}
			// ������ ���� ���� ������� �Ѿ
			else
				++iter;
		}
		// ���ѽð��� �������� �������� ���� ������� �Ѿ
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
		// �ش� ������� ���ٸ� �����ϰ� true�� ����
		if ((*iter)->pObj == pObj)
		{
			SAFE_DELETE((*iter));
			m_Schedule.erase(iter);
			return true;
		}
	}
	return false;
}
