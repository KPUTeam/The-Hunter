#pragma once
#include "Dx11Component.h"

DX11_BEGIN

class DX11_DLL CDx11Material :
	public CDx11Component
{
private:
	friend class CDx11GameObject;
	friend class CDx11Renderer;
	friend class CDx11UIButton;
	friend class CDx11Mesh;

private:
	CDx11Material();
	CDx11Material(const CDx11Material& script);
	~CDx11Material();

private:
	class CDx11Texture*		m_pDiffuseTex;
	class CDx11Texture*		m_pSpecularTex;
	class CDx11Texture*		m_pNormalMapTex;
	MATERIAL					m_tMaterial;
	vector<class CDx11Texture*>	m_vecMultiTex;

public:
	void SetDiffuseTexture(const string& strKey);
	void SetDiffuseTexture(const string& strKey, TCHAR* pFileName,
		const string& strPathKey = DX11_TEXTURE_PATH);
	void SetDiffuseTexture(class CDx11Texture* pTexture);
	void SetDiffuseTexture(const string& strKey, const string& strPath);

	void SetSpecularTexture(const string& strKey, TCHAR* pFileName,
		const string& strPathKey = DX11_TEXTURE_PATH);
	void SetSpecularTexture(const string& strKey, const string& strPath);

	void SetNormalMapTexture(const string& strKey, TCHAR* pFileName,
		const string& strPathKey = DX11_TEXTURE_PATH);
	void SetNormalMapTexture(const string& strKey, const string& strPath);

	void SetDiffuse(const Vec4& vDif);
	void SetAmbient(const Vec4& vAmb);
	void SetSpecular(const Vec3& vSpc);
	void SetSpecular(const Vec4& vSpc);
	void SetEmissive(const Vec4& vEmv);
	void SetSpecularPower(float fPower);
	void SetMaterial(class CDx11Shader* pShader);
	void AddMultiTex(const string& strKey);
	void AddMultiTex(const string& strKey, TCHAR* pFileName,
		const string& strPathKey = DX11_TEXTURE_PATH);
	void AddMultiTex(class CDx11Texture* pTex);
	void DeleteMultiTex(const UINT& idx);
	void DeleteMultiTex(const string& strTag);
	void DeleteMultiTex(TCHAR* pFileName);

public:
	virtual bool Init();
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Render(float fTime);
	virtual CDx11Material* Clone();
};

DX11_END
