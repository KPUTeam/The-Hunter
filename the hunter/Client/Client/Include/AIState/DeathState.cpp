#include "DeathState.h"

#include "Core/Dx11Scheduler.h"
#include "Component/Dx11Animation3D.h"
#include "GameObject/Dx11GameObject.h"
#include "Component/DX11FSM.h"

#include "ShakeHeadState.h"

// #include "WalkState.h"

CDeathState::CDeathState()
{
	m_eState = AI_IDLE;
}

CDeathState::CDeathState(const CDeathState & script)
{
	*this = script;
}


CDeathState::~CDeathState()
{
}

void CDeathState::Enter()
{
	CDx11GameObject* pMeshObj = m_pGameObject->FindChildFromTag("pMeshObj");

	// 애니매이션 클립을 초기화 한다.
	CDx11Animation3D* pAnimation3D = (CDx11Animation3D*)pMeshObj->FindComponentFromType(CT_ANIMATION3D);
	pAnimation3D->ChangeAnimation("Death");
	m_dClipTime = pAnimation3D->GetCilpTime("Death");
	SAFE_RELEASE(pAnimation3D);
	SAFE_RELEASE(pMeshObj);

	DX11_GET_SINGLE(CDx11Scheduler)->CreateSchedule(this, &CDeathState::Exit, m_dClipTime, SO_ONCE);
}

void CDeathState::Execute(float fTime)
{
}

CDeathState * CDeathState::Clone()
{
	return new CDeathState(*this);
}

int CDeathState::Exit(float fTime)
{
	
	m_pGameObject->Death();

	return 0;
}