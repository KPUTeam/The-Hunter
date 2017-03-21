#pragma once
#include "Dx11Component.h"

DX11_BEGIN

class DX11_DLL CDx11Camera :
	public CDx11Component
{
public:
	CDx11Camera();
	CDx11Camera(const CDx11Camera& camera);
	~CDx11Camera();

private:
	enum PROJECTION_TYPE
	{
		PT_PERSPECTIVE,
		PT_ORTHOGONAL
	};

private:
	Matrix		m_matView;
	Matrix		m_matProj;
	float		m_fAngle;
	float		m_fNear;
	float		m_fFar;
	PROJECTION_TYPE	m_eProjType;

public:
	Matrix GetViewMatrix();
	Matrix GetProjMatrix();

public:
	void SetProjection(float fAngle, float fNear, float fFar);
	void SetOrthoProjection(float n, float f);
	void SetAngle(float fAngle);
	void SetNear(float fNear);
	void SetDistance(float fDist);

public:
	virtual bool Init();
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Render(float fTime);
	virtual CDx11Camera* Clone();
};

DX11_END