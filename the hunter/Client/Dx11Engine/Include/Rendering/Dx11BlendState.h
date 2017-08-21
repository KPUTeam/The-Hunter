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
	D3D11_BLEND_DESC	m_tDesc;
	UINT				m_iTargetCount;
	float				m_fOldFactor[4];
	UINT				m_iOldSampleMask;
	float				m_fFactor[4];
	UINT				m_iSampleMask;

public:
	bool CreateBlendState(BOOL bEnable, D3D11_BLEND eSrcBlend, D3D11_BLEND eDestBlend,
		D3D11_BLEND_OP eBlendOp, D3D11_BLEND eAlphaSrcBlend, D3D11_BLEND eAlphaDestBlend,
		D3D11_BLEND_OP eAlphaBlendOp, BOOL bAlphaToCoverage = FALSE, 
		BOOL bIndependentBlend = FALSE,
		UINT8 iWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL);
	bool AddTargetInfo(BOOL bEnable, D3D11_BLEND eSrcBlend, D3D11_BLEND eDestBlend,
		D3D11_BLEND_OP eBlendOp, D3D11_BLEND eAlphaSrcBlend, D3D11_BLEND eAlphaDestBlend,
		D3D11_BLEND_OP eAlphaBlendOp,
		UINT8 iWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL);
	bool CreateState();

public:
	virtual void SetState();
	virtual void ResetState();
};

DX11_END
