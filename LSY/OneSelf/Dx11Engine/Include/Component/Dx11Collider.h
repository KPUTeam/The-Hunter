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

#if defined(_DEBUG) | defined(DEBUG)
	class CDx11Mesh*		m_pMesh;
	class CDx11Shader*		m_pShader;
	class CDx11RenderState* m_pWireFrame;
	Matrix					m_matWorld;
	Vec4					m_vColor;
#endif

public:
	COLLIDER_SUB_TYPE	GetColliderType();
	void AddCollider(CDx11Collider* pColl);
	void EraseCollider(CDx11Collider* pColl);
	bool CheckCollider(CDx11Collider* pColl);

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
	bool CollisionSphereToSphere(const Vec3& vC1, float fR1, const Vec3& vC2, float fR2);
};

DX11_END