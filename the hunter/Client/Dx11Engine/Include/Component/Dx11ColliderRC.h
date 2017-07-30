#pragma once
#include "Dx11Collider.h"

DX11_BEGIN

class DX11_DLL CDx11ColliderRC :
	public CDx11Collider
{
private:
	friend class CDx11GameObject;

public:
	CDx11ColliderRC();
	CDx11ColliderRC(const CDx11ColliderRC& rc);
	~CDx11ColliderRC();

private:
	Vec2		m_vLT;
	Vec2		m_vRB;
	Vec3		m_vPrevPos;

	class CDx11RenderState*	m_pDepthState;


public:
	void SetRCInfo(float l, float t, float r, float b);
	void SetRCInfo();
	Vec2 GetLT();
	Vec2 GetRB();

public:
	virtual bool Init();
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Render(float fTime);
	virtual CDx11ColliderRC* Clone();

public:
	virtual bool Collision(CDx11Collider* pColl);
};

DX11_END
