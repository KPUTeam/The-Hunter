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

	// �ִϸ��̼� Ŭ���� �ʱ�ȭ �Ѵ�.
	CDx11Animation3D* pAnimation3D = (CDx11Animation3D*)pBodyObj->FindComponentFromType(CT_ANIMATION3D);
	pAnimation3D->ChangeAnimation("Walk");
	m_dClipTime = pAnimation3D->GetCilpTime("Walk");
	SAFE_RELEASE(pAnimation3D);

	SAFE_RELEASE(pBodyObj);

	int iSatusTime = rand() % 5 + 2;
	DX11_GET_SINGLE(CDx11Scheduler)->CreateSchedule(this, &CWalkState::Exit, m_dClipTime*iSatusTime, SO_ONCE);

	// �������� �����Ѵ�.
	CDx11Transform* pTr = m_pGameObject->GetTransform();
	vDest = pTr->GetWorldPos();

	vDest.x += rand() % 201 - 100;
	vDest.z += rand() % 201 - 100;

	vDest.y = 0;

	SAFE_RELEASE(pTr);
}

void CWalkState::Execute(float fTime)
{
	// �� ��ġ�� �޾ƿ´�.
	CDx11Transform* pTr = m_pGameObject->GetTransform();
	Vec3 vPos = pTr->GetWorldPos();	vPos.y = 0;

	// ������ ���Ѵ�.
	Vec3 vDir = vDest- vPos; 
	vDir = vDir.Normalize();

	Vec3 MyAxisZ = pTr->GetWorldAxis(AXIS_Z);

	// ����� �� Z ���� �����Ѵ�.
	float fAngle = vDir.Dot(MyAxisZ);

	// ����� �� Z ���� �����Ѵ�.
	//Vec3 vAxis = vDir.Cross(MyAxisZ);

	// ȸ����Ų��.
	Vec3 vAxis = pTr->GetWorldAxis(AXIS_Y);
	pTr->RotateWorld(fAngle, fTime, vAxis);

	// ������ �ӵ��� �����δ�.
	pTr->MoveWorld(1.7, fTime, AXIS_Z);

	SAFE_RELEASE(pTr);
}

int CWalkState::Exit(float fTime)
{
	// FSM�� ã�´�.
	CDx11FSM* pFSM = (CDx11FSM*)m_pGameObject->FindComponentFromTag("FSM");

	// ���¸� �����Ѵ�.

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

	// ���̵� ������Ʈ�� �����Ѵ�.
	//CIdleState* State = pFSM->SetState<CIdleState>(m_pGameObject);
	//SAFE_RELEASE(State);

	SAFE_RELEASE(pFSM);

	return 0;
}

CWalkState * CWalkState::Clone()
{
	return new CWalkState(*this);
}
