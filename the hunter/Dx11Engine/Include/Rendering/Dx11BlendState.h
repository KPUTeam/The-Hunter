#pragma once
#include "Dx11RenderState.h"

DX11_BEGIN

class DX11_DLL CDx11BlendState :
	public CDx11RenderState
{
private:
	friend class CDx11RenderManager;

private:
	CDx11BlendState();
	~CDx11BlendState();

private:
	D3D11_BLEND_DESC		m_tDesc;
	float					m_fBlendFactor[4];
	float					m_fOldBlendFactor[4];
	UINT					m_iOldSampleMask;

public:
	bool CreateBlendState(D3D11_BLEND eSrcBlend = D3D11_BLEND_SRC_ALPHA,
		D3D11_BLEND eDestBlend = D3D11_BLEND_INV_SRC_ALPHA,
		D3D11_BLEND_OP eBlendOp = D3D11_BLEND_OP_ADD,
		D3D11_BLEND eSrcAlphaBlend = D3D11_BLEND_ONE,
		D3D11_BLEND eDestAlphaBlend = D3D11_BLEND_ZERO,
		D3D11_BLEND_OP eAlphaBlendOp = D3D11_BLEND_OP_ADD,
		UINT8 iWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL,
		BOOL bAlphaEnable = FALSE, BOOL bIndependentEnable = FALSE);
	bool CreateBlendState(const D3D11_RENDER_TARGET_BLEND_DESC* pTargetBlend,
		UINT iCount, BOOL bAlphaEnable = FALSE, BOOL bIndependentEnable = TRUE);

public:
	virtual void SetState();
	virtual void ResetState();
};

DX11_END