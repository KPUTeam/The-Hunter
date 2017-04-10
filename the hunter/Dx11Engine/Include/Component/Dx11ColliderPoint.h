#pragma once
#include "Dx11Collider.h"

DX11_BEGIN

class DX11_DLL CDx11ColliderPoint :
	public CDx11Collider
{
private:
	friend class CDx11GameObject;

private:
	CDx11ColliderPoint();
	CDx11ColliderPoint(const CDx11ColliderPoint& point);
	~CDx11ColliderPoint();

private:
	Vec2	m_vPoint;

public:
	Vec2 GetPoint() const;
	void SetPoint(const Vec2& vPoint);
	void SetPoint(const POINT& pt);


public:
	virtual bool Init();
	virtual void Input(float fTime);
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Render(float fTime);
	virtual CDx11ColliderPoint* Clone();

public:
	virtual bool Collision(CDx11Collider* pColl);
};

DX11_END