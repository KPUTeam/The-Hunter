#pragma once
#include "../Dx11Object.h"

DX11_BEGIN

class DX11_DLL CDx11RenderManager :
	public CDx11Object
{
	DX11_DECLARE_SINGLE(CDx11RenderManager)

private:
	unordered_map<string, class CDx11RenderState*>	m_mapState;
	vector<class CDx11GameObject*>	m_RenderList[RT_END];
	UINT		m_iObjCount;

public:
	void AddObjCount();
	void ReleaseObjCount();
	void AddRenderObject(class CDx11GameObject* pObj);
	void ClearRenderList();

public:
	bool Init();
	void Render(float fTime);
	class CDx11DepthStencilState* CreateDepthStencilState(const string& strKey,
		BOOL bDepthEnable, D3D11_DEPTH_WRITE_MASK eDepthMask,
		D3D11_COMPARISON_FUNC eDepthFunc);
	class CDx11RasterizerState* CreateRasterizerState(const string& strKey,
		D3D11_FILL_MODE eFillMode,
		D3D11_CULL_MODE eCullMode, BOOL bFrontCounterClockWise = FALSE,
		BOOL bDepthClipEnable = TRUE);
	class CDx11BlendState* CreateBlendState(const string& strKey,
		D3D11_BLEND eSrcBlend = D3D11_BLEND_SRC_ALPHA,
		D3D11_BLEND eDestBlend = D3D11_BLEND_INV_SRC_ALPHA,
		D3D11_BLEND_OP eBlendOp = D3D11_BLEND_OP_ADD,
		D3D11_BLEND eSrcAlphaBlend = D3D11_BLEND_ONE,
		D3D11_BLEND eDestAlphaBlend = D3D11_BLEND_ZERO,
		D3D11_BLEND_OP eAlphaBlendOp = D3D11_BLEND_OP_ADD,
		UINT8 iWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL,
		BOOL bAlphaEnable = FALSE, BOOL bIndependentEnable = FALSE);
	class CDx11BlendState* CreateBlendState(const string& strKey,
		const D3D11_RENDER_TARGET_BLEND_DESC* pTargetBlend,
		UINT iCount, BOOL bAlphaEnable = FALSE, BOOL bIndependentEnable = TRUE);

	class CDx11RenderState* FindRenderState(const string& strKey);

public:
	static bool SortDist(CDx11GameObject* pObj1, CDx11GameObject* pObj2);
	static bool SortDistInv(CDx11GameObject* pObj1, CDx11GameObject* pObj2);
	static bool SortUI(CDx11GameObject* pObj1, CDx11GameObject* pObj2);
};

DX11_END
