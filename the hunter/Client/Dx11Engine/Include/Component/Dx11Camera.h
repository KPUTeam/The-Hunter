#pragma once
#include "Dx11Component.h"

DX11_BEGIN

class DX11_DLL CDx11Camera :
	public CDx11Component
{
private:
	friend class CDx11GameObject;

private:
	CDx11Camera();
	CDx11Camera(const CDx11Camera& camera);
	~CDx11Camera();

private:
	Matrix		m_matView;
	Matrix		m_matProj;
	Vec3		m_vPrevPos;
	float		m_fViewAngle;
	float		m_fNear;
	float		m_fViewDist;
	CAMERA_TYPE	m_eCamType;
	CDx11GameObject*	m_pTarget;
	Frustrum	m_tFrustrum;

	float		m_fMin;
	float		m_fMax;

	int			m_iShakeTimes;

public:
	void SetShakeTimes(int iTimes);
	int GetShakeTimes();
	void ShakeCamera();

	Matrix GetViewMatrix()	const;
	Matrix GetProjMatrix()	const;
	CAMERA_TYPE GetCameraType()	const;
	float GetCameraDist()	const;
	CDx11GameObject* GetTarget()	const;

	void SetMinMax(float fMin, float fMax);

public:
	void ZoomInOut(class CDx11Mouse* pMouse);
	void SetTarget(CDx11GameObject* pTarget);
	void SetProjection(float fViewAngle, float fNear, float fViewDist);
	void SetOrthoProjection(float fWidth, float fHeight, float fNear, float fViewDist);
	void SetCameraType(CAMERA_TYPE eType);
	void TargetTrace();

	// Frustrum
	void CalculateFrustrum();
	Frustrum* GetFrustrum();

private:
	void ComputeViewMatrix();

public:
	virtual bool Init();
	void RotateLeftRight(float fGap);
	void RotateUpDown(float fGap);
	void RotateCamera(CDx11Mouse * pMouse);
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Render(float fTime);
	virtual CDx11Camera* Clone();
};

DX11_END
