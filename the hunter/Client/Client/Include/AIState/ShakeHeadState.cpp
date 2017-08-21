#include "ShakeHeadState.h"

#include "Core/Dx11Scheduler.h"
#include "Component/Dx11Animation3D.h"
#include "GameObject/Dx11GameObject.h"
#include "Component/DX11FSM.h"

#include "WalkState.h"
#include "IdleState.h"

CShakeHeadState::CShakeHeadState()
{
	m_eState = AI_SHAKEHEAD;
}

CShakeHeadState::CShakeHeadState(const CShakeHeadState & script)
{
}


CShakeHeadState::~CShakeHeadState()
{
}

void CShakeHeadState::Enter()
{
	CDx11GameObject* pBodyObj = m_pGameObject->FindChildFromTag("pMeshObj");

	// 애니매이션 클립을 초기화 한다.
	CDx11Animation3D* pAnimation3D = (CDx11Animation3D*)pBodyObj->FindComponentFromType(CT_ANIMATION3D);
	pAnimation3D->ChangeAnimation("Idle_Shake");
	m_dClipTime = pAnimation3D->GetCilpTime("Idle_Shake");
	SAFE_RELEASE(pAnimation3D);

	SAFE_RELEASE(pBodyObj);

	DX11_GET_SINGLE(CDx11Scheduler)->CreateSchedule(this, &CShakeHeadState::Exit, m_dClipTime, SO_ONCE);
}

void CShakeHeadState::Execute(float fTime)
{
}

int CShakeHeadState::Exit(float fTime)
{
	// FSM을 찾는다.
	CDx11FSM* pFSM = (CDx11FSM*)m_pGameObject->FindComponentFromTag("FSM");
	CWalkState* pState = pFSM->SetState<CWalkState>(m_pGameObject);
	//CIdleState* pState  = pFSM->SetState<CIdleState>(m_pGameObject);
	SAFE_RELEASE(pState);

	SAFE_RELEASE(pFSM);

	return 0;
}

CShakeHeadState * CShakeHeadState::Clone()
{
	return new CShakeHeadState(*this);
}
