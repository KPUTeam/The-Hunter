#include "RunState.h"

#include "Core/Dx11Scheduler.h"
#include "Component/Dx11Animation3D.h"
#include "GameObject/Dx11GameObject.h"
#include "Component/DX11FSM.h"
#include "Scene/Dx11Layer.h"
#include "Component/Dx11Transform.h"

#include "../ObjScript/Monster/MonsterSC.h"
#include "WalkState.h"
#include "AttackState.h"

CRunState::CRunState()
{
	m_eState = AI_RUN;
}

CRunState::CRunState(const CRunState & script)
{
	*this = script;
}


CRunState::~CRunState()
{
}

void CRunState::Enter()
{
	CDx11GameObject* pBodyObj = m_pGameObject->FindChildFromTag("pMeshObj");

	// 애니매이션 클립을 초기화 한다.
	CDx11Animation3D* pAnimation3D = (CDx11Animation3D*)pBodyObj->FindComponentFromType(CT_ANIMATION3D);
	pAnimation3D->ChangeAnimation("Run");
	m_dClipTime = pAnimation3D->GetCilpTime("Run");
	SAFE_RELEASE(pAnimation3D);

	SAFE_RELEASE(pBodyObj);
}

void CRunState::Execute(float fTime)
{
	//
	CMonsterSC* pSc = (CMonsterSC*)m_pGameObject->FindComponentFromTag("pMonsterSC");
	CDx11GameObject* pTarget =  pSc->GetTarget();
	CDx11GameObject* pBodyObj = pTarget->FindChildFromTag("pBodyObj");

	CDx11Transform* pTargetTr = pBodyObj->GetTransform();

	Vec3 vDest = pTargetTr->GetWorldPos();	vDest.y = 0;

	SAFE_RELEASE(pBodyObj);
	SAFE_RELEASE(pTargetTr);
	SAFE_RELEASE(pTarget);
	SAFE_RELEASE(pSc);


	// 내 위치를 받아온다.
	CDx11Transform* pTr = m_pGameObject->GetTransform();
	Vec3 vPos = pTr->GetWorldPos();	vPos.y = 0;

	// 방향을 구한다.
	Vec3 vDir = vDest - vPos;

	// 너무 멀면 타겟을 잃어버린다.
	if (vDir.Length() > 25) {
		m_vDir = vDir;
		Exit(fTime);
		SAFE_RELEASE(pTr);
		return;
	}

	// 가까우면 공격을 한다.
	if (vDir.Length() <1.5) {
		m_vDir = vDir;
		Exit(fTime);
		SAFE_RELEASE(pTr);
		return;
	}

	vDir = vDir.Normalize();

	Vec3 MyAxisZ = pTr->GetWorldAxis(AXIS_X);

	// 방향과 내 Z 축을 내적한다.
	float fAngle = vDir.Dot(MyAxisZ);

	// 방향과 내 Z 축을 외적한다.
	//Vec3 vAxis = vDir.Cross(MyAxisZ);

	// 회전시킨다.
	Vec3 vAxis = pTr->GetWorldAxis(AXIS_Y);
	pTr->RotateWorld(fAngle, fTime*2, vAxis);

	// 일정한 속도록 움직인다.
	pTr->MoveWorld(2.8, fTime, AXIS_Z);

	SAFE_RELEASE(pTr);
}

CRunState * CRunState::Clone()
{
	return new CRunState(*this);
}

int CRunState::Exit(float fTime)
{
	if (m_vDir.Length() > 25) {
		CMonsterSC* pSc = (CMonsterSC*)m_pGameObject->FindComponentFromTag("pMonsterSC");
		pSc->SetTarget(NULL);
		SAFE_RELEASE(pSc);

		// FSM을 찾는다.
		CDx11FSM* pFSM = (CDx11FSM*)m_pGameObject->FindComponentFromTag("FSM");
		CWalkState* pState = pFSM->SetState<CWalkState>(m_pGameObject);

		SAFE_RELEASE(pState);
		SAFE_RELEASE(pFSM);
	}

	else {
		// FSM을 찾는다.
		CDx11FSM* pFSM = (CDx11FSM*)m_pGameObject->FindComponentFromTag("FSM");
		CAttackState* pState = pFSM->SetState<CAttackState>(m_pGameObject);

		SAFE_RELEASE(pState);
		SAFE_RELEASE(pFSM);
	}

	return 0;
}
