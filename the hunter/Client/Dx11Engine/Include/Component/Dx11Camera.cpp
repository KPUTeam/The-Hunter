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
		// 휠업[+]확대 , 휠다운[-]축소
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
	// 뷰 행렬 * 원근 투영 행렬의 역행렬을 구한다.
	Matrix matInverse = m_matView * m_matProj;
	XMVECTOR	vDeterminent = XMMatrixDeterminant(matInverse);
	matInverse = XMMatrixInverse(&vDeterminent, matInverse);

	// 뷰포트 공간 
	Vec3	vPlaneVtx[8];

	//앞면
	
	vPlaneVtx[0] = Vec3(-1.f, 1.f, 0.f);	// LT
	vPlaneVtx[1] = Vec3(1.f, 1.f, 0.f);		// RT
	vPlaneVtx[2] = Vec3(1.f, -1.f, 0.f);	// RB
	vPlaneVtx[3] = Vec3(-1.f, -1.f, 0.f);	// LB

	//뒷면
	vPlaneVtx[4] = Vec3(-1.f, 1.f, 1.f);
	vPlaneVtx[5] = Vec3(1.f, 1.f, 1.f);
	vPlaneVtx[6] = Vec3(1.f, -1.f, 1.f);
	vPlaneVtx[7] = Vec3(-1.f, -1.f, 1.f);

	// 역행렬을 적용하여 절두체의 각 정점을 월드 공간으로 변환
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

	// 정점으로 각 평면을 구한다.
	// 바깥쪽으로 노말을 지정하기 위해 시계 방향으로.
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
	// 뷰공간으로의 변환 : 카메라의 위치를 월드상의 원점으로 이동시키고
	// 카메라의 Z축을 월드상의 Z+방향을 바라보도록 회전시킨다.
	// 즉 카메라가 월드상의 원점으로 이동하면 월드상의 모든 물체들이 카메라가 이동한만큼
	// 이동하게 되고 카메라의 Z축이 월드상의 Z+방향을 바라보도록 회전하게 된다면
	// 월드상의 모든 물체들은 카메라가 회전한만큼 카메라를 중심으로 공전하게 된다.

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

	// 카메라 위치를 타겟 위치로 옮긴다.
	m_pTransform->SetWorldPos(vTargetPos);

	// vDist의 Y축 회전을 적용한다.
	Matrix	matRotY = XMMatrixRotationY(fGap);
	vDist.Convert(XMVector3TransformCoord(vDist.Convert(), matRotY));

	// 변환된 dist 만큼 더한다.
	m_pTransform->MoveWorld(vDist);

	// 축을 갱신해줌
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

	// XMMatrixRotationAxis 함수를 이용
	// X축으로 회전행렬을 만들고
	// 회전행렬의 주소를 가르키는 변수를 선언

	Matrix	matRot = XMMatrixRotationAxis(vAxis.Convert(), fGap);
	float*	pMatrix = (float*)&matRot;
	memcpy(pMatrix + 12, &vTargetPos, sizeof(Vec3));

	// 카메라를 공전시키기 위하여 TransformCoord함수를 이용
	// XMVECTOR 타입을 리턴하는 TransformCoord 함수를 XMFLOAT으로 변환하여 vPos에 저장
	vPos.Convert(XMVector3TransformCoord(vDist.Convert(), matRot));

	// 카메라의 y - 플레이어의 y가 vDist와 같아지면 카메라는 플레이어의 정확히 수직선에 존재함
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


	// 축을 갱신해줌
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
	
	// 속도 0.003f
	vMouseGap *= 0.0005f;

	// 좌 우 회전
	if (vMouseGap.x != 0.f)
	{
		RotateLeftRight(vMouseGap.x);
	}

	// 상 하 회전
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

	// 절두체 갱신
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
