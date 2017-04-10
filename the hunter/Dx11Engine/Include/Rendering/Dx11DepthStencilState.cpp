#include "Dx11DepthStencilState.h"
#include "../Core/Dx11Device.h"

DX11_USING

CDx11DepthStencilState::CDx11DepthStencilState()	:
	m_iStencilRef(0)
{
	m_eType = RST_DEPTHSTENCIL;
	memset(&m_tDesc, 0, sizeof(m_tDesc));

	m_tDesc.DepthEnable = TRUE;
	m_tDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	m_tDesc.DepthFunc = D3D11_COMPARISON_LESS;
	m_tDesc.StencilEnable = FALSE;
	m_tDesc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
	m_tDesc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;

	m_tDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	m_tDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	m_tDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	m_tDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	m_tDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	m_tDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	m_tDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	m_tDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

}


CDx11DepthStencilState::~CDx11DepthStencilState()
{
}

bool CDx11DepthStencilState::CreateDepthStencilState(BOOL bDepthEnable,
	D3D11_DEPTH_WRITE_MASK eDepthMask, 
	D3D11_COMPARISON_FUNC eDepthFunc)
{
	m_tDesc.DepthEnable = bDepthEnable;
	m_tDesc.DepthWriteMask = eDepthMask;
	m_tDesc.DepthFunc = eDepthFunc;

	if(FAILED(DX11_DEVICE->CreateDepthStencilState(&m_tDesc,
		(ID3D11DepthStencilState**)&m_pState)))
		return false;

	return true;
}

void CDx11DepthStencilState::SetState()
{
	DX11_CONTEXT->OMGetDepthStencilState((ID3D11DepthStencilState**)&m_pPrevState,
		&m_iStencilRef);
	DX11_CONTEXT->OMSetDepthStencilState((ID3D11DepthStencilState*)m_pState,
		0);
}

void CDx11DepthStencilState::ResetState()
{
	DX11_CONTEXT->OMSetDepthStencilState((ID3D11DepthStencilState*)m_pPrevState,
		0);

	SAFE_RELEASE(m_pPrevState);
}
