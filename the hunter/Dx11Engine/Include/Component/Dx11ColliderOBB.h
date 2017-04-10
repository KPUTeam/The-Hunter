#pragma once
#include "Dx11Collider.h"

DX11_BEGIN

class DX11_DLL CDx11ColliderOBB :
	public CDx11Collider
{
public:
	CDx11ColliderOBB();
	CDx11ColliderOBB(const CDx11ColliderOBB& obb);
	~CDx11ColliderOBB();

private:
	Vec3		m_vCenter;
	Vec3		m_vRotation;
	Vec3		m_vAxis[3];
	float		m_fAxisLen[3];
	Vec3		m_vPrevPos;
	Vec3		m_vPrevRot;

public:
	void SetOBBInfo(const float fAxisLen[], 
		class CDx11Transform* pTransform);

public:
	virtual bool Init();
	virtual void Input(float fTime);
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Render(float fTime);
	virtual CDx11ColliderOBB* Clone();

public:
	virtual bool Collision(CDx11Collider* pColl);
};

DX11_END