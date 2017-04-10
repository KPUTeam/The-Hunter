#pragma once
#include "Dx11Collider.h"

DX11_BEGIN

class DX11_DLL CDx11ColliderAABB :
	public CDx11Collider
{
public:
	CDx11ColliderAABB();
	CDx11ColliderAABB(const CDx11ColliderAABB& aabb);
	~CDx11ColliderAABB();

private:
	Vec3	m_vMin;
	Vec3	m_vMax;
	Vec3	m_vPrevPos;

public:
	Vec3 GetMin() const;
	Vec3 GetMax() const;
	void SetAABBInfo(const Vec3& vMin, const Vec3& vMax);

public:
	virtual bool Init();
	virtual void Input(float fTime);
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Render(float fTime);
	virtual CDx11ColliderAABB* Clone();

public:
	virtual bool Collision(CDx11Collider* pColl);
};

DX11_END