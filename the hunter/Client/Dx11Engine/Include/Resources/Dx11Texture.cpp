#include "Dx11Texture.h"
#include "../Core/Dx11PathManager.h"
#include "../Core/Dx11Device.h"

DX11_USING

CDx11Texture::CDx11Texture()	:
	m_pSampler(NULL),
	m_pResourceView(NULL),
	m_tSampler({}),
	m_bVTF(false)
{
}

CDx11Texture::~CDx11Texture()
{
	SAFE_RELEASE(m_pSampler);
	SAFE_RELEASE(m_pResourceView);
}

wstring CDx11Texture::GetFileName()
{
	return m_strFileName;
}

void CDx11Texture::SetVTFEnable()
{
	m_bVTF = true;
}

bool CDx11Texture::LoadTexture(TCHAR * pFileName,
	const string & strPathKey)
{
	TCHAR	strExt[_MAX_EXT] = {};

	_wsplitpath_s(pFileName, 0, 0, 0, 0, 0, 0, strExt, _MAX_EXT);

	wstring	strPath = DX11_GET_SINGLE(CDx11PathManager)->FindPath(strPathKey);
	strPath += pFileName;

	m_strFileName = pFileName;

	if (_wcsicmp(strExt, L".dds") == 0)
	{
		if (FAILED(LoadFromDDSFile(strPath.c_str(), DDS_FLAGS_NONE, NULL, m_Image)))
			return false;
	}
	
	else if (_wcsicmp(strExt, L".tga") == 0)
	{
		if (FAILED(LoadFromTGAFile(strPath.c_str(), NULL, m_Image)))
			return false;
	}

	else
	{
		if (FAILED(LoadFromWICFile(strPath.c_str(), WIC_FLAGS_NONE, NULL, m_Image)))
			return false;
	}
	
	if (FAILED(CreateShaderResourceView(DX11_DEVICE, m_Image.GetImages(),
		m_Image.GetImageCount(), m_Image.GetMetadata(), 
		&m_pResourceView)))
		return false;

	m_tSampler.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	m_tSampler.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	m_tSampler.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	m_tSampler.MinLOD = 1.f;
	m_tSampler.MaxLOD = 1.f;
	m_tSampler.MipLODBias = 1.f;
	m_tSampler.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

	if (FAILED(DX11_DEVICE->CreateSamplerState(&m_tSampler,
		&m_pSampler)))
		return false;

	return true;
}

bool CDx11Texture::LoadTexture(const string & strPath)
{
	char	strExt[_MAX_EXT] = {};

	_splitpath_s(strPath.c_str(), 0, 0, 0, 0, 0, 0, strExt, _MAX_EXT);

	TCHAR	strFileName[MAX_PATH] = {};
	MultiByteToWideChar(CP_ACP, 0, strPath.c_str(), -1, strFileName,
		strPath.length() * 2);

	m_strFileName = strFileName;

	if (strcmp(strExt, ".dds") == 0)
	{
		if (FAILED(LoadFromDDSFile(strFileName, DDS_FLAGS_NONE, NULL, m_Image)))
			return false;
	}

	else if (strcmp(strExt, ".tga") == 0)
	{
		if (FAILED(LoadFromTGAFile(strFileName, NULL, m_Image)))
			return false;
	}

	else
	{
		if (FAILED(LoadFromWICFile(strFileName, WIC_FLAGS_NONE, NULL, m_Image)))
			return false;
	}

	if (FAILED(CreateShaderResourceView(DX11_DEVICE, m_Image.GetImages(),
		m_Image.GetImageCount(), m_Image.GetMetadata(),
		&m_pResourceView)))
		return false;

	m_tSampler.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	m_tSampler.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	m_tSampler.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	m_tSampler.MinLOD = 1.f;
	m_tSampler.MaxLOD = 1.f;
	m_tSampler.MipLODBias = 1.f;
	m_tSampler.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

	if (FAILED(DX11_DEVICE->CreateSamplerState(&m_tSampler,
		&m_pSampler)))
		return false;

	return true;
}

bool CDx11Texture::CreateTexture(UINT iWidth, UINT iHeight, 
	UINT iArraySize, DXGI_FORMAT eFmt, D3D11_USAGE eUsage,
	D3D11_BIND_FLAG eBindFlag, int iCpuFlag, bool bVTF)
{
	m_Image.Initialize2D(eFmt, iWidth, iHeight, iArraySize, 1);

	if (FAILED(CreateShaderResourceViewEx(DX11_DEVICE, m_Image.GetImages(),
		m_Image.GetImageCount(), m_Image.GetMetadata(),
		eUsage, eBindFlag, iCpuFlag, 0, false, &m_pResourceView)))
		return false;

	m_bVTF = bVTF;

	CreateSampler(D3D11_FILTER_MIN_MAG_MIP_POINT);

	return true;
}

bool CDx11Texture::CreateTexture(UINT iLength, UINT iArraySize, 
	DXGI_FORMAT eFmt, D3D11_USAGE eUsage,
	D3D11_BIND_FLAG eBindFlag, int iCpuFlag, bool bVTF)
{
	return true;
}

bool CDx11Texture::CreateSampler(D3D11_FILTER eFilter)
{
	m_tSampler.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	m_tSampler.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	m_tSampler.Filter = eFilter;
	m_tSampler.MinLOD = 1.f;
	m_tSampler.MaxLOD = 1.f;
	m_tSampler.MipLODBias = 1.f;
	m_tSampler.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

	if (FAILED(DX11_DEVICE->CreateSamplerState(&m_tSampler,
		&m_pSampler)))
		return false;

	return true;
}

void CDx11Texture::SetTexture(const UINT iTRegister, const UINT iSRegister)
{
	if (m_bVTF)
		DX11_CONTEXT->VSSetShaderResources(iTRegister, 1, &m_pResourceView);

	else
		DX11_CONTEXT->PSSetShaderResources(iTRegister, 1, &m_pResourceView);

	if (iSRegister != UINT_MAX)
	{
		if (m_bVTF)
			DX11_CONTEXT->VSSetSamplers(iSRegister, 1, &m_pSampler);

		else
			DX11_CONTEXT->PSSetSamplers(iSRegister, 1, &m_pSampler);
	}
}

void CDx11Texture::SetData(void * pData, UINT iSize)
{
	ID3D11Texture2D*	pTexture = NULL;
	m_pResourceView->GetResource((ID3D11Resource**)&pTexture);

	D3D11_MAPPED_SUBRESOURCE	tMap;
	DX11_CONTEXT->Map(pTexture, 0, D3D11_MAP_WRITE_DISCARD, 0, &tMap);
	
	memcpy(tMap.pData, pData, iSize);

	DX11_CONTEXT->Unmap(pTexture, 0);

	SAFE_RELEASE(pTexture);
}
