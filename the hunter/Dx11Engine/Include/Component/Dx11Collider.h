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
//#if defined(DEBUG)	| defined(_DEBUG)
	class CDx11Mesh*		m_pMesh;
	class CDx11RenderState*	m_pState;
	class CDx11Shader*		m_pShader;
	Matrix					m_matWorld;
	Vec4					m_vColor;
	bool					m_bSetColor;
//#endif

public:
	void SetColor(const Vec4& vColor);
	void ColorEnable();

public:
	COLLIDER_SUB_TYPE GetColliderType();
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
	bool CollisionSphereToSphere(const Vec3& vC1, float fR1,
		const Vec3& vC2, float fR2);
	bool CollisionRectToRect(const Vec2& vLT1, const Vec2& vRB1,
		const Vec2& vLT2, const Vec2& vRB2);
	bool CollisionPointToRect(const Vec2& vPoint, const Vec2& vLT,
		const Vec2& vRB);
	bool CollisionAABB(const Vec3& vMin1, const Vec3& vMax1, 
		const Vec3& vMin2, const Vec3& vMax2);
	bool CollisionOBB(const Vec3& vCenter1, const Vec3 vAxis1[], const float fAxisLen1[],
		const Vec3& vCenter2, const Vec3 vAxis2[], const float fAxisLen2[]);
};

DX11_END

