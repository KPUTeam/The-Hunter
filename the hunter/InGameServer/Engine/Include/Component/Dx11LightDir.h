#pragma once
#include "Dx11Light.h"

DX11_BEGIN

class DX11_DLL CDx11LightDir :
	public CDx11Light
{
private:
	friend class CDx11GameObject;

private:
	CDx11LightDir();
	CDx11LightDir(const CDx11LightDir& light);
	~CDx11LightDir();

private:
	Vec4	m_vDir;

public:
	void SetLightDir(const Vec3& vDir);

public:
	virtual void SetConstant(class CDx11Shader* pShader);

public:
	virtual bool Init();
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Render(float fTime);
	virtual CDx11LightDir* Clone();
};

DX11_END
