#pragma once
#include "../Dx11Object.h"

DX11_BEGIN

class DX11_DLL CDx11Scheduler :
	public CDx11Object
{
	DX11_DECLARE_SINGLE(CDx11Scheduler)

private:
	typedef struct _tagScheduleInfo
	{
		function<int(float)>	func;
		float					fLimitTime;
		float					fTime;
		void*					pObj;
		SCHEDULE_OPTION			eOption;
	}SCHEDULEINFO, *PSCHEDULEINFO;

private:
	list<PSCHEDULEINFO>		m_Schedule;

public:
	bool Init();
	int Update(float fTime);
	bool DeleteSchedule(void* pObj);

public:
	template <typename T>
	bool CreateSchedule(T* pObj, int (T::*pFunc)(float), float fLimitTime,
		SCHEDULE_OPTION eOption = SO_ONCE)
	{
		PSCHEDULEINFO	pInfo = new SCHEDULEINFO;

		pInfo->func = bind(pFunc, pObj, placeholders::_1);
		pInfo->fLimitTime = fLimitTime;
		pInfo->pObj = pObj;
		pInfo->eOption = eOption;
		pInfo->fTime = 0.f;

		m_Schedule.push_back(pInfo);

		return true;
	}
};

DX11_END