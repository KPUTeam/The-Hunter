#include "Dx11DepthStencilState.h"
#include "../Core/Dx11Device.h"

DX11_USING

CDx11DepthStencilState::CDx11DepthStencilState()
{
	
}

CDx11DepthStencilState::~CDx11DepthStencilState()
{
}

bool CDx11DepthStencilState::CreateDepthStencilState(bool bDepthEnable, 
	D3D11_DEPTH_WRITE_MASK eWriteMask, D3D11_COMPARISON_FUNC eDepthFunc, 
	bool bStencilEnable, UINT8 iReadMask, UINT8 iWriteMask)
{
	D3D11_DEPTH_STENCIL_DESC	tDesc = {};

	tDesc.DepthEnable = bDepthEnable;
	tDesc.DepthWriteMask = eWriteMask;
	tDesc.DepthFunc = eDepthFunc;
	tDesc.StencilEnable = bStencilEnable;
	tDesc.StencilReadMask = iReadMask;
	tDesc.StencilWriteMask = iWriteMask;

	if (FAILED(DX11_DEVICE->CreateDepthStencilState(&tDesc,
		(ID3D11DepthStencilState**)&m_pState)))
		return false;

	return true;
}

void CDx11DepthStencilState::SetState()
{
	DX11_CONTEXT->OMGetDepthStencilState((ID3D11DepthStencilState**)&m_pOldState, &m_iOldStencilRef);
	DX11_CONTEXT->OMSetDepthStencilState((ID3D11DepthStencilState*)m_pState, 0);
}

void CDx11DepthStencilState::ResetState()
{
	DX11_CONTEXT->OMSetDepthStencilState((ID3D11DepthStencilState*)m_pOldState, m_iOldStencilRef);
	SAFE_RELEASE(m_pOldState);
}
