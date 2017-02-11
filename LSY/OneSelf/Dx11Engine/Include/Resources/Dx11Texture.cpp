#include "Dx11Texture.h"
#include "DirectXTex.h"
#include "../Core/Dx11FilePathManager.h"
#include "../Core/Dx11Device.h"

// 텍스쳐 관련 블로그
// http://m.blog.naver.com/sorkelf/220846504837

DX11_USING

CDx11Texture::CDx11Texture()	:
	m_pRSView(NULL),
	m_pTexture(NULL)
{
}

CDx11Texture::~CDx11Texture()
{
	SAFE_RELEASE(m_pRSView);
	SAFE_RELEASE(m_pTexture);
}

bool CDx11Texture::LoadTexture(TCHAR * pFileName, 
	const string & strPathKey)
{
	ScratchImage	tImage;

	int	iLength = lstrlen(pFileName);

	TCHAR	strExt[16] = {};

	for (int i = iLength - 1; i >= 0; --i)
	{
		if (pFileName[i] == '.')
		{
			lstrcpy(strExt, &pFileName[i + 1]);
			break;
		}
	}

	// 풀경로를 만들어준다.
	wstring	strPath = DX11_GET_SINGLE(CDx11FilePathManager)->FindPath(strPathKey);
	strPath += pFileName;

	if (lstrcmp(strExt, L"dds") == 0 ||
		lstrcmp(strExt, L"DDS") == 0)
	{
		if (FAILED(LoadFromDDSFile(strPath.c_str(), DDS_FLAGS_NONE,
			NULL, tImage)))
			return false;
	}

	else if (lstrcmp(strExt, L"tga") == 0 ||
		lstrcmp(strExt, L"TGA") == 0)
	{
		if (FAILED(LoadFromTGAFile(strPath.c_str(), NULL, tImage)))
			return false;
	}

	else
	{
		if (FAILED(LoadFromWICFile(strPath.c_str(), WIC_FLAGS_NONE,
			NULL, tImage)))
			return false;
	}

	if (FAILED(CreateShaderResourceView(DX11_DEVICE, tImage.GetImages(),
		tImage.GetImageCount(), tImage.GetMetadata(), &m_pRSView)))
		return false;

	return true;
}

void CDx11Texture::SetTexture(UINT iRegister)
{
	DX11_CONTEXT->PSSetShaderResources(iRegister, 1, &m_pRSView);
}
