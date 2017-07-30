#pragma once
#include "Dx11Component.h"

DX11_BEGIN

class DX11_DLL CDx11Collider :
	public CDx11Component
{
protected:
	CDx11Collider();
	CDx11Collider(const CDx11Collider& collider);
	virtual ~CDx11Collider() = 0;

protected:
	COLLIDER_SUB_TYPE						m_eCollType;
	function<int(CDx11Collider*, float)>	m_CallBack;
	bool									m_bCallBack;
	list<CDx11Collider*>					m_CollList;

	class CDx11Mesh*				m_pMesh;
	class CDx11RenderState*		m_pState;
	class CDx11Shader*				m_pShader;
	Matrix									m_matWorld;
	Vec4										m_vColor;
	bool										m_bSetColor;

public:
	void SetColor(const Vec4& vColor);
	void SetColor(float r, float g, float b, float a);
	void ColorEnable();

public:
	COLLIDER_SUB_TYPE GetCollType();

public:
	void AddCollision(CDx11Collider* pColl);
	bool CollsionInCollider(CDx11Collider* pColl);
	void EraseCollision(CDx11Collider* pColl);
	void DestroyObject();

public:
	virtual void SetTransform(class CDx11Transform* pTransform);

public:
	virtual bool Init();
	virtual int Update(float fTime) = 0;
	virtual int LateUpdate(float fTime) = 0;
	virtual void Render(float fTime);
	virtual CDx11Collider* Clone() = 0;

public:
	virtual bool Collision(CDx11Collider* pColl) = 0;;

protected:
	bool CollisionSphereToSphere(const Vec3& vC1, float fR1,
		const Vec3& vC2, float fR2)	const;
	bool CollisionRectToRect(const Vec2& vLT1, const Vec2& vRB1,
		const Vec2& vLT2, const Vec2& vRB2)	const;
	bool CollisionPointToRect(const Vec2& vPoint, const Vec2& vLT,
		const Vec2& vRB)	const;
	bool CollisionRayToSphere(const RAY& tRay, const Vec3& vCenter, float fRadius)	const;
	bool CollisionRayToTriangle(RAY& tRay, const Vec3& vP0, const Vec3& vP1,
		const Vec3& vP2)	const;
	bool CollisionAABB(const Vec3& vMin1, const Vec3& vMax1, const Vec3& vMin2, const Vec3& vMax2) const;
	bool CollisionOBB(const Vec3& vCenter1, const Vec3 vAxis1[], const float fAxisLen1[],
		const Vec3& vCenter2, const Vec3 vAxis2[], const float fAxisLen2[])const;
	bool CollisionFrustrumToSphere(Frustrum& f, const Vec3& c, float r)	const;
};

DX11_END
