#include "Dx11BlendState.h"
#include "../Core/Dx11Device.h"

DX11_USING

CDx11BlendState::CDx11BlendState()	:
	m_iTargetCount(0)
{
	memset(m_fFactor, 0, sizeof(float) * 4);
	memset(m_fOldFactor, 0, sizeof(float) * 4);

	m_iOldSampleMask = 0xffffffff;
	m_iSampleMask = 0xffffffff;
}

CDx11BlendState::~CDx11BlendState()
{
}

bool CDx11BlendState::CreateBlendState(BOOL bEnable, D3D11_BLEND eSrcBlend,
	D3D11_BLEND eDestBlend, D3D11_BLEND_OP eBlendOp, 
	D3D11_BLEND eAlphaSrcBlend, D3D11_BLEND eAlphaDestBlend, 
	D3D11_BLEND_OP eAlphaBlendOp, BOOL bAlphaToCoverage, BOOL bIndependentBlend,
	UINT8 iWriteMask)
{
	m_tDesc.AlphaToCoverageEnable = bAlphaToCoverage;
	m_tDesc.IndependentBlendEnable = bIndependentBlend;
	AddTargetInfo(bEnable, eSrcBlend, eDestBlend, eBlendOp, eAlphaSrcBlend,
		eAlphaDestBlend, eAlphaBlendOp, iWriteMask);

	return true;
}

bool CDx11BlendState::AddTargetInfo(BOOL bEnable, D3D11_BLEND eSrcBlend, 
	D3D11_BLEND eDestBlend, D3D11_BLEND_OP eBlendOp, D3D11_BLEND eAlphaSrcBlend,
	D3D11_BLEND eAlphaDestBlend, D3D11_BLEND_OP eAlphaBlendOp, UINT8 iWriteMask)
{
	if (m_iTargetCount == DX11_RENDER_TARGET_MAX)
		return false;

	m_tDesc.RenderTarget[m_iTargetCount].BlendEnable = bEnable;
	m_tDesc.RenderTarget[m_iTargetCount].SrcBlend = eSrcBlend;
	m_tDesc.RenderTarget[m_iTargetCount].DestBlend = eDestBlend;
	m_tDesc.RenderTarget[m_iTargetCount].BlendOp = eBlendOp;
	m_tDesc.RenderTarget[m_iTargetCount].SrcBlendAlpha = eAlphaSrcBlend;
	m_tDesc.RenderTarget[m_iTargetCount].DestBlendAlpha = eAlphaDestBlend;
	m_tDesc.RenderTarget[m_iTargetCount].BlendOpAlpha = eAlphaBlendOp;
	m_tDesc.RenderTarget[m_iTargetCount].RenderTargetWriteMask = iWriteMask;

	++m_iTargetCount;

	return true;
}

bool CDx11BlendState::CreateState()
{
	if (FAILED(DX11_DEVICE->CreateBlendState(&m_tDesc, 
		(ID3D11BlendState**)&m_pState)))
		return false;

	return true;
}

void CDx11BlendState::SetState()
{
	DX11_CONTEXT->OMGetBlendState((ID3D11BlendState**)&m_pOldState,
		m_fOldFactor, &m_iOldSampleMask);
	DX11_CONTEXT->OMSetBlendState((ID3D11BlendState*)m_pState,
		m_fFactor, m_iSampleMask);
}

void CDx11BlendState::ResetState()
{
	DX11_CONTEXT->OMSetBlendState((ID3D11BlendState*)m_pOldState,
		m_fOldFactor, m_iOldSampleMask);
	SAFE_RELEASE(m_pOldState);
}
