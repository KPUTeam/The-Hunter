#pragma once
#include "Dx11Resource.h"

DX11_BEGIN

class DX11_DLL CDx11Sampler :
	public CDx11Resource
{
private:
	friend class CDx11ResourcesManager;

private:
	CDx11Sampler();
	~CDx11Sampler();

private:
	D3D11_SAMPLER_DESC	m_tSampler;
	ID3D11SamplerState*	m_pSampler;

public:
	bool CreateSampler(D3D11_FILTER eFilter = D3D11_FILTER_MIN_MAG_MIP_LINEAR,
		D3D11_TEXTURE_ADDRESS_MODE u = D3D11_TEXTURE_ADDRESS_WRAP,
		D3D11_TEXTURE_ADDRESS_MODE v = D3D11_TEXTURE_ADDRESS_WRAP,
		D3D11_TEXTURE_ADDRESS_MODE w = D3D11_TEXTURE_ADDRESS_WRAP,
		D3D11_COMPARISON_FUNC eFunc = D3D11_COMPARISON_NEVER);
	void SetSampler(UINT iRegister = 0);
};

DX11_END