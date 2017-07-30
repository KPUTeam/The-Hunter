#include "Dx11ColliderRay.h"
#include "../Core/Dx11Device.h"
#include "../Scene/Dx11SceneManager.h"
#include "../Scene/Dx11Scene.h"
#include "../GameObject/Dx11GameObject.h"
#include "Dx11Camera.h"
#include "Dx11ColliderSphere.h"
#include "Dx11ColliderOBB.h"
#include "Dx11Transform.h"
#include "../Core/Dx11Input.h"

DX11_USING

CDx11ColliderRay::CDx11ColliderRay()
{
	m_eCollType = CST_RAY;
	SetTypeName<CDx11ColliderRay>();
	memset(&m_tRay, 0, sizeof(RAY));
}


CDx11ColliderRay::CDx11ColliderRay(const CDx11ColliderRay & ray)
{
	*this = ray;
}

CDx11ColliderRay::~CDx11ColliderRay()
{
}

RAY CDx11ColliderRay::GetRay() const
{
	return m_tRay;
}

void CDx11ColliderRay::SetMousePos(const POINT & ptPos)
{
	m_vMousePos = ptPos;
}

void CDx11ColliderRay::SetTargetPos(const POINT & ptPos)
{
	m_vTargetPos = ptPos;
}

void CDx11ColliderRay::SetStartPos(const Vec3 & vPos)
{
	m_vStartPos = vPos;
}

bool CDx11ColliderRay::Init()
{
	RESOLUTION_TYPE eRS = DX11_GET_SINGLE(CDx11Device)->GetResolutionType();
	m_vTargetPos = Vec2(g_tRS[eRS].iWidth / 2, g_tRS[eRS].iHeight / 2);
	m_vStartPos = VEC3ZERO;
	return true;
}

int CDx11ColliderRay::Update(float fTime)
{
	// 뷰포트를 얻어온다.z

	D3D11_VIEWPORT	vp;
	UINT	iVPCount = 1;
	DX11_CONTEXT->RSGetViewports(&iVPCount, &vp);

	// 카메라 투영행렬을 얻어온다.
	CDx11Scene*	pScene = DX11_GET_SINGLE(CDx11SceneManager)->GetCurrentScene();
	CDx11GameObject*	pCameraObj = pScene->GetCamera();
	CDx11Camera*	pCamera = (CDx11Camera*)pCameraObj->FindComponentFromType(CT_CAMERA);

	Matrix	matProj = pCamera->GetProjMatrix();
	Matrix	matView = pCamera->GetViewMatrix();

	// 뷰 행렬을 역행렬로 만들어준다.
	XMVECTOR	vDeterminent = XMMatrixDeterminant(matView);
	matView = XMMatrixInverse(&vDeterminent, matView);
	float*	pProj = (float*)&matProj;
	float*	pView = (float*)&matView;


	// 투영공간 좌표 x = sx * 2/w - 1
	// 투영공간 좌표 y = sy * -2/h + 1

	// 뷰 공간에서의 광선 방향을 구한다.
	// 투영 행렬은 11, 22에 값이 들어있고 나머지는 0이다.
	// 그러므로 11, 22로 나누고 뷰 공간의 x, y 좌표를 구해준다.

	//	POINT pt = DX11_GET_SINGLE(CDx11Input)->GetMousePoint();
	//	m_vMousePos.x = pt.x;
	//	m_vMousePos.y = pt.y;

	m_tRay.vDir.x = (m_vTargetPos.x * 2.f / vp.Width - 1.f) / (*pProj);
	m_tRay.vDir.y = (m_vTargetPos.y * -2.f / vp.Height + 1.f) / (*(pProj + 5));
	m_tRay.vDir.z = 1.f;

	m_tRay.vDir = m_tRay.vDir.Normalize();
	m_tRay.vPos = m_vStartPos;

	// 월드 공간으로 변환한다.
	XMVECTOR	v = XMVector3TransformNormal(m_tRay.vDir.Convert(), matView);
	m_tRay.vDir.Convert(v);
	m_tRay.vDir = m_tRay.vDir.Normalize();

	v = XMVector3TransformCoord(m_tRay.vPos.Convert(), matView);
	m_tRay.vPos.Convert(v);

	SAFE_RELEASE(pCamera);
	SAFE_RELEASE(pCameraObj);
	SAFE_RELEASE(pScene);

	return 0;
}

int CDx11ColliderRay::LateUpdate(float fTime)
{
	return 0;
}

void CDx11ColliderRay::Render(float fTime)
{
}

CDx11ColliderRay * CDx11ColliderRay::Clone()
{
	return new CDx11ColliderRay(*this);
}

bool CDx11ColliderRay::Collision(CDx11Collider * pColl)
{
	switch (pColl->GetCollType())
	{
	//case CST_SPHERE:
	//	return CollisionRayToSphere(m_tRay,
	//		((CDx11ColliderSphere*)pColl)->GetCenter(),
	//		((CDx11ColliderSphere*)pColl)->GetRadius());

	//case CST_OBB:
	//	return CollisionRayToSphere(m_tRay,
	//		((CDx11ColliderOBB*)pColl)->GetCenter(),
	//		((CDx11ColliderOBB*)pColl)->GetRadius());

	}
	return false;
}
