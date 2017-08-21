#include "WalkState.h"

#include "Core/Dx11Scheduler.h"
#include "Component/Dx11Animation3D.h"
#include "GameObject/Dx11GameObject.h"
#include "Component/DX11FSM.h"
#include "Component/Dx11Transform.h"

#include "SmileState.h"
#include "ShakeHeadState.h"
#include "IdleState.h"

CWalkState::CWalkState()
{
	m_eState = AI_WALK;
}

CWalkState::CWalkState(const CWalkState & script)
{
	*this = script;
}


CWalkState::~CWalkState()
{
}

void CWalkState::Enter()
{

	CDx11GameObject* pBodyObj = m_pGameObject->FindChildFromTag("pMeshObj");

	// 애니매이션 클립을 초기화 한다.
	CDx11Animation3D* pAnimation3D = (CDx11Animation3D*)pBodyObj->FindComponentFromType(CT_ANIMATION3D);
	pAnimation3D->ChangeAnimation("Walk");
	m_dClipTime = pAnimation3D->GetCilpTime("Walk");
	SAFE_RELEASE(pAnimation3D);

	SAFE_RELEASE(pBodyObj);

	int iSatusTime = rand() % 5 + 2;
	DX11_GET_SINGLE(CDx11Scheduler)->CreateSchedule(this, &CWalkState::Exit, m_dClipTime*iSatusTime, SO_ONCE);

	// 목적지를 설정한다.
	CDx11Transform* pTr = m_pGameObject->GetTransform();
	vDest = pTr->GetWorldPos();

	vDest.x += rand() % 201 - 100;
	vDest.z += rand() % 201 - 100;

	vDest.y = 0;

	SAFE_RELEASE(pTr);
}

void CWalkState::Execute(float fTime)
{
	// 내 위치를 받아온다.
	CDx11Transform* pTr = m_pGameObject->GetTransform();
	Vec3 vPos = pTr->GetWorldPos();	vPos.y = 0;

	// 방향을 구한다.
	Vec3 vDir = vDest- vPos; 
	vDir = vDir.Normalize();

	Vec3 MyAxisZ = pTr->GetWorldAxis(AXIS_Z);

	// 방향과 내 Z 축을 내적한다.
	float fAngle = vDir.Dot(MyAxisZ);

	// 방향과 내 Z 축을 외적한다.
	//Vec3 vAxis = vDir.Cross(MyAxisZ);

	// 회전시킨다.
	Vec3 vAxis = pTr->GetWorldAxis(AXIS_Y);
	pTr->RotateWorld(fAngle, fTime, vAxis);

	// 일정한 속도록 움직인다.
	pTr->MoveWorld(1.7, fTime, AXIS_Z);

	SAFE_RELEASE(pTr);
}

int CWalkState::Exit(float fTime)
{
	// FSM을 찾는다.
	CDx11FSM* pFSM = (CDx11FSM*)m_pGameObject->FindComponentFromTag("FSM");

	// 상태를 선택한다.

	int iChooseSatus = rand() % 2;

	if (iChooseSatus == 0)
	{
		CSmileState* pState = pFSM->SetState<CSmileState>(m_pGameObject);
		SAFE_RELEASE(pState);
	}

	if (iChooseSatus == 1)
	{
		CShakeHeadState* pShakeHeadState = pFSM->SetState<CShakeHeadState>(m_pGameObject);
		SAFE_RELEASE(pShakeHeadState);
	}

	// 아이들 스테이트로 진입한다.
	//CIdleState* State = pFSM->SetState<CIdleState>(m_pGameObject);
	//SAFE_RELEASE(State);

	SAFE_RELEASE(pFSM);

	return 0;
}

CWalkState * CWalkState::Clone()
{
	return new CWalkState(*this);
}
