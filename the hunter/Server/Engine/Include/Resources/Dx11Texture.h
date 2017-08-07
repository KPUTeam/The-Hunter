#pragma once
#include "../Dx11Object.h"

DX11_BEGIN

class DX11_DLL CDx11Texture :
	public CDx11Object
{
private:
	friend class CDx11ResourcesManager;
	friend class CDx11Animation3D;

private:
	CDx11Texture();
	~CDx11Texture();

private:
	ScratchImage	m_Image;
	ID3D11ShaderResourceView*	m_pResourceView;
	D3D11_SAMPLER_DESC			m_tSampler;
	ID3D11SamplerState*			m_pSampler;
	wstring						m_strFileName;
	bool						m_bVTF;
	//ID3D11Texture2D*			m_pTexture;

public:
	wstring GetFileName();
	void SetVTFEnable();

public:
	bool LoadTexture(TCHAR* pFileName, const string& strPathKey = DX11_TEXTURE_PATH);
	bool LoadTexture(const string& strPath);
	bool CreateTexture(UINT iWidth, UINT iHeight, UINT iArraySize, 
		DXGI_FORMAT eFmt, D3D11_USAGE eUsage,
		D3D11_BIND_FLAG eBindFlag,
		int iCpuFlag, bool bVTF = false);
	bool CreateTexture(UINT iLength, UINT iArraySize, DXGI_FORMAT eFmt, 
		D3D11_USAGE eUsage,
		D3D11_BIND_FLAG eBindFlag,
		int iCpuFlag, bool bVTF = false);
	bool CreateSampler(D3D11_FILTER eFilter = D3D11_FILTER_MIN_MAG_MIP_LINEAR);
	void SetTexture(const UINT iTRegister = 0, const UINT iSRegister = 0);
	void SetData(void* pData, UINT iSize);
};

DX11_END
