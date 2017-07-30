#include "AttackState.h"

#include "Core/Dx11Scheduler.h"
#include "Component/Dx11Animation3D.h"
#include "GameObject/Dx11GameObject.h"
#include "Component/DX11FSM.h"
#include "Component/Dx11ColliderSphere.h"
#include "Component/Dx11ColliderOBB.h"
#include "Component/Dx11Transform.h"

#include "RunState.h"

CAttackState::CAttackState()
{
	m_eState = AI_ATTACK;
}

CAttackState::CAttackState(const CAttackState & script)
{
	*this = script;
}


CAttackState::~CAttackState()
{
}

void CAttackState::Enter()
{
	CDx11GameObject* pBodyObj = m_pGameObject->FindChildFromTag("pMeshObj");

	// �ִϸ��̼� Ŭ���� �ʱ�ȭ �Ѵ�.
	CDx11Animation3D* pAnimation3D = (CDx11Animation3D*)pBodyObj->FindComponentFromType(CT_ANIMATION3D);
	pAnimation3D->ChangeAnimation("Attack");
	m_dClipTime = pAnimation3D->GetCilpTime("Attack");
	SAFE_RELEASE(pAnimation3D);

	SAFE_RELEASE(pBodyObj);

	DX11_GET_SINGLE(CDx11Scheduler)->CreateSchedule(this, &CAttackState::Attack, m_dClipTime*0.4, SO_ONCE);
	DX11_GET_SINGLE(CDx11Scheduler)->CreateSchedule(this, &CAttackState::RemoveAttackBox, m_dClipTime*0.65,  SO_ONCE);
	DX11_GET_SINGLE(CDx11Scheduler)->CreateSchedule(this, &CAttackState::Exit, m_dClipTime, SO_ONCE);
}

void CAttackState::Execute(float fTime)
{
}

CAttackState * CAttackState::Clone()
{
	return new CAttackState(*this);
}

int CAttackState::Attack(float fTime)
{
	//  �� ��ġ�� ������ ����
	//CDx11GameObject* pMeshObj = m_pGameObject->FindComponentFromTag();

	CDx11Transform* pTr = m_pGameObject->GetTransform();
	Vec3 vPos = pTr->GetWorldPos();
	Vec3 AxisZ = pTr->GetWorldAxis(AXIS_Z);
	AxisZ *= 1;
	// ����
	vPos += AxisZ;
	vPos.y += 0.5;
	
	// ���濡 �ݸ����� �߰��Ѵ�.
	// �浹ü�� �����Ѵ�.


	//CDx11ColliderOBB* pOBB = m_pGameObject->AddComponent<CDx11ColliderOBB>(MONSTER_ATTACK_BOX);
	//float fAxisLen[3];
	//fAxisLen[0] = 0.6;
	//fAxisLen[1] = 0.8f;
	//fAxisLen[2] = 0.4;

	//pOBB->SetPivot(AxisZ);
	//pOBB->SetOBBInfo(fAxisLen, pTr);
	//SAFE_RELEASE(pOBB);

	// ���濡 �ݸ����� �߰��Ѵ�.
	CDx11ColliderSphere* pSphere = m_pGameObject->AddComponent<CDx11ColliderSphere>("MonsetAttackBox");
	pSphere->SetSphereInfo(0.8f, vPos);
	SAFE_RELEASE(pSphere);

	SAFE_RELEASE(pTr);
	return 0;
}

int CAttackState::RemoveAttackBox(float fTime)
{
	// �浹 ��ü�� �����.
	m_pGameObject->DeleteComponent(MONSTER_ATTACK_BOX);
	return 0;
}

int CAttackState::Exit(float fTime)
{
	// FSM�� ã�´�.
	CDx11FSM* pFSM = (CDx11FSM*)m_pGameObject->FindComponentFromTag("FSM");
	CRunState* staet = pFSM->SetState<CRunState>(m_pGameObject);

	SAFE_RELEASE(staet);
	SAFE_RELEASE(pFSM);

	return 0;
}
