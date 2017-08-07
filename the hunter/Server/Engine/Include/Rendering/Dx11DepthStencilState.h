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
	UINT		m_iOldStencilRef;

public:
	bool CreateDepthStencilState(bool bDepthEnable = true, 
		D3D11_DEPTH_WRITE_MASK eWriteMask = D3D11_DEPTH_WRITE_MASK_ALL,
		D3D11_COMPARISON_FUNC eDepthFunc = D3D11_COMPARISON_LESS,
		bool bStencilEnable = false, UINT8 iReadMask = 0xff, 
		UINT8 iWriteMask = 0xff);

public:
	virtual void SetState();
	virtual void ResetState();
};

DX11_END
