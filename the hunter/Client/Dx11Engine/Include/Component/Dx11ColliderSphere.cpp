#include "Dx11ColliderSphere.h"
#include "../Resources/Dx11ResourcesManager.h"
#include "Dx11Transform.h"
#include "../Scene/Dx11Scene.h"
#include "Dx11Camera.h"
#include "../GameObject/Dx11GameObject.h"
#include "../Shader/Dx11Shader.h"
#include "Dx11ColliderRay.h"
#include "Dx11ColliderTerrain.h"
#include "../Core/Dx11CollisionManager.h"
#include"DX11ColliderFrustrum.h"

DX11_USING

CDx11ColliderSphere::CDx11ColliderSphere()
{
	m_eCollType = CST_SPHERE;
	SetTypeName<CDx11ColliderSphere>();
}

CDx11ColliderSphere::CDx11ColliderSphere(const CDx11ColliderSphere & sphere)	:
	CDx11Collider(sphere)
{
	*this = sphere;
	m_iRef = 1;
}

CDx11ColliderSphere::~CDx11ColliderSphere()
{
}

Vec3 CDx11ColliderSphere::GetCenter()
{
	return m_vCenter;
}

float CDx11ColliderSphere::GetRadius()
{
	return m_fRadius;
}

void CDx11ColliderSphere::SetTransform(CDx11Transform * pTransform)
{
	CDx11Collider::SetTransform(pTransform);

	m_vPrevPos = pTransform->GetWorldPos();
}

void CDx11ColliderSphere::SetSphereInfo(float fRadius, const Vec3 & vCenter)
{
	m_fRadius = fRadius;
	m_vCenter = vCenter;
}

void CDx11ColliderSphere::SetRadius(float fRadius)
{
	m_fRadius = fRadius;
}

void CDx11ColliderSphere::SetCenter(const Vec3 & vCenter)
{
	m_vCenter = vCenter;
}

bool CDx11ColliderSphere::Init()
{
	m_pMesh = DX11_GET_SINGLE(CDx11ResourcesManager)->FindMesh("ColliderSphere");

	if (!CDx11Collider::Init())
		return false;

	m_fRadius = 1.f;
	m_vCenter = VEC3ZERO;

	return true;
}

int CDx11ColliderSphere::Update(float fTime)
{
	return 0;
}

int CDx11ColliderSphere::LateUpdate(float fTime)
{
	Vec3	vPos = m_pTransform->GetWorldPos();
	Vec3	vDist = vPos - m_vPrevPos;

	m_vPrevPos = vPos;

	m_vCenter += vDist;

	Matrix	matScale, matTrans;

	matScale = XMMatrixScaling(m_fRadius, m_fRadius, m_fRadius);
	matTrans = XMMatrixTranslation(m_vCenter.x, m_vCenter.y, m_vCenter.z);
	m_matWorld = matScale * matTrans;

	return 0;
}

void CDx11ColliderSphere::Render(float fTime)
{
	if (!DX11_GET_SINGLE(CDx11CollisionManager)->GetColliderRender())
		return;

	TRANSFORMCBUFFER	tBuffer = {};

	CDx11GameObject*	pCameraObj = m_pScene->GetCamera();
	CDx11Camera*	pCamera = (CDx11Camera*)pCameraObj->FindComponentFromTypeName("CDx11Camera");

	tBuffer.matWorld = m_matWorld;
	tBuffer.matView = pCamera->GetViewMatrix();
	tBuffer.matProj = pCamera->GetProjMatrix();
	tBuffer.matWV = m_matWorld * tBuffer.matView;
	tBuffer.matWVP = tBuffer.matWV * tBuffer.matProj;

	tBuffer.matWorld = XMMatrixTranspose(tBuffer.matWorld);
	tBuffer.matView = XMMatrixTranspose(tBuffer.matView);
	tBuffer.matProj = XMMatrixTranspose(tBuffer.matProj);
	tBuffer.matWV = XMMatrixTranspose(tBuffer.matWV);
	tBuffer.matWVP = XMMatrixTranspose(tBuffer.matWVP);

	SAFE_RELEASE(pCamera);
	SAFE_RELEASE(pCameraObj);
	if (m_pShader)
	{
		m_pShader->UpdateConstantBuffer("Transform", &tBuffer,
			ST_VERTEX);
	}

	CDx11Collider::Render(fTime);
}

CDx11ColliderSphere * CDx11ColliderSphere::Clone()
{
	return new CDx11ColliderSphere(*this);
}

bool CDx11ColliderSphere::Collision(CDx11Collider * pColl)
{
	switch (pColl->GetCollType())
	{
	case CST_SPHERE:
		return CollisionSphereToSphere(m_vCenter, m_fRadius, 
			((CDx11ColliderSphere*)pColl)->m_vCenter,
			((CDx11ColliderSphere*)pColl)->m_fRadius);

	case CST_RAY:
		return CollisionRayToSphere(((CDx11ColliderRay*)pColl)->GetRay(), m_vCenter, m_fRadius);

	case CST_TERRAIN:
		return pColl->Collision((CDx11Collider*)this);

	case CST_FRUSTUM:
		Vec3 m_vPlaneVtx[8] = {};
		m_vPlaneVtx[0] = ((CDX11ColliderFrustrum*)pColl)->m_vPlaneVtx[0];
		m_vPlaneVtx[1] = ((CDX11ColliderFrustrum*)pColl)->m_vPlaneVtx[1];
		m_vPlaneVtx[2] = ((CDX11ColliderFrustrum*)pColl)->m_vPlaneVtx[2];
		m_vPlaneVtx[3] = ((CDX11ColliderFrustrum*)pColl)->m_vPlaneVtx[3];
		m_vPlaneVtx[4] = ((CDX11ColliderFrustrum*)pColl)->m_vPlaneVtx[4];
		m_vPlaneVtx[5] = ((CDX11ColliderFrustrum*)pColl)->m_vPlaneVtx[5];
		m_vPlaneVtx[6] = ((CDX11ColliderFrustrum*)pColl)->m_vPlaneVtx[6];
		m_vPlaneVtx[7] = ((CDX11ColliderFrustrum*)pColl)->m_vPlaneVtx[7];

		Frustrum	tFru;

		tFru.m_vPlane[0] = XMPlaneFromPoints(m_vPlaneVtx[0].Convert(), m_vPlaneVtx[1].Convert(), m_vPlaneVtx[2].Convert());
		tFru.m_vPlane[1] = XMPlaneFromPoints(m_vPlaneVtx[4].Convert(), m_vPlaneVtx[7].Convert(), m_vPlaneVtx[6].Convert());
		tFru.m_vPlane[2] = XMPlaneFromPoints(m_vPlaneVtx[0].Convert(), m_vPlaneVtx[3].Convert(), m_vPlaneVtx[7].Convert());
		tFru.m_vPlane[3] = XMPlaneFromPoints(m_vPlaneVtx[1].Convert(), m_vPlaneVtx[5].Convert(), m_vPlaneVtx[6].Convert());
		tFru.m_vPlane[4] = XMPlaneFromPoints(m_vPlaneVtx[0].Convert(), m_vPlaneVtx[4].Convert(), m_vPlaneVtx[5].Convert());
		tFru.m_vPlane[5] = XMPlaneFromPoints(m_vPlaneVtx[3].Convert(), m_vPlaneVtx[2].Convert(), m_vPlaneVtx[6].Convert());

		tFru.vCenter.x = (tFru.vPos[0].x + tFru.vPos[1].x) / 2;
		tFru.vCenter.y = (tFru.vPos[0].y + tFru.vPos[3].y) / 2;
		tFru.vCenter.z = (tFru.vPos[0].z + tFru.vPos[4].z) / 2;

		return	CollisionFrustrumToSphere(tFru, m_vCenter, m_fRadius);
	}

	return false;
}
