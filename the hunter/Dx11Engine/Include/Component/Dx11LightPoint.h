#pragma once
#include "Dx11Light.h"

DX11_BEGIN

class DX11_DLL CDx11LightPoint :
	public CDx11Light
{
public:
	CDx11LightPoint();
	CDx11LightPoint(const CDx11LightPoint& light);
	~CDx11LightPoint();

private:
//	위치와 범위를 갖는다.
	Vec3	m_vPos;
	float	m_fRange;

public:
	void SetPos(const Vec3& vPos);
	void SetRange(float fRange);

public:
	virtual bool Init();
	virtual void Input(float fTime);
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Render(float fTime);
	virtual CDx11LightPoint* Clone();

public:
	virtual void SetLight(class CDx11Shader* pShader);
};

DX11_END