#include "Dx11RenderManager.h"
#include "Dx11DepthStencilState.h"
#include "Dx11RasterizerState.h"
#include "Dx11BlendState.h"

DX11_USING

DX11_DEFINITION_SINGLE(CDx11RenderManager)

CDx11RenderManager::CDx11RenderManager()
{
}

CDx11RenderManager::~CDx11RenderManager()
{
	Safe_Release_Map(m_mapState);
}

bool CDx11RenderManager::Init()
{
	// 깊이 비교를 <=로 처리하는 상태를 하나 만들어준다.
	CDx11RenderState* pState = CreateDepthStencilState(DX11_RS_DEPTH_LESSEQUAL,
		TRUE, D3D11_DEPTH_WRITE_MASK_ALL,
		D3D11_COMPARISON_LESS_EQUAL);

	SAFE_RELEASE(pState);

	// Wireframe
	pState = CreateRasterizerState(DX11_RS_WIREFRAME, D3D11_FILL_WIREFRAME,
		D3D11_CULL_BACK);

	SAFE_RELEASE(pState);

	// Front Face Culling
	pState = CreateRasterizerState(DX11_RS_CULL_FRONTFACE,
		D3D11_FILL_SOLID, D3D11_CULL_FRONT);

	SAFE_RELEASE(pState);

	// Alpha Blending
	pState = CreateBlendState(DX11_RS_ALPHABLEND);

	SAFE_RELEASE(pState);

	return true;
}

CDx11DepthStencilState * CDx11RenderManager::CreateDepthStencilState(
	const string & strKey, BOOL bDepthEnable,
	D3D11_DEPTH_WRITE_MASK eDepthMask,
	D3D11_COMPARISON_FUNC eDepthFunc)
{
	CDx11DepthStencilState*	pState = (CDx11DepthStencilState*)FindRenderState(strKey);

	if (pState)
		return pState;

	pState = new CDx11DepthStencilState;

	if (!pState->CreateDepthStencilState(bDepthEnable, eDepthMask,
		eDepthFunc))
	{
		SAFE_RELEASE(pState);
		return NULL;
	}

	pState->AddRef();

	m_mapState.insert(make_pair(strKey, pState));

	return pState;
}

CDx11RasterizerState * CDx11RenderManager::CreateRasterizerState(const string & strKey,
	D3D11_FILL_MODE eFillMode, D3D11_CULL_MODE eCullMode,
	BOOL bFrontCounterClockWise, BOOL bDepthClipEnable)
{
	CDx11RasterizerState*	pState = (CDx11RasterizerState*)FindRenderState(strKey);

	if (pState)
		return pState;

	pState = new CDx11RasterizerState;

	if (!pState->CreateRasterizerState(eFillMode, eCullMode,
		bFrontCounterClockWise, bDepthClipEnable))
	{
		SAFE_RELEASE(pState);
		return NULL;
	}

	pState->AddRef();

	m_mapState.insert(make_pair(strKey, pState));

	return pState;
}

CDx11BlendState * CDx11RenderManager::CreateBlendState(const string & strKey,
	D3D11_BLEND eSrcBlend, D3D11_BLEND eDestBlend, D3D11_BLEND_OP eBlendOp,
	D3D11_BLEND eSrcAlphaBlend, D3D11_BLEND eDestAlphaBlend,
	D3D11_BLEND_OP eAlphaBlendOp, UINT8 iWriteMask, BOOL bAlphaEnable,
	BOOL bIndependentEnable)
{
	CDx11BlendState*	pState = (CDx11BlendState*)FindRenderState(strKey);

	if (pState)
		return pState;

	pState = new CDx11BlendState;

	if (!pState->CreateBlendState(eSrcBlend, eDestBlend, eBlendOp,
		eSrcAlphaBlend, eDestAlphaBlend, eAlphaBlendOp, iWriteMask, bAlphaEnable,
		bIndependentEnable))
	{
		SAFE_RELEASE(pState);
		return NULL;
	}

	pState->AddRef();

	m_mapState.insert(make_pair(strKey, pState));

	return pState;
}

CDx11BlendState * CDx11RenderManager::CreateBlendState(const string & strKey,
	const D3D11_RENDER_TARGET_BLEND_DESC * pTargetBlend, UINT iCount,
	BOOL bAlphaEnable, BOOL bIndependentEnable)
{
	CDx11BlendState*	pState = (CDx11BlendState*)FindRenderState(strKey);

	if (pState)
		return pState;

	pState = new CDx11BlendState;

	if (!pState->CreateBlendState(pTargetBlend, iCount, bAlphaEnable,
		bIndependentEnable))
	{
		SAFE_RELEASE(pState);
		return NULL;
	}

	pState->AddRef();

	m_mapState.insert(make_pair(strKey, pState));

	return pState;
}

CDx11RenderState * CDx11RenderManager::FindRenderState(const string & strKey)
{
	unordered_map<string, class CDx11RenderState*>::iterator	iter = m_mapState.find(strKey);

	if (iter == m_mapState.end())
		return NULL;

	iter->second->AddRef();

	return iter->second;
}
