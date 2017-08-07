#pragma once
#include "Dx11Component.h"

DX11_BEGIN

class DX11_DLL CDx11Light :
	public CDx11Component
{
protected:
	CDx11Light();
	CDx11Light(const CDx11Light& light);
	virtual ~CDx11Light() = 0;

protected:
	LIGHTINFO		m_tInfo;

public:
	void SetLightInfo(const Vec4& vDif, const Vec4& vAmb,
		const Vec4& vSpc);
	void SetDiffuse(const Vec4& vDif);
	void SetAmbient(const Vec4& vAmb);
	void SetSpecular(const Vec4& vSpc);
	void SetLightType(LIGHT_TYPE eType);

public:
	virtual void SetConstant(class CDx11Shader* pShader);

public:
	virtual bool Init();
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Render(float fTime);
	virtual CDx11Light* Clone() = 0;
};

DX11_END
