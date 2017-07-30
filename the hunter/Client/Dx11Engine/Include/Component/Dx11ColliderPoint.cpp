#include "Dx11ColliderPoint.h"
#include "Dx11ColliderRC.h"

DX11_USING

CDx11ColliderPoint::CDx11ColliderPoint()	:
	m_vPoint(0.f, 0.f)
{
	m_eCollType = CST_POINT;
	SetTypeName<CDx11ColliderPoint>();
}

CDx11ColliderPoint::CDx11ColliderPoint(const CDx11ColliderPoint & point)	:
	CDx11Collider(point)
{
	m_vPoint = point.m_vPoint;
}

CDx11ColliderPoint::~CDx11ColliderPoint()
{
}

Vec2 CDx11ColliderPoint::GetPoint() const
{
	return m_vPoint;
}

void CDx11ColliderPoint::SetPoint(const Vec2 & vPoint)
{
	m_vPoint = vPoint;
}

void CDx11ColliderPoint::SetPoint(const POINT & pt)
{
	m_vPoint = pt;
}

bool CDx11ColliderPoint::Init()
{
	return true;
}

int CDx11ColliderPoint::Update(float fTime)
{
	return 0;
}

int CDx11ColliderPoint::LateUpdate(float fTime)
{
	return 0;
}

void CDx11ColliderPoint::Render(float fTime)
{
}

CDx11ColliderPoint * CDx11ColliderPoint::Clone()
{
	return new CDx11ColliderPoint(*this);
}

bool CDx11ColliderPoint::Collision(CDx11Collider * pColl)
{
	switch (pColl->GetCollType())
	{
	case CST_POINT:
		return m_vPoint == ((CDx11ColliderPoint*)pColl)->m_vPoint;

	case CST_RC:
		return CollisionPointToRect(m_vPoint,
			((CDx11ColliderRC*)pColl)->GetLT(), 
			((CDx11ColliderRC*)pColl)->GetRB());
	}

	return false;
}
