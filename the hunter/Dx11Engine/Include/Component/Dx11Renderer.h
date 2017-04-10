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
	vector<vector<class CDx11Material*>>	m_vecMaterial;
	class CDx11RenderState* m_pRenderState[RST_END];
	RENDER_TYPE_FLAG		m_eRenderType;

public:
	RENDER_TYPE_FLAG GetRenderType() const;
	void SetRenderType(RENDER_TYPE_FLAG eRenderType);
	CDx11Material*	GetMaterial(int iContainer = 0, int iSubset = 0) const;

public:
	virtual bool Init();
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Render(float fTime);
	virtual CDx11Renderer* Clone();


public:
	void SetMesh(const string& strKey);
	void SetShader(const string& strKey);
	void SetMaterial(CDx11Material* pMtrl, int iContainer = 0);
	void CreateMaterial(const string& strDiffuseTexKey, int iContainer = 0,
		const string& strDiffuseSampKey = DEFAULT_SAMPLER);
	void CreateMaterial(const string& strDiffuseTexKey,
		TCHAR* pFileName, int iContainer = 0 ,const string& strPathKey = TEXTURE_PATH,
		const string& strDiffuseSampKey = DEFAULT_SAMPLER);
	void SetRenderState(const string& strKey);

private:
	void SetTransformCBuffer();
	void SetLightCBuffer();
	void SetMaterialCBuffer();
	void SetAnimationCBuffer();
};

DX11_END
