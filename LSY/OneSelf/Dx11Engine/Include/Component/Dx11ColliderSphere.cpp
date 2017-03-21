#include "Dx11ColliderSphere.h"
#include "Dx11Transform.h"

#if defined(_DEBUG) | defined(DEBUG)
#include "Dx11Camera.h"
#include "../Resources/Dx11Mesh.h"
#include "../Resources/Dx11ResourcesManager.h"
#include "../Rendering/Dx11Shader.h"
#include "../Scene/Dx11Scene.h"
#include "../GameObject/Dx11GameObject.h"
#endif
DX11_USING

CDx11ColliderSphere::CDx11ColliderSphere()
{
	m_eSubType = CST_SPHERE;
	SetTypeName<CDx11ColliderSphere>();
}

CDx11ColliderSphere::CDx11ColliderSphere(const CDx11ColliderSphere & collider)
	: CDx11Collider(collider)
{
	m_vCenter = collider.m_vCenter;
	m_fRadius = collider.m_fRadius;
}

CDx11ColliderSphere::~CDx11ColliderSphere()
{
}

void CDx11ColliderSphere::SetInfo(const Vec3 & vCenter, float fRadius)
{
	m_vCenter = vCenter;
	m_fRadius = fRadius;

	m_vPrevPos = m_pTransform->GetWorldPos();
}

bool CDx11ColliderSphere::Init()
{
	CDx11Collider::Init();

#if defined(_DEBUG) | defined(DEBUG)
	m_pMesh = DX11_GET_SINGLE(CDx11ResourcesManager)->FindMesh(SPHERE_MESH);
#endif

	return true;
}

void CDx11ColliderSphere::Input(float fTime)
{
}

int CDx11ColliderSphere::Update(float fTime)
{
	return 0;
}

int CDx11ColliderSphere::LateUpdate(float fTime)
{
	Vec3	vPos = m_pTransform->GetWorldPos();
	Vec3	vDist = vPos - m_vPrevPos;
	m_vCenter += vDist;
	m_vPrevPos = vPos;

#if defined(_DEBUG) | defined(DEBUG)
	Matrix	matScale, matTrans;
	matScale = XMMatrixScaling(m_fRadius * 2.f, m_fRadius * 2.f, m_fRadius * 2.f);
	matTrans = XMMatrixTranslation(m_vCenter.x, m_vCenter.y, m_vCenter.z);

	m_matWorld = matScale * matTrans;
#endif
	return 0;
}

void CDx11ColliderSphere::Render(float fTime)
{
#if defined(_DEBUG) | defined(DEBUG)
	CDx11GameObject*	pObj = m_pScene->GetMainCamera();
	CDx11Camera*	pCamera = (CDx11Camera*)pObj->FindComponentFromType(CT_CAMERA);

	TRANSFORMCBUFFER	tBuffer = {};
	tBuffer.matView = pCamera->GetViewMatrix();
	tBuffer.matProj = pCamera->GetProjMatrix();

	SAFE_RELEASE(pCamera);
	SAFE_RELEASE(pObj);

	tBuffer.matWorld = m_matWorld;

	tBuffer.matWV = tBuffer.matWorld * tBuffer.matView;
	tBuffer.matWVP = tBuffer.matWV * tBuffer.matProj;

	tBuffer.matWorld = XMMatrixTranspose(tBuffer.matWorld);
	tBuffer.matView = XMMatrixTranspose(tBuffer.matView);
	tBuffer.matProj = XMMatrixTranspose(tBuffer.matProj);

	tBuffer.matWV = XMMatrixTranspose(tBuffer.matWV);
	tBuffer.matWVP = XMMatrixTranspose(tBuffer.matWVP);

	m_pShader->UpdateConstantBuffer("Transform", &tBuffer, ST_VERTEX);
#endif

	CDx11Collider::Render(fTime);
}

CDx11ColliderSphere * CDx11ColliderSphere::Clone()
{
	return new CDx11ColliderSphere(*this);
}

bool CDx11ColliderSphere::Collision(CDx11Collider * pColl)
{
	switch (pColl->GetColliderType())
	{
	case CST_SPHERE:
		return CollisionSphereToSphere(m_vCenter, m_fRadius,
			((CDx11ColliderSphere*)pColl)->m_vCenter,
			((CDx11ColliderSphere*)pColl)->m_fRadius);
	}

	return false;
}
