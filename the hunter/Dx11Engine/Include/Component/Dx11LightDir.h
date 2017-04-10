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
//	Directional Light는 방향값만 가지고 있으면 된다.
	Vec3		m_vDir;

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