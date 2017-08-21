#pragma once
#include "Dx11RenderState.h"

DX11_BEGIN

class DX11_DLL  CDx11RasterizerState:
	public CDx11RenderState
{
private:
	friend class CDx11RenderManager;

private:
	CDx11RasterizerState();
	~CDx11RasterizerState();

public:
	bool CreateRasterizerState(D3D11_FILL_MODE eFillMode,
		D3D11_CULL_MODE eCullMode, BOOL bClockWise, int iDepthBias = 0,
		float fDepthBiasClamp = 0.f, float fSlopeScaleDepthBias = 0.f,
		bool bDepthClipEnable = true, bool bScissorEnable = false,
		bool bMultiSampleEnable = false, bool bAntialiasedEnable = false);

public:
	virtual void SetState();
	virtual void ResetState();
};

DX11_END
