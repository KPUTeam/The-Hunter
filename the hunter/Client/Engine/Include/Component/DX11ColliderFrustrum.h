#pragma once
#include "Dx11Collider.h"

DX11_BEGIN


class DX11_DLL CDX11ColliderFrustrum :
	public CDx11Collider
{
	friend class CDx11GameObject;
	friend class CDx11ColliderSphere;

public:
	CDX11ColliderFrustrum();
	CDX11ColliderFrustrum(const CDX11ColliderFrustrum& frustrum);
	~CDX11ColliderFrustrum();

private:
	Vec3	m_vPlaneVtx[8];
	Vec3 m_vScale;

private:
	Vec3	m_vRot;
	Vec3 m_vPreRot;

public:
	void SetFrustrumScale(float x, float y, float z);

public:
	virtual bool Init();
	virtual void Input(float fTime);
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Render(float fTime);
	virtual CDX11ColliderFrustrum* Clone();

public:
	virtual bool Collision(CDx11Collider* pColl);

};

DX11_END