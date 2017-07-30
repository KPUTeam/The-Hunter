#include "DX11ColliderFrustrum.h"

#include "../GameObject/Dx11GameObject.h"
#include "Dx11Transform.h"
#include "Dx11Camera.h"
#include "../Resources/Dx11Mesh.h"
#include "../Resources/Dx11ResourcesManager.h"
#include "../Shader/Dx11Shader.h"
#include "../Scene/Dx11Scene.h"
#include "../Core/Dx11CollisionManager.h"
#include "Dx11ColliderSphere.h"

DX11_USING

CDX11ColliderFrustrum::CDX11ColliderFrustrum()
{
	m_eCollType = CST_FRUSTUM;
	SetTypeName<CDX11ColliderFrustrum>();
}

CDX11ColliderFrustrum::CDX11ColliderFrustrum(const CDX11ColliderFrustrum & frustrum)
{
	*this = frustrum;
}


CDX11ColliderFrustrum::~CDX11ColliderFrustrum()
{
}


void CDX11ColliderFrustrum::SetFrustrumScale(float x, float y, float z)
{
	m_vScale.x = x;
	m_vScale.y = y;
	m_vScale.z = z;
}

bool CDX11ColliderFrustrum::Init()
{
	// 앞면
	m_vPlaneVtx[0] = Vec3(-1, 1, 1);
	m_vPlaneVtx[1] = Vec3(1, 1, 1);
	m_vPlaneVtx[2] = Vec3(1, -1, 1);
	m_vPlaneVtx[3] = Vec3(-1, -1, 1);

	// 뒷면
	m_vPlaneVtx[4] = Vec3(-1, 1, 1.f);
	m_vPlaneVtx[5] = Vec3(1, 1, 1.f);
	m_vPlaneVtx[6] = Vec3(1, -1, 1.f);
	m_vPlaneVtx[7] = Vec3(-1, -1, 1.f);

	m_pMesh = DX11_GET_SINGLE(CDx11ResourcesManager)->FindMesh(FRUSTRUM_MESH);

	if (!CDx11Collider::Init())
		return false;

	return true;
}

void CDX11ColliderFrustrum::Input(float fTime)
{
}

int CDX11ColliderFrustrum::Update(float fTime)
{
	return 0;
}

int CDX11ColliderFrustrum::LateUpdate(float fTime)
{
	Vec3 vPos = m_pTransform->GetWorldPos();

	Matrix	s, r, t;

	// 스케일
	s = XMMatrixScaling(m_vScale.x, m_vScale.y, m_vScale.z);

	// 이동
	t = XMMatrixTranslation(vPos.x, vPos.y, vPos.z);

	// 회전
	// XMVECTOR q = XMQuaternionRotationRollPitchYaw(vRot.x, vRot.y, vRot.z);
	r = m_pTransform->GetWorldRotMatrix();

	m_matWorld = s*r*t;

	// 앞면
	m_vPlaneVtx[0] = Vec3(-1.f, 1.f, 1.f);
	m_vPlaneVtx[1] = Vec3(1.f, 1.f, 1.f);
	m_vPlaneVtx[2] = Vec3(1.f, - 1.f, 1.f);
	m_vPlaneVtx[3] = Vec3(-1.f, -1.f, 1.f);

	// 뒷면
	m_vPlaneVtx[4] = Vec3(-1.f, 1.f, 1.f);
	m_vPlaneVtx[5] = Vec3(1.f, 1.f, 1.f);
	m_vPlaneVtx[6] = Vec3(1.f, -1.f, 1.f);
	m_vPlaneVtx[7] = Vec3(-1.f, -1.f, 1.f);

	for (int i = 0; i < 8; ++i) {
		Vector v = m_vPlaneVtx[i].Convert();
		v = XMVector3TransformCoord(v, m_matWorld);


		m_vPlaneVtx[i].Convert(v);
	}

	return 0;
}

void CDX11ColliderFrustrum::Render(float fTime)
{
	if (!DX11_GET_SINGLE(CDx11CollisionManager)->GetColliderRender())
		return;

	CDx11GameObject*	pCameraObj = m_pScene->GetCamera();
	CDx11Camera*	pCamera = (CDx11Camera*)pCameraObj->FindComponentFromTypeName("CDx11Camera");

	TRANSFORMCBUFFER	tBuffer = {};

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

	m_pShader->UpdateConstantBuffer("Transform", &tBuffer, ST_VERTEX);

	CDx11Collider::Render(fTime);
}

CDX11ColliderFrustrum * CDX11ColliderFrustrum::Clone()
{
	return new CDX11ColliderFrustrum(*this);
}

bool CDX11ColliderFrustrum::Collision(CDx11Collider * pColl)
{
	switch (pColl->GetCollType())
	{
	case CST_SPHERE:

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

		return CollisionFrustrumToSphere(tFru, ((CDx11ColliderSphere*)pColl)->GetCenter(), ((CDx11ColliderSphere*)pColl)->GetRadius());
	}

	return false;
}
