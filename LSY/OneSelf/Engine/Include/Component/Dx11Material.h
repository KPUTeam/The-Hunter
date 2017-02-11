#pragma once
#include "Dx11Component.h"

DX11_BEGIN

class DX11_DLL CDx11Material :
	public CDx11Component
{
private:
	friend class CDx11GameObject;
	friend class CDx11Renderer;

private:
	CDx11Material();
	~CDx11Material();

private:
	class CDx11Texture*		m_pDiffuseTex;
	class CDx11Sampler*		m_pDiffuseSampler;
	MATERIAL				m_tInfo;

public:
	void SetDiffuse(const Vec4& vDif);
	void SetAmbient(const Vec4& vAmb);
	void SetSpecular(const Vec4& vSpc);
	void SetEmissive(const Vec4& vEmv);
	void SetSpecularPower(float fPower);

public:
	virtual bool Init();
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Render(float fTime);
	virtual CDx11Material* Clone();

public:
	bool SetDiffuseTexture(const string& strKey, TCHAR* pFileName = NULL,
		const string& strPathKey = TEXTURE_PATH);
	bool SetDiffuseSampler(const string& strKey);
	void SetMaterial(class CDx11Shader* pShader, UINT iRegisterTex = 0, UINT iRegisterSampler = 0);
};

DX11_END
