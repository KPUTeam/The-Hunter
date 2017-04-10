#pragma once
#include "Dx11Component.h"

DX11_BEGIN

class DX11_DLL CDx11Material :
	public CDx11Component
{
private:
	friend class CDx11GameObject;
	friend class CDx11Renderer;
	friend class CDx11Mesh;

private:
	CDx11Material();
	CDx11Material(const CDx11Material& mat);
	~CDx11Material();

private:
	class CDx11Texture*		m_pDiffuseTex;
	class CDx11Texture*		m_pSpecularTex;
	class CDx11Texture*		m_pNormalMapTex;
	class CDx11Sampler*		m_pDiffuseSampler;
	MATERIAL				m_tInfo;

public:
	void SetInfo(const Vec3& vDif, const Vec3& vAmb, const Vec3& vSpc,
		const Vec3& vEmv, float fPower);
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
	bool SetDiffuseTexture(class CDx11Texture* pTexture);
	bool SetDiffuseTexture(const string& strKey, const string& strPath);

	bool SetSpecularTexture(const string& strKey, TCHAR* pFileName = NULL,
		const string& strPathKey = TEXTURE_PATH);
	bool SetSpecularTexture(const string& strKey, const string& strPath);
	
	bool SetNormalMapTexture(const string& strKey, TCHAR* pFileName = NULL,
		const string& strPathKey = TEXTURE_PATH);
	bool SetNormalMapTexture(const string& strKey, const string& strPath);

	bool SetDiffuseSampler(const string& strKey);
	void SetMaterial(class CDx11Shader* pShader, UINT iRegisterTex = 0, UINT iRegisterSampler = 0);
};

DX11_END