#pragma once
#include "Dx11Component.h"

DX11_BEGIN

class DX11_DLL CDx11Renderer :
	public CDx11Component
{
private:
	friend class CDx11GameObject;

private:
	CDx11Renderer();
	CDx11Renderer(const CDx11Renderer& renderer);
	~CDx11Renderer();

private:
	class CDx11Mesh*		m_pMesh;
	class CDx11Shader*		m_pShader;
	class CDx11Material*	m_pMaterial;
	class CDx11RenderState*	m_pRenderState[RST_END];
	RENDER_TYPE				m_eRenderType;

public:
	RENDER_TYPE	GetRenderType()	const;
	void SetRenderType(RENDER_TYPE eRenderType);

public:
	virtual bool Init();
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Render(float fTime);
	virtual CDx11Renderer* Clone();

public:
	void SetMesh(const string& strKey);
	void SetShader(const string& strKey);
	void SetMaterial(CDx11Material* pMtrl);
	void CreateMaterial(const string& strDiffuseTexKey,
		const string& strDiffuseSampKey = DEFAULT_SAMPLER);
	void CreateMaterial(const string& strDiffuseTexKey,
		TCHAR* pFileName, const string& strPathKey = TEXTURE_PATH,
		const string& strDiffuseSampKey = DEFAULT_SAMPLER);
	void SetRenderState(const string& strKey);

private:
	void SetTransformCBuffer();
	void SetLightCBuffer();
	void SetMaterialCBuffer();
};

DX11_END