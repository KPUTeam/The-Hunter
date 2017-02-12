#include "Dx11ColliderSphere.h"
#include "Dx11Transform.h"

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

	return 0;
}

void CDx11ColliderSphere::Render(float fTime)
{
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
