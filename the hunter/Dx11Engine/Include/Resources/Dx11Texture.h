#pragma once
#include "Dx11Resources.h"

DX11_BEGIN

class DX11_DLL CDx11Texture :
	public CDx11Resources
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
	bool LoadTexture(const string& strPath);
	void SetTexture(UINT iRegister = 0);
};

DX11_END
