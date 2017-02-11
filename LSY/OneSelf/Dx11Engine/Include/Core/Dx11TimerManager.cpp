#include "Dx11TimerManager.h"
#include "Dx11Timer.h"

DX11_USING

DX11_DEFINITION_SINGLE(CDx11TimerManager)

CDx11TimerManager::CDx11TimerManager()
{
	SetTypeName<CDx11TimerManager>();
}

CDx11TimerManager::~CDx11TimerManager()
{
	Safe_Release_Map(m_mapTimer);
}

bool CDx11TimerManager::Init()
{
	CDx11Timer*	pTimer = CreateTimer("Main");

	SAFE_RELEASE(pTimer);

	return true;
}

CDx11Timer * CDx11TimerManager::CreateTimer(const string & strKey)
{
	CDx11Timer*	pTimer = FindTimer(strKey);

	if (pTimer)
		return pTimer;

	pTimer = new CDx11Timer;

	if (!pTimer->Init())
	{
		SAFE_RELEASE(pTimer);
		return NULL;
	}

	pTimer->AddRef();

	m_mapTimer.insert(make_pair(strKey, pTimer));

	return pTimer;
}

CDx11Timer * CDx11TimerManager::FindTimer(const string & strKey)
{
	unordered_map<string, class CDx11Timer*>::iterator	iter = m_mapTimer.find(strKey);

	if (iter == m_mapTimer.end())
		return NULL;

	iter->second->AddRef();

	return iter->second;
}
