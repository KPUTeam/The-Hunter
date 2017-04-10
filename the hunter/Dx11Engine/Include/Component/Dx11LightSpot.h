#pragma once
#include "Dx11Light.h"

DX11_BEGIN

class DX11_DLL CDx11LightSpot :
	public CDx11Light
{
public:
	CDx11LightSpot();
	CDx11LightSpot(const CDx11LightSpot& light);
	~CDx11LightSpot();

private:
	Vec3	m_vPos;
	Vec3	m_vDir;
	float	m_fRange;
	float	m_fSpot;

public:
	void SetPos(const Vec3& vPos);
	void SetRange(float fRange);
	void SetDireciton(const Vec3& vDir);
	void SetSpot(float fSpot);

public:
	virtual bool Init();
	virtual void Input(float fTime);
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Render(float fTime);
	virtual CDx11LightSpot* Clone();

public:
	virtual void SetLight(class CDx11Shader* pShader);
};

DX11_END