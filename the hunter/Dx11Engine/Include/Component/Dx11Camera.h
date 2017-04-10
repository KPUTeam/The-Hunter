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
	int			m_iShakeTimes;
	PROJECTION_TYPE		m_eProjType;
	CAMERA_VIEW_TYPE	m_eCameraType;
	Vec3					m_vPrevPos;
	class CDx11GameObject*	m_pTarget;

private:
	float		m_fMin;
	float		m_fMax;
	float		m_fMouseSens;
	Vec2		m_vMouseGap;

public:
	Matrix GetViewMatrix();
	Matrix GetProjMatrix();
	int	GetShakeTimes();
	CDx11GameObject* GetTarget() const;
	Vec2 GetMouseGap() const;
	

public:
	void SetProjection(float fAngle, float fNear, float fFar);
	void SetOrthoProjection(float fNear, float fFar);
	void SetAngle(float fAngle);
	void SetNear(float fNear);
	void SetDistance(float fDist);
	void SetShakeTimes(int iTimes);
	void SetTarget(class CDx11GameObject* pTarget);
	void ShakeCamera();
	void TraceTarget();

public:
	void SetMinMax(float fMin, float fMax);
	void ZoomInOut(class CDx11Mouse* pMouse);
	void RotateCamera(class CDx11Mouse* pMouse);
	void SetCameraType(CAMERA_VIEW_TYPE eType);

public:
	virtual bool Init();
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Render(float fTime);
	virtual CDx11Camera* Clone();
};

DX11_END