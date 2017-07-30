#include "Dx11Camera.h"
#include "../Core/Dx11Device.h"
#include "../GameObject/Dx11GameObject.h"
#include "Dx11Transform.h"
#include "Dx11Mouse.h"
#include "../Core/Dx11Input.h"
DX11_USING

CDx11Camera::CDx11Camera()	:
	m_pTarget(NULL),
	m_iShakeTimes(0)
{
	SetTypeName<CDx11Camera>();
	m_eComType = CT_CAMERA;
}

CDx11Camera::CDx11Camera(const CDx11Camera & camera)	:
	CDx11Component(camera)
{
	*this = camera;
	m_iRef = 1;
}

CDx11Camera::~CDx11Camera()
{
	SAFE_RELEASE(m_pTarget);
}

void CDx11Camera::SetShakeTimes(int iTimes)
{
	m_iShakeTimes = iTimes;
}

int CDx11Camera::GetShakeTimes()
{
	return m_iShakeTimes;
}

void CDx11Camera::ShakeCamera()
{
	if (m_iShakeTimes % 4 == 0 || m_iShakeTimes % 4 == 3)
	{
		m_pTransform->MoveWorld(-0.2, 1.f, AXIS_X);
	}
	else if (m_iShakeTimes % 4 == 1 || m_iShakeTimes % 4 == 2)
	{
		m_pTransform->MoveWorld(+0.2, 1.f, AXIS_X);
	}
	--m_iShakeTimes;
}

Matrix CDx11Camera::GetViewMatrix() const
{
	return m_matView;
}

Matrix CDx11Camera::GetProjMatrix() const
{
	return m_matProj;
}

CAMERA_TYPE CDx11Camera::GetCameraType() const
{
	return m_eCamType;
}

float CDx11Camera::GetCameraDist() const
{
	return m_fViewDist;
}

CDx11GameObject * CDx11Camera::GetTarget() const
{
	if (!m_pTarget)
		return NULL;

	m_pTarget->AddRef();
	return m_pTarget;
}

void CDx11Camera::SetMinMax(float fMin, float fMax)
{
	m_fMin = fMin;
	m_fMax = fMax;
}

void CDx11Camera::ZoomInOut(CDx11Mouse * pMouse)
{

	short sWheelDir = pMouse->GetWheelDir();

	if (sWheelDir)
	{
		// �پ�[+]Ȯ�� , �ٴٿ�[-]���
		float fMove = sWheelDir * 0.5f;

		m_pTransform->MoveWorld(fMove, 1.f, AXIS_Z);
	}
}

void CDx11Camera::SetTarget(CDx11GameObject * pTarget)
{
	if (!m_pTarget)
		m_pTarget = pTarget;

	//if (m_pTarget)
	//{
	//	CDx11Transform*	pTr = m_pTarget->GetTransform();
	//	Vec3	vPos = m_pTransform->GetWorldPos();
	//	Vec3	vDist = vPos - pTr->GetWorldPos();

	//	SAFE_RELEASE(pTr);

	//	SAFE_RELEASE(m_pTarget);
	//	m_pTarget = pTarget;
	//	m_pTarget->AddRef();

	//	pTr = pTarget->GetTransform();

	//	m_vPrevPos = pTr->GetWorldPos();

	//	SAFE_RELEASE(pTr);

	//	m_pTransform->SetWorldPos(m_vPrevPos + vDist);
	//}

	//else
	//{
	//	SAFE_RELEASE(m_pTarget);
	//	m_pTarget = pTarget;
	//	m_pTarget->AddRef();

	//	CDx11Transform*	pTr = pTarget->GetTransform();

	//	m_vPrevPos = pTr->GetWorldPos();

	//	SAFE_RELEASE(pTr);

	//	m_pTransform->MoveWorld(m_vPrevPos);
	//}
}

void CDx11Camera::SetProjection(float fViewAngle, float fNear, float fViewDist)
{
	if (m_eCamType == CT_ORTHOGONAL)
		return;

	m_fViewAngle = fViewAngle;
	m_fNear = fNear;
	m_fViewDist = fViewDist;

	RESOLUTION_TYPE eRSType = DX11_GET_SINGLE(CDx11Device)->GetResolutionType();
	RESOLUTION	tRS = g_tRS[eRSType];
	float	fAspect = tRS.iWidth / (float)tRS.iHeight;
	m_matProj = XMMatrixPerspectiveFovLH(m_fViewAngle, fAspect, m_fNear, m_fViewDist);
}

void CDx11Camera::SetOrthoProjection(float fWidth, float fHeight, 
	float fNear, float fViewDist)
{
	m_fNear = fNear;
	m_fViewDist = fViewDist;

	m_matProj = XMMatrixOrthographicOffCenterLH(0.f, fWidth, fHeight, 0.f,
		fNear, fViewDist);
}

void CDx11Camera::SetCameraType(CAMERA_TYPE eType)
{
	m_eCamType = eType;
}

void CDx11Camera::TargetTrace()
{
	if (m_pTarget)
	{
		CDx11Transform*	pTr = m_pTarget->GetTransform();

		Vec3	vPos = pTr->GetWorldPos();

		SAFE_RELEASE(pTr);

		Vec3	vMove = vPos - m_vPrevPos;
		m_vPrevPos = vPos;

		m_pTransform->MoveWorld(vMove);
	}
}

void CDx11Camera::CalculateFrustrum()
{
	// �� ��� * ���� ���� ����� ������� ���Ѵ�.
	Matrix matInverse = m_matView * m_matProj;
	XMVECTOR	vDeterminent = XMMatrixDeterminant(matInverse);
	matInverse = XMMatrixInverse(&vDeterminent, matInverse);

	// ����Ʈ ���� 
	Vec3	vPlaneVtx[8];

	//�ո�
	
	vPlaneVtx[0] = Vec3(-1.f, 1.f, 0.f);	// LT
	vPlaneVtx[1] = Vec3(1.f, 1.f, 0.f);		// RT
	vPlaneVtx[2] = Vec3(1.f, -1.f, 0.f);	// RB
	vPlaneVtx[3] = Vec3(-1.f, -1.f, 0.f);	// LB

	//�޸�
	vPlaneVtx[4] = Vec3(-1.f, 1.f, 1.f);
	vPlaneVtx[5] = Vec3(1.f, 1.f, 1.f);
	vPlaneVtx[6] = Vec3(1.f, -1.f, 1.f);
	vPlaneVtx[7] = Vec3(-1.f, -1.f, 1.f);

	// ������� �����Ͽ� ����ü�� �� ������ ���� �������� ��ȯ
	XMVECTOR v;
	for (UINT i = 0; i < 8; ++i)
	{
		v = XMVector3TransformCoord(vPlaneVtx[i].Convert(), matInverse);
		vPlaneVtx[i].Convert(v);
		m_tFrustrum.vPos[i] = vPlaneVtx[i];
	}

	m_tFrustrum.vCenter.x = (m_tFrustrum.vPos[0].x + m_tFrustrum.vPos[1].x) / 2;
	m_tFrustrum.vCenter.y = (m_tFrustrum.vPos[0].y + m_tFrustrum.vPos[3].y) / 2;
	m_tFrustrum.vCenter.z = (m_tFrustrum.vPos[0].z + m_tFrustrum.vPos[4].z) / 2;

	// �������� �� ����� ���Ѵ�.
	// �ٱ������� �븻�� �����ϱ� ���� �ð� ��������.
	m_tFrustrum.m_vPlane[0] = XMPlaneFromPoints(vPlaneVtx[0].Convert(), vPlaneVtx[1].Convert(), vPlaneVtx[2].Convert());
	m_tFrustrum.m_vPlane[1] = XMPlaneFromPoints(vPlaneVtx[4].Convert(), vPlaneVtx[7].Convert(), vPlaneVtx[6].Convert());
	m_tFrustrum.m_vPlane[2] = XMPlaneFromPoints(vPlaneVtx[0].Convert(), vPlaneVtx[3].Convert(), vPlaneVtx[7].Convert());
	m_tFrustrum.m_vPlane[3] = XMPlaneFromPoints(vPlaneVtx[1].Convert(), vPlaneVtx[5].Convert(), vPlaneVtx[6].Convert());
	m_tFrustrum.m_vPlane[4] = XMPlaneFromPoints(vPlaneVtx[0].Convert(), vPlaneVtx[4].Convert(), vPlaneVtx[5].Convert());
	m_tFrustrum.m_vPlane[5] = XMPlaneFromPoints(vPlaneVtx[3].Convert(), vPlaneVtx[2].Convert(), vPlaneVtx[6].Convert());
}

Frustrum* CDx11Camera::GetFrustrum()
{
	return &m_tFrustrum;
}

void CDx11Camera::ComputeViewMatrix()
{
	// ����������� ��ȯ : ī�޶��� ��ġ�� ������� �������� �̵���Ű��
	// ī�޶��� Z���� ������� Z+������ �ٶ󺸵��� ȸ����Ų��.
	// �� ī�޶� ������� �������� �̵��ϸ� ������� ��� ��ü���� ī�޶� �̵��Ѹ�ŭ
	// �̵��ϰ� �ǰ� ī�޶��� Z���� ������� Z+������ �ٶ󺸵��� ȸ���ϰ� �ȴٸ�
	// ������� ��� ��ü���� ī�޶� ȸ���Ѹ�ŭ ī�޶� �߽����� �����ϰ� �ȴ�.

	if (m_eCamType == CT_ORTHOGONAL)
		return;

	m_matView = XMMatrixIdentity();
	float*	pView = (float*)&m_matView;

	Vec3	vPos = m_pTransform->GetWorldPos();
	Vec3	vAxis[AXIS_MAX];

	for (int i = 0; i < AXIS_MAX; ++i)
	{
		vAxis[i] = m_pTransform->GetWorldAxis((AXIS)i);
		memcpy(pView + (i * 4), &vAxis[i], sizeof(Vec3));
	}

	m_matView = XMMatrixTranspose(m_matView);

	for (int i = 0; i < AXIS_MAX; ++i)
	{
		float	fDot = vPos.Dot(vAxis[i]) * -1.f;
		memcpy(pView + (12 + i), &fDot, sizeof(float));
	}
}

bool CDx11Camera::Init()
{
	//m_pTransform->SetWorldPos(0.f, 0.f, -5.f);
	m_fViewAngle = 0.f;
	m_fNear = 0.05f;
	m_fViewDist = 1000.f;

	m_eCamType = CT_PERSPECTIVE;

	m_matView = XMMatrixIdentity();
	m_matProj = XMMatrixIdentity();

	/*XMVECTOR	vPos, vLookAt, vUp;
	vPos = XMLoadFloat3(&Vec3(0.f, 0.f, -5.f));
	vLookAt = XMLoadFloat3(&Vec3(0.f, 0.f, 0.f));
	vUp = XMVectorSet(0.f, 1.f, 0.f, 0.f);

	m_matView = XMMatrixLookAtLH(vPos, vLookAt, vUp);*/

	return true;
}

void CDx11Camera::RotateLeftRight(float fGap)
{
	CDx11Transform* pTr = m_pTarget->GetTransform();

	Vec3	vPos = m_pTransform->GetWorldPos();
	Vec3	vTargetPos = pTr->GetWorldPos();
	Vec3	vDist = vPos - vTargetPos;
	Vec3	vAxis = m_pTransform->GetWorldAxis(AXIS_Y);
	SAFE_RELEASE(pTr);

	// ī�޶� ��ġ�� Ÿ�� ��ġ�� �ű��.
	m_pTransform->SetWorldPos(vTargetPos);

	// vDist�� Y�� ȸ���� �����Ѵ�.
	Matrix	matRotY = XMMatrixRotationY(fGap);
	vDist.Convert(XMVector3TransformCoord(vDist.Convert(), matRotY));

	// ��ȯ�� dist ��ŭ ���Ѵ�.
	m_pTransform->MoveWorld(vDist);

	// ���� ��������
	for (int i = 0; i < AXIS_MAX; ++i)
	{
		vAxis = m_pTransform->GetWorldAxis((AXIS)i);
		vAxis.Convert(XMVector3TransformNormal(vAxis.Convert(), matRotY));
		m_pTransform->SetWorldAxis(vAxis, (AXIS)i);
	}


}


void CDx11Camera::RotateUpDown(float fGap)
{
	CDx11Transform* pTr = m_pTarget->GetTransform();

	Vec3	vPos = m_pTransform->GetWorldPos();
	Vec3	vTargetPos = pTr->GetWorldPos();
	Vec3	vDist = vPos - vTargetPos;
	Vec3	vAxis = m_pTransform->GetWorldAxis(AXIS_X);

	// XMMatrixRotationAxis �Լ��� �̿�
	// X������ ȸ������� �����
	// ȸ������� �ּҸ� ����Ű�� ������ ����

	Matrix	matRot = XMMatrixRotationAxis(vAxis.Convert(), fGap);
	float*	pMatrix = (float*)&matRot;
	memcpy(pMatrix + 12, &vTargetPos, sizeof(Vec3));

	// ī�޶� ������Ű�� ���Ͽ� TransformCoord�Լ��� �̿�
	// XMVECTOR Ÿ���� �����ϴ� TransformCoord �Լ��� XMFLOAT���� ��ȯ�Ͽ� vPos�� ����
	vPos.Convert(XMVector3TransformCoord(vDist.Convert(), matRot));

	// ī�޶��� y - �÷��̾��� y�� vDist�� �������� ī�޶�� �÷��̾��� ��Ȯ�� �������� ������
	if (vDist.Length() - 0.3f > abs(vPos.y - vTargetPos.y))
	{
		m_pTransform->SetWorldPos(vPos);

	}
	else if (vPos.y != vTargetPos.y)
	{
		matRot = XMMatrixRotationAxis(vAxis.Convert(), -fGap);
		pMatrix = (float*)&matRot;
		memcpy(pMatrix + 12, &vTargetPos, sizeof(Vec3));
		vPos.Convert(XMVector3TransformCoord(vDist.Convert(), matRot));
		m_pTransform->SetWorldPos(vPos);
	}


	// ���� ��������
	for (int i = 0; i < AXIS_MAX; ++i)
	{
		vAxis = m_pTransform->GetWorldAxis((AXIS)i);
		vAxis.Convert(XMVector3TransformNormal(vAxis.Convert(), matRot));
		vAxis = vAxis.Normalize();
		m_pTransform->SetWorldAxis(vAxis, (AXIS)i);
	}

	SAFE_RELEASE(pTr);
}

void CDx11Camera::RotateCamera(CDx11Mouse * pMouse)
{
	if(!m_pTarget)
		return;

	Vec2 vMouseGap = pMouse->GetMouseGap();
	
	// �ӵ� 0.003f
	vMouseGap *= 0.0005f;

	// �� �� ȸ��
	if (vMouseGap.x != 0.f)
	{
		RotateLeftRight(vMouseGap.x);
	}

	// �� �� ȸ��
	if (vMouseGap.y != 0.f)
	{
		RotateUpDown(vMouseGap.y);
	}
	 
	pMouse->SetMousePositionToCenter();
}

int CDx11Camera::Update(float fTime)
{
	if (m_iShakeTimes > 0)
		ShakeCamera();

	TargetTrace();

	CDx11Mouse*	pMouse = DX11_GET_SINGLE(CDx11Input)->GetMouse();
	ZoomInOut(pMouse);
	//RotateCamera(pMouse);
	SAFE_RELEASE(pMouse);

	return 0;
}

int CDx11Camera::LateUpdate(float fTime)
{
	//TargetTrace();

	ComputeViewMatrix();

	// ����ü ����
	CalculateFrustrum();

	return 0;
}

void CDx11Camera::Render(float fTime)
{
	//TargetTrace();
	//ComputeViewMatrix();
}

CDx11Camera * CDx11Camera::Clone()
{
	return new CDx11Camera(*this);
}
