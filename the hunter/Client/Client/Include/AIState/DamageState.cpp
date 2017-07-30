#include "DamageState.h"

#include "Core/Dx11Scheduler.h"
#include "Component/Dx11Animation3D.h"
#include "GameObject/Dx11GameObject.h"
#include "Component/DX11FSM.h"
#include "Component/Dx11Transform.h"

#include "SmileState.h"
#include "ShakeHeadState.h"
#include "IdleState.h"
#include "WalkState.h"
#include "RunState.h"

CDamageState::CDamageState()
{
	m_eState = AI_DAMAGE;
}

CDamageState::CDamageState(const CDamageState & script)
{
	*this = script;
}


CDamageState::~CDamageState()
{
}

void CDamageState::Enter()
{
	CDx11GameObject* pBodyObj = m_pGameObject->FindChildFromTag("pMeshObj");

	// 애니매이션 클립을 초기화 한다.
	CDx11Animation3D* pAnimation3D = (CDx11Animation3D*)pBodyObj->FindComponentFromType(CT_ANIMATION3D);

	int iChooseSatus = rand() % 2;

	if (iChooseSatus == 0)
	{
		pAnimation3D->ChangeAnimation("Damage_Left");
		m_dClipTime = pAnimation3D->GetCilpTime("Damage_Left");
	}

	if (iChooseSatus == 1)
	{
		pAnimation3D->ChangeAnimation("Damage_Right");
		m_dClipTime = pAnimation3D->GetCilpTime("Damage_Right");
	}

	SAFE_RELEASE(pAnimation3D);
	SAFE_RELEASE(pBodyObj);


	DX11_GET_SINGLE(CDx11Scheduler)->CreateSchedule(this, &CDamageState::Exit, m_dClipTime, SO_ONCE);
}

void CDamageState::Execute(float fTime)
{
}

int CDamageState::Exit(float fTime)
{
	// FSM을 찾는다.
	CDx11FSM* pFSM = (CDx11FSM*)m_pGameObject->FindComponentFromTag("FSM");
	CRunState* pState = pFSM->SetState<CRunState>(m_pGameObject);
	//CDamageState* pState = pFSM->SetState<CDamageState>(m_pGameObject);
	SAFE_RELEASE(pState);
	SAFE_RELEASE(pFSM);

	return 0;
}

CDamageState * CDamageState::Clone()
{
	return new CDamageState(*this);
}
