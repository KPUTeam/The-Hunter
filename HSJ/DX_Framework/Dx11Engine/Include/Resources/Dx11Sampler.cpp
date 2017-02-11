#include "Dx11Sampler.h"
#include "../Core/Dx11Device.h"

DX11_USING

CDx11Sampler::CDx11Sampler()	:
	m_pSampler(NULL),
	m_tSampler({})
{
}

CDx11Sampler::~CDx11Sampler()
{
	SAFE_RELEASE(m_pSampler);
}

bool CDx11Sampler::CreateSampler(D3D11_FILTER eFilter, 
	D3D11_TEXTURE_ADDRESS_MODE u, D3D11_TEXTURE_ADDRESS_MODE v, 
	D3D11_TEXTURE_ADDRESS_MODE w, D3D11_COMPARISON_FUNC eFunc)
{
	m_tSampler.Filter = eFilter;
	m_tSampler.AddressU = u;
	m_tSampler.AddressV = v;
	m_tSampler.AddressW = w;
	m_tSampler.ComparisonFunc = eFunc;

	if (FAILED(DX11_DEVICE->CreateSamplerState(&m_tSampler, &m_pSampler)))
		return false;

	return true;
}

void CDx11Sampler::SetSampler(UINT iRegister)
{
	DX11_CONTEXT->PSSetSamplers(iRegister, 1, &m_pSampler);
}
