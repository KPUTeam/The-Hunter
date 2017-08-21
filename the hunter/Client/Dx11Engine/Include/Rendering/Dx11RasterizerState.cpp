#include "Dx11RasterizerState.h"
#include "../Core/Dx11Device.h"

DX11_USING

CDx11RasterizerState::CDx11RasterizerState()
{
}

CDx11RasterizerState::~CDx11RasterizerState()
{
}

bool CDx11RasterizerState::CreateRasterizerState(
	D3D11_FILL_MODE eFillMode, D3D11_CULL_MODE eCullMode, BOOL bClockWise,
	int iDepthBias, float fDepthBiasClamp, float fSlopeScaleDepthBias,
	bool bDepthClipEnable, bool bScissorEnable, bool bMultiSampleEnable,
	bool bAntialiasedEnable)
{
	D3D11_RASTERIZER_DESC	tInfo = {};

	tInfo.AntialiasedLineEnable = bAntialiasedEnable;
	tInfo.CullMode = eCullMode;
	tInfo.DepthBias = iDepthBias;
	tInfo.DepthBiasClamp = fDepthBiasClamp;
	tInfo.DepthClipEnable = bDepthClipEnable;
	tInfo.FillMode = eFillMode;
	tInfo.FrontCounterClockwise = bClockWise;
	tInfo.MultisampleEnable = bMultiSampleEnable;
	tInfo.ScissorEnable = bScissorEnable;
	tInfo.SlopeScaledDepthBias = fSlopeScaleDepthBias;

	if (FAILED(DX11_DEVICE->CreateRasterizerState(&tInfo, 
		(ID3D11RasterizerState**)&m_pState)))
		return false;

	return true;
}

void CDx11RasterizerState::SetState()
{
	DX11_CONTEXT->RSGetState((ID3D11RasterizerState**)&m_pOldState);
	DX11_CONTEXT->RSSetState((ID3D11RasterizerState*)m_pState);
}

void CDx11RasterizerState::ResetState()
{
	DX11_CONTEXT->RSSetState((ID3D11RasterizerState*)m_pOldState);
	SAFE_RELEASE(m_pOldState);
}
