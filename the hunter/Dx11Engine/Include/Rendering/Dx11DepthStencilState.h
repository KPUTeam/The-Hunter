#pragma once
#include "Dx11RenderState.h"

DX11_BEGIN

class DX11_DLL CDx11DepthStencilState :
	public CDx11RenderState
{
private:
	friend class CDx11RenderManager;

private:
	CDx11DepthStencilState();
	~CDx11DepthStencilState();

private:
	D3D11_DEPTH_STENCIL_DESC		m_tDesc;
	UINT		m_iStencilRef;

public:
	bool CreateDepthStencilState(BOOL bDepthEnable,
		D3D11_DEPTH_WRITE_MASK eDepthMask,
		D3D11_COMPARISON_FUNC eDepthFunc);

public:
	virtual void SetState();
	virtual void ResetState();
};

DX11_END