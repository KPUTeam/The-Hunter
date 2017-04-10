#include "Dx11BlendState.h"
#include "../Core/Dx11Device.h"

DX11_USING

CDx11BlendState::CDx11BlendState()
{
	m_eType = RST_BLEND;
	m_iOldSampleMask = 0xffffffff;
	memset(m_fBlendFactor, 0, sizeof(float) * 4);
	memset(m_fOldBlendFactor, 0, sizeof(float) * 4);
}


CDx11BlendState::~CDx11BlendState()
{
}

bool CDx11BlendState::CreateBlendState(D3D11_BLEND eSrcBlend, 
	D3D11_BLEND eDestBlend, D3D11_BLEND_OP eBlendOp, 
	D3D11_BLEND eSrcAlphaBlend, D3D11_BLEND eDestAlphaBlend,
	D3D11_BLEND_OP eAlphaBlendOp, UINT8 iWriteMask, 
	BOOL bAlphaEnable, BOOL bIndependentEnable)
{
	m_tDesc.AlphaToCoverageEnable = bAlphaEnable;
	m_tDesc.IndependentBlendEnable = bIndependentEnable;
	m_tDesc.RenderTarget[0].BlendEnable = true;
	m_tDesc.RenderTarget[0].BlendOp = eBlendOp;
	m_tDesc.RenderTarget[0].BlendOpAlpha = eAlphaBlendOp;
	m_tDesc.RenderTarget[0].SrcBlend = eSrcBlend;
	m_tDesc.RenderTarget[0].DestBlend = eDestBlend;
	m_tDesc.RenderTarget[0].SrcBlendAlpha = eSrcAlphaBlend;
	m_tDesc.RenderTarget[0].DestBlendAlpha = eDestAlphaBlend;
	m_tDesc.RenderTarget[0].RenderTargetWriteMask = iWriteMask;

	if (FAILED(DX11_DEVICE->CreateBlendState(&m_tDesc,
		(ID3D11BlendState**)&m_pState)))
		return false;

	return true;
}

bool CDx11BlendState::CreateBlendState(
	const D3D11_RENDER_TARGET_BLEND_DESC * pTargetBlend, 
	UINT iCount, BOOL bAlphaEnable, BOOL bIndependentEnable)
{
	m_tDesc.AlphaToCoverageEnable = bAlphaEnable;
	m_tDesc.IndependentBlendEnable = bIndependentEnable;
	for (UINT i = 0; i < iCount; ++i)
	{
		m_tDesc.RenderTarget[i] = pTargetBlend[i];
	}

	if (FAILED(DX11_DEVICE->CreateBlendState(&m_tDesc,
		(ID3D11BlendState**)&m_pState)))
		return false;

	return true;
}

void CDx11BlendState::SetState()
{
	DX11_CONTEXT->OMGetBlendState((ID3D11BlendState**)&m_pPrevState, m_fOldBlendFactor, &m_iOldSampleMask);
	DX11_CONTEXT->OMSetBlendState((ID3D11BlendState*)m_pState, m_fBlendFactor, 0xffffffff);
}

void CDx11BlendState::ResetState()
{
	DX11_CONTEXT->OMSetBlendState((ID3D11BlendState*)m_pPrevState, m_fOldBlendFactor, m_iOldSampleMask);
	SAFE_RELEASE(m_pPrevState);
}
