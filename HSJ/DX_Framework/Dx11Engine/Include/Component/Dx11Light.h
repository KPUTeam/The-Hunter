#pragma once
#include "Dx11Component.h"

DX11_BEGIN

class DX11_DLL CDx11Light :
	public CDx11Component
{
protected:
	CDx11Light();
	CDx11Light(const CDx11Light& light);
	virtual ~CDx11Light();

protected:
	LIGHTINFO		m_tInfo;

public:
	void SetDiffuse(const Vec4& vDif);
	void SetAmbient(const Vec4& vAmb);
	void SetSpecular(const Vec4& vSpc);

public:
	virtual bool Init();
	virtual void Input(float fTime);
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Render(float fTime);
	virtual CDx11Light* Clone() = 0;

public:
	virtual void SetLight(class CDx11Shader* pShader);
};

DX11_END
