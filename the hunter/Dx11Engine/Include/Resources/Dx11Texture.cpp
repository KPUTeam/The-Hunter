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

bool CDx11Texture::LoadTexture(TCHAR * pFileName, const string & strPathKey)
{
//	이미지 픽셀 정보를 읽어올 클래스
	ScratchImage	tImage;
	
//	파일이름에서 확장자만 추출
	int iLength = lstrlen(pFileName);
	TCHAR	strExt[16] = {};

	for (int i = iLength - 1; i >= 0; --i)
	{
		if (pFileName[i] == '.')
		{
			lstrcpy(strExt, &pFileName[i + 1]);
			break;
		}
	}

//	풀 경로를 만들어준다.
	wstring strPath = DX11_GET_SINGLE(CDx11FilePathManager)->FindPath(strPathKey);
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
		if (FAILED(LoadFromWICFile(strPath.c_str(), WIC_FLAGS_NONE, NULL,
			tImage)))
			return false;
	}

	if (FAILED(CreateShaderResourceView(DX11_DEVICE, tImage.GetImages(),
		tImage.GetImageCount(), tImage.GetMetadata(), &m_pRSView)))
		return false;

	return true;
}

bool CDx11Texture::LoadTexture(const string & strPath)
{
	ScratchImage	tImage;

	int	iLength = strPath.length();

	char	strExt[16] = {};

	for (int i = iLength - 1; i >= 0; --i)
	{
		if (strPath[i] == '.')
		{
			strcpy_s(strExt, strPath.c_str() + (i + 1));
			break;
		}
	}

	TCHAR	strName[MAX_PATH] = {};

	MultiByteToWideChar(CP_ACP, 0, strPath.c_str(), -1, strName, strPath.length() * 2);

	if (strcmp(strExt, "dds") == 0 ||
		strcmp(strExt, "DDS") == 0)
	{
		if (FAILED(LoadFromDDSFile(strName, DDS_FLAGS_NONE,
			NULL, tImage)))
			return false;
	}

	else if (strcmp(strExt, "tga") == 0 ||
		strcmp(strExt, "TGA") == 0)
	{
		if (FAILED(LoadFromTGAFile(strName, NULL, tImage)))
			return false;
	}

	else
	{
		if (FAILED(LoadFromWICFile(strName, WIC_FLAGS_NONE,
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
