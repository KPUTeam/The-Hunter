#pragma once
#include "Dx11Collider.h"

DX11_BEGIN

class DX11_DLL CDx11ColliderRay :
	public CDx11Collider
{
private:
	friend class CDx11GameObject;

private:
	CDx11ColliderRay();
	CDx11ColliderRay(const CDx11ColliderRay& ray);
	~CDx11ColliderRay();

private:
	RAY		m_tRay;
	Vec2	m_vMousePos;
	Vec2	m_vTargetPos;
	Vec3	m_vStartPos;

public:
	RAY	GetRay() const;

public:
	void SetMousePos(const POINT& ptPos);
	void SetTargetPos(const POINT& ptPos);
	void SetStartPos(const Vec3& vPos);

public:
	virtual bool Init();
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Render(float fTime);
	virtual CDx11ColliderRay* Clone();

public:
	virtual bool Collision(CDx11Collider* pColl);

};

DX11_END