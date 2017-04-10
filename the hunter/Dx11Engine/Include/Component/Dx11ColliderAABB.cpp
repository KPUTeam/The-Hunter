#include "Dx11ColliderAABB.h"
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

CDx11ColliderAABB::CDx11ColliderAABB() :
	m_vMin(Vec3Zero),
	m_vMax(Vec3Zero),
	m_vPrevPos(Vec3Zero)
{
	m_eSubType = CST_AABB;
	SetTypeName<CDx11ColliderAABB>();
}

CDx11ColliderAABB::CDx11ColliderAABB(const CDx11ColliderAABB & aabb)	:
	CDx11Collider(aabb)
{
	m_vMin = aabb.m_vMin;
	m_vMax = aabb.m_vMax;
}

CDx11ColliderAABB::~CDx11ColliderAABB()
{
}

Vec3 CDx11ColliderAABB::GetMin() const
{
	return m_vMin;
}

Vec3 CDx11ColliderAABB::GetMax() const
{
	return m_vMax;
}

void CDx11ColliderAABB::SetAABBInfo(const Vec3 & vMin, const Vec3 & vMax)
{
	m_vMin = vMin;
	m_vMax = vMax;
}

bool CDx11ColliderAABB::Init()
{
//#if defined(DEBUG) | defined(_DEBUG)
	m_pMesh = DX11_GET_SINGLE(CDx11ResourcesManager)->FindMesh("CubeMesh");
//#endif

	if (!CDx11Collider::Init())
		return false;

	m_vMin = Vec3Zero;
	m_vMax = Vec3Zero;
	m_vPrevPos = Vec3Zero;

	return true;
}

void CDx11ColliderAABB::Input(float fTime)
{
}

int CDx11ColliderAABB::Update(float fTime)
{
	return 0;
}

int CDx11ColliderAABB::LateUpdate(float fTime)
{
	Vec3	vPos = m_pTransform->GetWorldPos();
	Vec3	vDist = vPos - m_vPrevPos;
	m_vMin += vDist;
	m_vMax += vDist;
	m_vPrevPos = vPos;

//#if defined(DEBUG) | defined(_DEBUG)
	Matrix	matScale, matTranslate;
	Vec3	vCenter = Vec3(vPos.x, vPos.y, vPos.z);
	float	fDist = m_vMin.Distance(m_vMax);
	matScale = XMMatrixScaling(fDist / 2, fDist / 2, fDist / 2);
	matTranslate = XMMatrixTranslation(vCenter.x, vCenter.y, vCenter.z);
	m_matWorld = matScale * matTranslate;
//#endif

	return 0;
}

void CDx11ColliderAABB::Render(float fTime)
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

CDx11ColliderAABB * CDx11ColliderAABB::Clone()
{
	return new CDx11ColliderAABB(*this);
}

bool CDx11ColliderAABB::Collision(CDx11Collider * pColl)
{
	switch (pColl->GetColliderType())
	{
	case CST_AABB:
		return CollisionAABB(m_vMin, m_vMax,
			((CDx11ColliderAABB*)pColl)->m_vMin,
			((CDx11ColliderAABB*)pColl)->m_vMax);
	}
	return false;
}
