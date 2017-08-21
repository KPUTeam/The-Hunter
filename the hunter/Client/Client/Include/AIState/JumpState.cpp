#include "JumpState.h"

#include "Core/Dx11Scheduler.h"
#include "Component/Dx11Animation3D.h"
#include "GameObject/Dx11GameObject.h"
#include "Component/DX11FSM.h"
#include "Component/Dx11Transform.h"
#include "RunState.h"
#include "../ObjScript/Monster/MonsterSC.h"

CJumpState::CJumpState()
{
	m_eState = AI_JUMP;
}

CJumpState::CJumpState(const CJumpState & script)
{
}

//CJumpState::CJumpState(const CJumpState & script):
//{
//	*this = script;
//}


CJumpState::~CJumpState()
{
}

void CJumpState::Enter()
{
	CDx11GameObject* pBodyObj = m_pGameObject->FindChildFromTag("pMeshObj");

	// 애니매이션 클립을 초기화 한다.
	CDx11Animation3D* pAnimation3D = (CDx11Animation3D*)pBodyObj->FindComponentFromType(CT_ANIMATION3D);
	pAnimation3D->ChangeAnimation("Jump");
	m_dClipTime = pAnimation3D->GetCilpTime("Jump");
	SAFE_RELEASE(pAnimation3D);
	SAFE_RELEASE(pBodyObj);


	DX11_GET_SINGLE(CDx11Scheduler)->CreateSchedule(this, &CJumpState::Exit, m_dClipTime, SO_ONCE);
}

void CJumpState::Execute(float fTime)
{
}

CJumpState * CJumpState::Clone()
{
	return new CJumpState(*this);
}

int CJumpState::Exit(float fTime)
{
	//// 스크립트를 찾아서 점프를 끈다.
	//CMonsterSC* pSc = (CMonsterSC*)m_pGameObject->FindComponentFromTag("MonsterSC");
	//pSc->SetJumpFlag(false);
	//SAFE_RELEASE(pSc);

	CDx11FSM* pFSM = (CDx11FSM*)m_pGameObject->FindComponentFromType(CT_FSM);
	CRunState* pState = pFSM->SetState<CRunState>(m_pGameObject);
	SAFE_RELEASE(pState);
	SAFE_RELEASE(pFSM);

	return 0;
}
