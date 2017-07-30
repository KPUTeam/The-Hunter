#include "IdleState.h"

#include "Core/Dx11Scheduler.h"
#include "Component/Dx11Animation3D.h"
#include "GameObject/Dx11GameObject.h"
#include "Component/DX11FSM.h"

#include "ShakeHeadState.h"
#include "WalkState.h"

CIdleState::CIdleState()
{
	m_eState = AI_IDLE;
}

CIdleState::CIdleState(const CIdleState & script)
{
	*this = script;
}


CIdleState::~CIdleState()
{
}

void CIdleState::Enter()
{
	CDx11GameObject* pMeshObj = m_pGameObject->FindChildFromTag("pMeshObj");

	// 애니매이션 클립을 초기화 한다.
	CDx11Animation3D* pAnimation3D = (CDx11Animation3D*)pMeshObj->FindComponentFromType(CT_ANIMATION3D);
	pAnimation3D->ChangeAnimation("Idle");
	m_dClipTime = pAnimation3D->GetCilpTime("Idle");
	SAFE_RELEASE(pAnimation3D);
	SAFE_RELEASE(pMeshObj);

	DX11_GET_SINGLE(CDx11Scheduler)->CreateSchedule(this, &CIdleState::Exit, m_dClipTime*(rand()%3+1), SO_ONCE);
}

void CIdleState::Execute(float fTime)
{
}

CIdleState * CIdleState::Clone()
{
	return new CIdleState(*this);
}

int CIdleState::Exit(float fTime)
{
	// FSM을 찾는다.
	CDx11FSM* pFSM = (CDx11FSM*)m_pGameObject->FindComponentFromTag("FSM");
	// CWalkState* staet = pFSM->SetState<CWalkState>(m_pGameObject);

	CWalkState* pState = pFSM->SetState<CWalkState>(m_pGameObject);
	SAFE_RELEASE(pState);

	SAFE_RELEASE(pFSM);

	return 0;
}