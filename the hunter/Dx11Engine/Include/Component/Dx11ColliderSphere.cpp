#include "Dx11ColliderSphere.h"
#include "Dx11Transform.h"

//#if defined(_DEBUG) | defined(DEBUG)
#include "../Resources/Dx11Mesh.h"
#include "../Resources/Dx11ResourcesManager.h"
#include "../Rendering/Dx11Shader.h"
#include "../Scene/Dx11Scene.h"
#include "Dx11Camera.h"
#include "../GameObject/Dx11GameObject.h"
//#endif

DX11_USING

CDx11ColliderSphere::CDx11ColliderSphere()
{
	m_eSubType = CST_SPHERE;
	SetTypeName<CDx11ColliderSphere>();
}

CDx11ColliderSphere::CDx11ColliderSphere(const CDx11ColliderSphere & collider)	:
	CDx11Collider(collider)
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
//#if defined(DEBUG) | defined(_DEBUG)
	m_pMesh = DX11_GET_SINGLE(CDx11ResourcesManager)->FindMesh("SphereMesh");
//#endif

	if (!CDx11Collider::Init())
		return false;

	m_fRadius = 1.f;
	m_vCenter = Vec3Zero;

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

//#if defined(DEBUG) | defined(_DEBUG)
	Matrix	matScale, matTransform;

	matScale = XMMatrixScaling(m_fRadius * 2, m_fRadius * 2, m_fRadius * 2);
	matTransform = XMMatrixTranslation(m_vCenter.x, m_vCenter.y, m_vCenter.z);
	m_matWorld = matScale * matTransform;
//#endif

	return 0;
}

void CDx11ColliderSphere::Render(float fTime)
{
//#if defined(DEBUG) | defined(_DEBUG)
	CDx11GameObject*	pCameraObj = m_pScene->GetMainCamera();
	CDx11Camera*		pCamera = (CDx11Camera*)pCameraObj->FindComponentFromTypeName("CDx11Camera");

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
//#endif
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
