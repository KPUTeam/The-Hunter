#include "Dx11Camera.h"
#include "../Core/Dx11Device.h"
#include "Dx11Transform.h"
#include "../GameObject/Dx11GameObject.h"
#include "Dx11Mouse.h"
#include "../Core/Dx11Input.h"
//#include "../Core/Dx11Debug.h"

DX11_USING

CDx11Camera::CDx11Camera()	:
	m_iShakeTimes(0),
	m_pTarget(NULL)
{
	m_eComType = CT_CAMERA;
	m_eProjType = PT_PERSPECTIVE;
	m_eCameraType = CVT_FIRST;
	SetTypeName<CDx11Camera>();
}

CDx11Camera::CDx11Camera(const CDx11Camera & camera)	:
	CDx11Component(camera)
{
	*this = camera;
	m_iRefCount = 1;

	if (m_pTarget)
		m_pTarget->AddRef();
}


CDx11Camera::~CDx11Camera()
{
	SAFE_RELEASE(m_pTarget);
}

Matrix CDx11Camera::GetViewMatrix()
{
	return m_matView;
}

Matrix CDx11Camera::GetProjMatrix()
{
	return m_matProj;
}


int CDx11Camera::GetShakeTimes()
{
	return m_iShakeTimes;
}

CDx11GameObject * CDx11Camera::GetTarget() const
{
	if (!m_pTarget)
		return NULL;

	m_pTarget->AddRef();
	return m_pTarget;
}

Vec2 CDx11Camera::GetMouseGap() const
{
	return m_vMouseGap;
}

void CDx11Camera::SetTarget(CDx11GameObject * pTarget)
{
	SAFE_RELEASE(m_pTarget);
	
	m_pTarget = pTarget;
	m_pTarget->AddRef();

	CDx11Transform*	pTr = m_pTarget->GetTransform();
	m_vPrevPos = pTr->GetWorldPos();
	SAFE_RELEASE(pTr);

}

void CDx11Camera::ShakeCamera()
{
	if (m_iShakeTimes % 4 == 0 || m_iShakeTimes % 4 == 3)
	{
		m_pTransform->MoveWorld(-0.1f, 1.f, AXIS_X);
	}
	else if (m_iShakeTimes % 4 == 1 || m_iShakeTimes % 4 == 2)
	{
		m_pTransform->MoveWorld(+0.1f, 1.f, AXIS_X);
	}
	--m_iShakeTimes;
}

void CDx11Camera::TraceTarget()
{
	if (m_pTarget)
	{
		CDx11Transform*	pTr = m_pTarget->GetTransform();
		Vec3	vPos = pTr->GetWorldPos();
		Vec3	vMove = vPos - m_vPrevPos;
		m_vPrevPos = vPos;

		m_pTransform->MoveWorld(vMove);
		SAFE_RELEASE(pTr);
	}
}

void CDx11Camera::SetMinMax(float fMin, float fMax)
{
	m_fMin = fMin;
	m_fMax = fMax;
}

void CDx11Camera::ZoomInOut(CDx11Mouse* pMouse)
{
	short sWheelDir = pMouse->GetWheelDir();

	if (sWheelDir)
	{
		// 휠업[+]확대 , 휠다운[-]축소
		float fMove = sWheelDir * 0.1f;

		CDx11Transform* pTr = m_pTarget->GetTransform();

		Vec3 vTargetPos = pTr->GetWorldPos();
		Vec3 vPos = m_pTransform->GetWorldPos();

		// 타겟과 카메라 사이의 거리
		float fDist = vTargetPos.Distance(vPos);
		// fMove만큼 이동하므로 거리는 그만큼 가까워지거나 멀어진다.
		fDist -= fMove;

		// 최대, 최소 검사
		if (fDist < m_fMin)
		{
			fMove = 0.f;
		}
		else if (fDist > m_fMax)
		{
			fMove = 0.f;
		}

		m_pTransform->MoveWorld(fMove, 1.f, AXIS_Z);
		SAFE_RELEASE(pTr);
	}
}

void CDx11Camera::RotateCamera(CDx11Mouse * pMouse)
{
	if (pMouse->GetRightDrag())
	{
		Vec2 vMouseGap = pMouse->GetMouseGap();
		vMouseGap *= m_fMouseSens;

		if (vMouseGap.x != 0.f)
		{
			CDx11Transform* pTr = m_pTarget->GetTransform();
			
			Vec3	vPos = m_pTransform->GetWorldPos();
			Vec3	vTargetPos = pTr->GetWorldPos();
			Vec3	vDist = vPos - vTargetPos;
			Vec3	vAxis = m_pTransform->GetWorldAxis(AXIS_Y);

			// Y축으로 회전행렬을 만들고
			// 회전행렬의 주소를 가르키는 변수를 선언
			Matrix	matRotY = XMMatrixRotationY(vMouseGap.x);
//			XMVECTOR vQuarternion = XMQuaternionRotationAxis(vAxis.Convert(), vMouseGap.x);
//			Matrix	matRotY = XMMatrixRotationQuaternion(vQuarternion);
			float*	pMatrix = (float*)&matRotY;

			// 행렬의 Translate 부분을 복사
			memcpy(pMatrix + 12, &vTargetPos, sizeof(Vec3));

			// 카메라를 공전시키기 위하여 TransformCoord함수를 이용
			// XMVECTOR 타입을 리턴하는 TransformCoord 함수를 XMFLOAT으로 변환하여 vPos에 저장
			vPos.Convert(XMVector3TransformCoord(vDist.Convert(), matRotY));
			m_pTransform->SetWorldPos(vPos);
			m_vMouseGap.x = vMouseGap.x;

			// 축을 갱신해줌
			for (int i = 0; i < AXIS_MAX; ++i)
			{
				vAxis = m_pTransform->GetWorldAxis((AXIS)i);
				vAxis.Convert(XMVector3TransformNormal(vAxis.Convert(), matRotY));
				m_pTransform->SetWorldAxis(vAxis, (AXIS)i);
			}
			
			SAFE_RELEASE(pTr);		
		}

		if (vMouseGap.y != 0.f)
		{
			CDx11Transform* pTr = m_pTarget->GetTransform();

			Vec3	vPos = m_pTransform->GetWorldPos();
			Vec3	vTargetPos = pTr->GetWorldPos();
			Vec3	vDist = vPos - vTargetPos;
			Vec3	vAxis = m_pTransform->GetWorldAxis(AXIS_X);

			// XMMatrixRotationAxis 함수를 이용
			// X축으로 회전행렬을 만들고
			// 회전행렬의 주소를 가르키는 변수를 선언

//			XMVECTOR	vQuarternion = XMQuaternionRotationAxis(vAxis.Convert(), vMouseGap.y);
			Matrix	matRot = XMMatrixRotationAxis(vAxis.Convert(), vMouseGap.y);
//			Matrix	matRot = XMMatrixRotationQuaternion(vQuarternion);
			float*	pMatrix = (float*)&matRot;

			// 행렬의 Translate 부분을 복사
			memcpy(pMatrix + 12, &vTargetPos, sizeof(Vec3));

			// 카메라를 공전시키기 위하여 TransformCoord함수를 이용
			// XMVECTOR 타입을 리턴하는 TransformCoord 함수를 XMFLOAT으로 변환하여 vPos에 저장
			vPos.Convert(XMVector3TransformCoord(vDist.Convert(), matRot));
			m_pTransform->SetWorldPos(vPos);
			m_vMouseGap.y = vMouseGap.y;

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



	}
}

void CDx11Camera::SetCameraType(CAMERA_VIEW_TYPE eType)
{
	m_eCameraType = eType;

	if (m_eCameraType == CVT_THIRD)
	{
		m_pTransform->MoveWorld(-3.f, 1.f, AXIS_Z);
	}
}

void CDx11Camera::SetProjection(float fAngle, float fNear, float fFar)
{
//	해상도 정보를 디바이스에서 얻어옴
	RESOLUTION_TYPE eType = DX11_GET_SINGLE(CDx11Device)->GetResolutionType();
//	종횡비는 가로 / 세로 (float타입)	
	float fAspect = g_tRS[eType].iWidth / (float)g_tRS[eType].iHeight;
//	투영 행렬을 구한다.
	m_matProj = XMMatrixPerspectiveFovLH(fAngle, fAspect, fNear, fFar);

	m_fAngle = fAngle;
	m_fNear = fNear;
	m_fFar = fFar;
}

void CDx11Camera::SetOrthoProjection(float fNear, float fFar)
{
	RESOLUTION_TYPE eType = DX11_GET_SINGLE(CDx11Device)->GetResolutionType();
	m_matProj = XMMatrixOrthographicOffCenterLH(0.f, (float)g_tRS[eType].iWidth,
		(float)g_tRS[eType].iHeight, 0.f, fNear, fFar);

	m_fNear = fNear;
	m_fFar = fFar;
	m_eProjType = PT_ORTHOGONAL;
}

void CDx11Camera::SetAngle(float fAngle)
{
	m_fAngle = fAngle;
	RESOLUTION_TYPE	eType = DX11_GET_SINGLE(CDx11Device)->GetResolutionType();
	float	fAspect = g_tRS[eType].iWidth / (float)g_tRS[eType].iHeight;
	m_matProj = XMMatrixPerspectiveFovLH(m_fAngle, fAspect, m_fNear, m_fFar);
}

void CDx11Camera::SetNear(float fNear)
{
	m_fNear = fNear;
	RESOLUTION_TYPE	eType = DX11_GET_SINGLE(CDx11Device)->GetResolutionType();
	float	fAspect = g_tRS[eType].iWidth / (float)g_tRS[eType].iHeight;
	m_matProj = XMMatrixPerspectiveFovLH(m_fAngle, fAspect, m_fNear, m_fFar);
}

void CDx11Camera::SetDistance(float fDist)
{
	m_fFar = fDist;
	RESOLUTION_TYPE	eType = DX11_GET_SINGLE(CDx11Device)->GetResolutionType();
	float	fAspect = g_tRS[eType].iWidth / (float)g_tRS[eType].iHeight;
	m_matProj = XMMatrixPerspectiveFovLH(m_fAngle, fAspect, m_fNear, m_fFar);
}

void CDx11Camera::SetShakeTimes(int iTimes)
{
	m_iShakeTimes = iTimes;
}



bool CDx11Camera::Init()
{
	m_matView = XMMatrixIdentity();
	m_matProj = XMMatrixIdentity();
	m_fMin = 1.f;
	m_fMax = 10.f;
	m_fMouseSens = 0.003f;
	return true;
}

int CDx11Camera::Update(float fTime)
{
	if(m_iShakeTimes > 0)
		ShakeCamera();

	TraceTarget();

	// 3인칭 카메라일 때만 마우스 입력 적용
	if (m_eCameraType == CVT_THIRD)
	{
		
		// 마우스를 얻어온다.
		CDx11Mouse*	pMouse = DX11_GET_SINGLE(CDx11Input)->GetMouse();
		
		if (m_pTarget)
		{
			ZoomInOut(pMouse);
			RotateCamera(pMouse);
		}

		SAFE_RELEASE(pMouse);
	}
	return 0;
}

int CDx11Camera::LateUpdate(float fTime)
{
	// 뷰 행렬 초기화
	m_matView = XMMatrixIdentity();

	// 투영일 때만 계산한다.
	if (m_eProjType == PT_PERSPECTIVE)
	{
		// 회전 부분을 만든다.
		// pMatrix는 뷰 행렬을 가리키는 포인터
		float* pMatrix = (float*)&m_matView;
		for (int i = 0; i < AXIS_MAX; ++i)
		{
			// 행렬 1줄씩(X축, Y축, Z축) 메모리 복사
			// Transform에서 축을 얻어온다.
			memcpy(pMatrix + (i * 4), &m_pTransform->GetWorldAxis()[i],
				sizeof(Vec3));
		}
		// 행렬 전치
		m_matView = XMMatrixTranspose(m_matView);


		// 위치 부분을 만든다.
		// Transform에서 위치를 얻어온다.
		Vec3	vPos = m_pTransform->GetWorldPos();
		vPos *= -1.f;
		for (int i = 0; i < AXIS_MAX; ++i)
		{
			float fDot = vPos.Dot(m_pTransform->GetWorldAxis((AXIS)i));

			//	위치 부분은 4행부분이므로 +12부터 시작
			memcpy(pMatrix + (12 + i), &fDot, sizeof(float));
		}
	}
	return 0;
}

void CDx11Camera::Render(float fTime)
{
}

CDx11Camera * CDx11Camera::Clone()
{
	return new CDx11Camera(*this);
}
