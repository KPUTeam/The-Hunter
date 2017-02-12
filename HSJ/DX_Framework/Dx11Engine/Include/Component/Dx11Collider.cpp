#include "Dx11Collider.h"

DX11_USING

CDx11Collider::CDx11Collider()
{
	m_eComType = CT_COLLIDER;
}

CDx11Collider::CDx11Collider(const CDx11Collider & collider)	:
	CDx11Component(collider)
{
	m_eSubType = collider.m_eSubType;
	m_vPrevPos = collider.m_vPrevPos;
}

CDx11Collider::~CDx11Collider()
{
}

COLLIDER_SUB_TYPE CDx11Collider::GetColliderType()
{
	return m_eSubType;
}

bool CDx11Collider::Init()
{
	return true;
}

void CDx11Collider::Input(float fTime)
{
}

int CDx11Collider::Update(float fTime)
{
	return 0;
}

int CDx11Collider::LateUpdate(float fTime)
{
	return 0;
}

void CDx11Collider::Render(float fTime)
{
}

bool CDx11Collider::CollisionSphereToSphere(const Vec3 & vC1, float fR1,
	const Vec3 & vC2, float fR2)
{
	float fDist = vC1.Distance(vC2);

	return fDist <= fR1 + fR2;
}
