#include "Dx11RasterizerState.h"
#include "../Core/Dx11Device.h"

DX11_USING 

CDx11RasterizerState::CDx11RasterizerState()
{
	m_eType = RST_RASTERIZER;

	m_tDesc.FillMode = D3D11_FILL_SOLID;
	m_tDesc.CullMode = D3D11_CULL_BACK;
	m_tDesc.FrontCounterClockwise = false;
	m_tDesc.DepthBias = 0;
	m_tDesc.DepthBiasClamp = 0.f;
	m_tDesc.SlopeScaledDepthBias = 0.f;
	m_tDesc.DepthClipEnable = true;
	m_tDesc.ScissorEnable = false;
	m_tDesc.MultisampleEnable = false;
	m_tDesc.AntialiasedLineEnable = false;
}

CDx11RasterizerState::~CDx11RasterizerState()
{
}

bool CDx11RasterizerState::CreateRasterizerState(
	D3D11_FILL_MODE eFillMode, D3D11_CULL_MODE eCullMode, 
	BOOL bFrontCounterClockWise, BOOL bDepthClipEnable)
{
	m_tDesc.FillMode = eFillMode;
	m_tDesc.CullMode = eCullMode;
	m_tDesc.FrontCounterClockwise = bFrontCounterClockWise;
	m_tDesc.DepthClipEnable = bDepthClipEnable;

	if (FAILED(DX11_DEVICE->CreateRasterizerState(&m_tDesc,
		(ID3D11RasterizerState**)&m_pState)))
		return false;

	return true;
}

void CDx11RasterizerState::SetState()
{
	DX11_CONTEXT->RSGetState((ID3D11RasterizerState**)&m_pPrevState);
	DX11_CONTEXT->RSSetState((ID3D11RasterizerState*)m_pState);
}

void CDx11RasterizerState::ResetState()
{
	DX11_CONTEXT->RSSetState((ID3D11RasterizerState*)m_pPrevState);
	SAFE_RELEASE(m_pPrevState);
}
