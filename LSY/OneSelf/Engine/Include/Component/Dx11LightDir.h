#pragma once
#include "Dx11Light.h"

DX11_BEGIN

class CDx11LightDir :
	public CDx11Light
{
private:
	friend class CDx11GameObject;

private:
	CDx11LightDir();
	CDx11LightDir(const CDx11LightDir& LightDir);
	~CDx11LightDir();

private:
	Vec3	m_vDir;

public:
	virtual bool Init();
	virtual void Input(float fTime);
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Render(float fTime);
	virtual CDx11LightDir* Clone();

public:
	virtual void SetLight(class CDx11Shader* pShader);
};

DX11_END