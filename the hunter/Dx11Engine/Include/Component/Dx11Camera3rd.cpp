#include "Dx11Camera3rd.h"
#include "../GameObject\/Dx11GameObject.h"
#include "Dx11Mouse.h"
#include "../Core/Dx11Input.h"
#include "Dx11Transform.h"
#include "Dx11Camera.h"

DX11_USING

CDx11Camera3rd::CDx11Camera3rd()
{
	m_eComType = CT_SCRIPT;
	SetTypeName<CDx11Camera3rd>();
}


CDx11Camera3rd::CDx11Camera3rd(const CDx11Camera3rd & camera)
{
	*this = camera;
	m_iRefCount = 1;

}

CDx11Camera3rd::~CDx11Camera3rd()
{
}

void CDx11Camera3rd::SetMinMax(float fMin, float fMax)
{
	m_fMin = fMin;
	m_fMax = fMax;
}


bool CDx11Camera3rd::Init()
{
	m_fMin = 1.f;
	m_fMax = 10.f;
	return true;
}

int CDx11Camera3rd::Update(float fTime)
{
	// 마우스를 얻어온다.
	CDx11Mouse*	pMouse = DX11_GET_SINGLE(CDx11Input)->GetMouse();
	short sWheelDir = pMouse->GetWheelDir();

	if (sWheelDir)
	{
		float fMove = sWheelDir * 0.1f;

		CDx11Camera*	pCamera = (CDx11Camera*)m_pGameObject->FindComponentFromTypeName("CDx11Camera");
		CDx11GameObject* pTarget = pCamera->GetTarget();
		CDx11Transform* pTr = pTarget->GetTransform();

		Vec3 vTargetPos = pTr->GetWorldPos();
		Vec3 vPos = m_pTransform->GetWorldPos();

		float fDist = vTargetPos.Distance(vPos);
		fDist -= fMove;
		
		// 최대, 최소 검사
		if (fDist < m_fMin)
		{
			fMove -= (m_fMin - fDist);
		}
		else if (fDist > m_fMax)
		{
			fMove += (fDist - m_fMax);
		}

		m_pTransform->MoveWorld(fMove, 1.f, AXIS_Z);

		SAFE_RELEASE(pTr);
		SAFE_RELEASE(pTarget);
		SAFE_RELEASE(pCamera);
	}


	SAFE_RELEASE(pMouse);

	return 0;
}

int CDx11Camera3rd::LateUpdate(float fTime)
{
	return 0;
}

void CDx11Camera3rd::Render(float fTime)
{
}

CDx11Camera3rd * CDx11Camera3rd::Clone()
{
	return new CDx11Camera3rd(*this);
}
