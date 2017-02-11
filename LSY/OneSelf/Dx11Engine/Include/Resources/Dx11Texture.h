#pragma once
#include "Dx11Resource.h"

DX11_BEGIN

class DX11_DLL CDx11Texture :
	public CDx11Resource
{
private:
	friend class CDx11ResourcesManager;

private:
	CDx11Texture();
	~CDx11Texture();

private:
	ID3D11ShaderResourceView*	m_pRSView;
	ID3D11Texture2D*			m_pTexture;

public:
	bool LoadTexture(TCHAR* pFileName, const string& strPathKey = TEXTURE_PATH);
	void SetTexture(UINT iRegister = 0);
};

DX11_END