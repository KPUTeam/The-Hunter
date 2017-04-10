#pragma once
#include "Dx11RenderState.h"

DX11_BEGIN

class DX11_DLL CDx11RasterizerState :
	public CDx11RenderState
{
private:
	friend class CDx11RenderManager;

private:
	CDx11RasterizerState();
	~CDx11RasterizerState();

private:
	D3D11_RASTERIZER_DESC	m_tDesc;

public:
	bool CreateRasterizerState(D3D11_FILL_MODE eFillMode,
		D3D11_CULL_MODE eCullMode, BOOL bFrontCounterClockWise = FALSE,
		BOOL bDepthClipEnable = TRUE);

public:
	virtual void SetState();
	virtual void ResetState();
};

DX11_END