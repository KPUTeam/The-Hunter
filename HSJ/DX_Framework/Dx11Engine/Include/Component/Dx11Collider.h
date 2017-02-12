#pragma once
#include "Dx11Component.h"

DX11_BEGIN

class DX11_DLL CDx11Collider :
	public CDx11Component
{
public:
	CDx11Collider();
	CDx11Collider(const CDx11Collider& collider);
	virtual ~CDx11Collider();
	
protected:
	COLLIDER_SUB_TYPE		m_eSubType;
	list<CDx11Collider*>	m_CollList;
	Vec3					m_vPrevPos;

public:
	COLLIDER_SUB_TYPE GetColliderType();

public:
	virtual bool Init();
	virtual void Input(float fTime);
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Render(float fTime);
	virtual CDx11Collider* Clone() = 0;

public:
	virtual bool Collision(CDx11Collider* pColl) = 0;

protected:
	bool CollisionSphereToSphere(const Vec3& vC1, float fR1, const Vec3& vC2,
		float fR2);
};

DX11_END
