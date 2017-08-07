#include "Logic.h"
#include <process.h>
#include "Dx11Core.h"
#include "NetworkManager.h"
#include "Core/Dx11TimerManager.h"
#include "Core/Dx11Timer.h"
#include "Network\ZEROStream.h"
#include "NetworkManager.h"
#include "GameObject/Dx11GameObject.h"
#include "MonsterManager.h"


CLogic::CLogic()
	: m_hThread(NULL)
{
}


CLogic::~CLogic()
{
	if (m_hThread)
	{
		m_bLoop = false;
		WaitForSingleObject(m_hThread, INFINITE);
		CloseHandle(m_hThread);
	}
}

bool CLogic::Init()
{
	CDx11Core::GetInst()->Init(0, RT_HD, true);

	m_bLoop = true;
	m_fGlobalTime = 0.f;
	m_fStartTime = 0.f;

	DX11_GET_SINGLE(CDx11TimerManager)->Init();

//	if (!CMonsterManager::GetInst()->Init())	return false;

	m_hThread = (HANDLE)_beginthreadex(0, 0, CLogic::LogicThreadFunc, this, 0, 0);

	return true;
}

void CLogic::Run()
{
	while (m_bLoop)
	{
		CDx11Timer* pTimer = DX11_GET_SINGLE(CDx11TimerManager)->FindTimer("MainTimer");
		pTimer->Update();
		float fFrameTime = pTimer->GetDeltaTime();
		m_fGlobalTime += fFrameTime;
	//	cout << m_fGlobalTime << endl;

	//	if (CNetworkManager::GetInst()->GetConnectCount() == 2)
	//	{
	//		m_fStartTime += fFrameTime;
	//		cout << m_fStartTime << endl;
	//		if (m_fStartTime > 5.f) {
	//			CNetworkManager::GetInst()->SendGameStart();
	//		}
	//	}

		SAFE_RELEASE(pTimer);
	}
}

UINT CLogic::LogicThreadFunc(void * pArg)
{
	CLogic* pLogic = (CLogic*)pArg;
	pLogic->Run();
	return 0;
}
