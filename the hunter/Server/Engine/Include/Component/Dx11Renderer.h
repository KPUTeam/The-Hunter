#pragma once
#include "Dx11Component.h"

DX11_BEGIN

typedef struct _tagCBuffer
{
	string		strKey;
	void*		pData;
	int			iSize;
	int			iShaderType;
}CBUFFER, *PCBUFFER;

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
	class CDx11Mesh*	m_pMesh;
	class CDx11Shader*	m_pShader;
	vector<vector<class CDx11Material*>>	m_vecMaterial;
	class CDx11RenderState*	m_pRenderState[RSF_MAX];
	string		m_strCameraTag;
	unordered_map<string, PCBUFFER>		m_mapCBuffer;
	class CDx11Texture*		m_pBoneTexture;

public:
	CDx11Material* GetMaterial(UINT iContainer = 0, UINT iSubset = 0);
	bool IsRender2D()	const;
	bool AddCBuffer(const string& strKey, const string& strCBufferKey,
		int iSize, int iShaderType);
	PCBUFFER FindCBuffer(const string& strKey);
	bool UpdateCBuffer(const string& strKey, void* pData, int iSize);
	void UpdateCBuffer();
	void SetBoneTexture(class CDx11Texture* pBoneTex);

public:
	virtual bool Init();
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Render(float fTime);
	virtual CDx11Renderer* Clone();

public:
	class CDx11Mesh* GetMesh()	const;

public:
	void SetCameraTag(const string& strTag);
	void SetMesh(CDx11Mesh* pMesh);
	void SetMesh(const string& strKey);
	void SetMesh(const string& strKey, TCHAR* pFileName,
		const string& strPathKey = DX11_MESH_PATH);
	void SetMeshFromFullPath(const string& strKey, const wchar_t* pFullPath);
	void SetShader(CDx11Shader* pShader);
	void SetShader(const string& strKey);
	void AddMaterial(CDx11Material* pMtrl, int iContainer = 0);
	void DeleteMaterial(UINT iContainer = 0, UINT iSubset = 0);
	void CreateMaterial(const string& strDiffTexKey, UINT iContainer = 0);
	void CreateMaterial(const string& strDiffTexKey, TCHAR* pFileName,
		const string& strPathKey = DX11_TEXTURE_PATH, UINT iContainer = 0);
	void SetMaterialDiffuseTex(const string& strKey, const UINT& iContainer = 0,
		UINT iSubset = 0);
	void SetMaterialDiffuseTexFromLoad(const string& strKey, TCHAR* pFileName,
		const string& strPathKey = DX11_TEXTURE_PATH, const UINT& iContainer = 0,
		UINT iSubset = 0);
	void AddMultiTex(const string& strKey, const UINT& iContainer = 0,
		UINT iSubset = 0);
	void AddMultiTex(const string& strKey, TCHAR* pFileName,
		const string& strPathKey = DX11_TEXTURE_PATH, const UINT& iContainer = 0,
		UINT iSubset = 0);

	void DeleteMultiTex(int idx, const UINT& iContainer = 0,
		UINT iSubset = 0);
	void DeleteMultiTex(const string& strTag, const UINT& iContainer = 0,
		UINT iSubset = 0);
	void DeleteMultiTex(TCHAR* pFileName, const UINT& iContainer = 0,
		UINT iSubset = 0);

	void AddMultiTex(class CDx11Texture* pTex, const UINT& iContainer = 0,
		UINT iSubset = 0);
	void SetRenderState(const string& strKey, RENDERSTATE_FLAG eFlag);
	bool IsBlendRender();

private:
	void UpdateTransform();
	void UpdateLight();
	void UpdateMaterial();
	void UpdateRenderState();
	void UpdateAnimation();
	void ResetRenderState();
};

DX11_END
