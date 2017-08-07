#pragma once
#include "../Dx11Object.h"

DX11_BEGIN

class DX11_DLL CDx11RenderManager :
	public CDx11Object
{
	DX11_SINGLE_DECLARE(CDx11RenderManager)

private:
	unordered_map<string, class CDx11RenderState*>	m_mapRS;
	D3D11_RASTERIZER_DESC			m_tRasterizer;
	vector<class CDx11GameObject*>	m_vecRender[RG_END];

public:
	bool Init();
	void AddObject(class CDx11GameObject* pObj);
	void AddSky(class CDx11GameObject* pObj);
	void Render(float fTime);
	class CDx11RasterizerState* CreateRasterizerState(const string& strKey, D3D11_FILL_MODE eFillMode = D3D11_FILL_SOLID,
		D3D11_CULL_MODE eCullMode = D3D11_CULL_BACK, BOOL bClockWise = false, int iDepthBias = 0,
		float fDepthBiasClamp = 0.f, float fSlopeScaleDepthBias = 0.f,
		bool bDepthClipEnable = true, bool bScissorEnable = false,
		bool bMultiSampleEnable = false, bool bAntialiasedEnable = false);
	void SetFillMode(D3D11_FILL_MODE eFillMode);
	class CDx11DepthStencilState* CreateDepthStencilState(const string& strKey,
		bool bDepthEnable = true,
		D3D11_DEPTH_WRITE_MASK eWriteMask = D3D11_DEPTH_WRITE_MASK_ALL,
		D3D11_COMPARISON_FUNC eDepthFunc = D3D11_COMPARISON_LESS,
		bool bStencilEnable = false, UINT8 iReadMask = 0xff,
		UINT8 iWriteMask = 0xff);
	CDx11RenderState* FindRS(const string& strKey);

	class CDx11BlendState* CreateBlendState(const string& strKey,
		BOOL bEnable = TRUE, D3D11_BLEND eSrcBlend = D3D11_BLEND_SRC_ALPHA,
		D3D11_BLEND eDestBlend = D3D11_BLEND_INV_SRC_ALPHA,
		D3D11_BLEND_OP eBlendOp = D3D11_BLEND_OP_ADD,
		D3D11_BLEND eAlphaSrcBlend = D3D11_BLEND_ONE,
		D3D11_BLEND eAlphaDestBlend = D3D11_BLEND_ZERO,
		D3D11_BLEND_OP eAlphaBlendOp = D3D11_BLEND_OP_ADD,
		BOOL bAlphaToCoverage = FALSE,
		BOOL bIndependentBlend = FALSE,
		UINT8 iWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL);
	bool AddBlendState(const string& strKey,
		BOOL bEnable = TRUE, D3D11_BLEND eSrcBlend = D3D11_BLEND_SRC_ALPHA,
		D3D11_BLEND eDestBlend = D3D11_BLEND_INV_SRC_ALPHA,
		D3D11_BLEND_OP eBlendOp = D3D11_BLEND_OP_ADD,
		D3D11_BLEND eAlphaSrcBlend = D3D11_BLEND_ONE,
		D3D11_BLEND eAlphaDestBlend = D3D11_BLEND_ZERO,
		D3D11_BLEND_OP eAlphaBlendOp = D3D11_BLEND_OP_ADD,
		BOOL bAlphaToCoverage = FALSE,
		BOOL bIndependentBlend = FALSE,
		UINT8 iWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL);

public:
	static bool SortNormal(const class CDx11GameObject* pObj1,
		const class CDx11GameObject* pObj2);
	static bool SortAlpha(const class CDx11GameObject* pObj1,
		const class CDx11GameObject* pObj2);
	static bool SortUI(const class CDx11GameObject* pObj1,
		const class CDx11GameObject* pObj2);
};

DX11_END
