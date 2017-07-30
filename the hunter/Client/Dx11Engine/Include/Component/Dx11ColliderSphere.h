#pragma once
#include "Dx11Collider.h"

DX11_BEGIN

class DX11_DLL CDx11ColliderSphere :
	public CDx11Collider
{
private:
	friend class CDx11GameObject;

private:
	CDx11ColliderSphere();
	CDx11ColliderSphere(const CDx11ColliderSphere& sphere);
	~CDx11ColliderSphere();

private:
	float		m_fRadius;
	Vec3		m_vCenter;
	Vec3		m_vPrevPos;

public:
	Vec3 GetCenter();
	float GetRadius();

public:
	virtual void SetTransform(CDx11Transform* pTransform);

public:
	void SetSphereInfo(float fRadius, const Vec3& vCenter);
	void SetRadius(float fRadius);
	void SetCenter(const Vec3& vCenter);

public:
	virtual bool Init();
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Render(float fTime);
	virtual CDx11ColliderSphere* Clone();

public:
	virtual bool Collision(CDx11Collider* pColl);
};

DX11_END
