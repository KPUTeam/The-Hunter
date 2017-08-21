#include "Dx11Camera3rd.h"
#include "../Core/Dx11Input.h"
#include "Dx11Camera.h"
#include "../GameObject/Dx11GameObject.h"
#include "Dx11Transform.h"
#include "Dx11Mouse.h"

DX11_USING

CDx11Camera3rd::CDx11Camera3rd()
{
	m_eComType = CT_SCRIPT;
	SetTypeName<CDx11Camera3rd>();
}

CDx11Camera3rd::CDx11Camera3rd(const CDx11Camera3rd & camera)	:
	CDx11Script(camera)
{
	m_fMouseSens = camera.m_fMouseSens;
}

CDx11Camera3rd::~CDx11Camera3rd()
{
}

void CDx11Camera3rd::SetLimitDistance(float fMin, float fMax)
{
	m_fMinDist = fMin;
	m_fMaxDist = fMax;
}

bool CDx11Camera3rd::Init()
{
	m_fMouseSens = 0.003f;
	m_fMinDist = 1.f;
	m_fMaxDist = 10.f;

	return true;
}

int CDx11Camera3rd::Update(float fTime)
{
	CDx11Mouse*	pMouse = DX11_GET_SINGLE(CDx11Input)->GetMouse();

	short	sWheelDir = pMouse->GetWheelDir();

	if (sWheelDir)
	{
		float fMove = sWheelDir * 0.1f;

		CDx11Camera*	pCamera = (CDx11Camera*)m_pGameObject->FindComponentFromType(CT_CAMERA);

		CDx11GameObject*	pTarget = pCamera->GetTarget();

		CDx11Transform*	pTr = pTarget->GetTransform();

		Vec3	vTargetPos = pTr->GetWorldPos();
		Vec3	vPos = m_pTransform->GetWorldPos();

		float	fDist = vPos.Distance(vTargetPos);

		fDist -= fMove;

		if (fDist < m_fMinDist)
		{
			fMove -= (m_fMinDist - fDist);
		}

		else if (fDist > m_fMaxDist)
		{
			fMove += (fDist - m_fMaxDist);
		}

		SAFE_RELEASE(pTr);
		SAFE_RELEASE(pTarget);
		SAFE_RELEASE(pCamera);

		m_pTransform->MoveWorld(fMove, 1.f, AXIS_Z);
	}

	if (pMouse->RButtonStay())
	{
		Vec2	vMouseDrag = pMouse->GetMouseGap();

		vMouseDrag *= m_fMouseSens;

		if (vMouseDrag.x != 0.f)
		{
			CDx11Camera*	pCamera = (CDx11Camera*)m_pGameObject->FindComponentFromType(CT_CAMERA);

			CDx11GameObject*	pTarget = pCamera->GetTarget();

			CDx11Transform*	pTr = pTarget->GetTransform();

			//m_pTransform->RotateWorldY(vMouseDrag.x, 1.f);

			Vec3	vPos = m_pTransform->GetWorldPos();
			Vec3	vTargetPos = pTr->GetWorldPos();
			Vec3	vDist = vPos - vTargetPos;

			Matrix	matRotY = XMMatrixRotationY(vMouseDrag.x);
			float*	pMat = (float*)&matRotY;
			memcpy(pMat + 12, &vTargetPos, sizeof(Vec3));

			vPos.Convert(XMVector3TransformCoord(vDist.Convert(), matRotY));

			m_pTransform->SetWorldPos(vPos);

			Vec3	vAxis;
			
			for (int i = 0; i < AXIS_MAX; ++i)
			{
				vAxis = m_pTransform->GetWorldAxis((AXIS)i);
				vAxis.Convert(XMVector3TransformNormal(vAxis.Convert(), matRotY));
				m_pTransform->SetWorldAxis(vAxis, (AXIS)i);
			}

			SAFE_RELEASE(pTr);

			SAFE_RELEASE(pTarget);

			SAFE_RELEASE(pCamera);
		}

		if (vMouseDrag.y != 0.f)
		{
			CDx11Camera*	pCamera = (CDx11Camera*)m_pGameObject->FindComponentFromType(CT_CAMERA);

			CDx11GameObject*	pTarget = pCamera->GetTarget();

			CDx11Transform*	pTr = pTarget->GetTransform();

			Vec3	vAxis = m_pTransform->GetWorldAxis(AXIS_X);

			//m_pTransform->RotateWorldY(vMouseDrag.x, 1.f);

			Vec3	vPos = m_pTransform->GetWorldPos();
			Vec3	vTargetPos = pTr->GetWorldPos();
			Vec3	vDist = vPos - vTargetPos;

			Matrix	matRot = XMMatrixRotationAxis(vAxis.Convert(), vMouseDrag.y);
			float*	pMat = (float*)&matRot;
			memcpy(pMat + 12, &vTargetPos, sizeof(Vec3));

			vPos.Convert(XMVector3TransformCoord(vDist.Convert(), matRot));

			m_pTransform->SetWorldPos(vPos);

			for (int i = AXIS_Y; i < AXIS_MAX; ++i)
			{
				vAxis = m_pTransform->GetWorldAxis((AXIS)i);
				vAxis.Convert(XMVector3TransformNormal(vAxis.Convert(), matRot));
				m_pTransform->SetWorldAxis(vAxis, (AXIS)i);
			}

			SAFE_RELEASE(pTr);

			SAFE_RELEASE(pTarget);

			SAFE_RELEASE(pCamera);
		}
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
