#pragma once
#include "Dx11Collider.h"

DX11_BEGIN

class DX11_DLL CDx11ColliderRect :
	public CDx11Collider
{
public:
	CDx11ColliderRect();
	CDx11ColliderRect(const CDx11ColliderRect& rect);
	~CDx11ColliderRect();

	
public:
	Vec2		m_vLT;	//Left	Top
	Vec2		m_vRB;	//Right	Bottom
	Vec3		m_vPrevPos;

public:
	void SetRectInfo(float l, float t, float r, float b);
	Vec2 GetLT();
	Vec2 GetRB();

public:
	virtual bool Init();
	virtual void Input(float fTime);
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Render(float fTime);
	virtual CDx11ColliderRect* Clone();

public:
	virtual bool Collision(CDx11Collider* pColl);
};

DX11_END