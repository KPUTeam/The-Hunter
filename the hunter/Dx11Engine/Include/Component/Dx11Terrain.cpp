#include "Dx11Terrain.h"
#include "../Resources/Dx11Mesh.h"
#include "../Resources/Dx11ResourcesManager.h"
#include "Dx11Renderer.h"
#include "../GameObject/Dx11GameObject.h"
#include "Dx11Material.h"
#include "../Core/Dx11FilePathManager.h"
#include "Dx11Transform.h"

DX11_USING

CDx11Terrain::CDx11Terrain()
{
	SetTypeName<CDx11Terrain>();
}


CDx11Terrain::CDx11Terrain(const CDx11Terrain & terrain) :
	CDx11Component(terrain)
{
	*this = terrain;
}

CDx11Terrain::~CDx11Terrain()
{
}

bool CDx11Terrain::Init()
{
	return true;
}

int CDx11Terrain::Update(float fTime)
{
	return 0;
}

int CDx11Terrain::LateUpdate(float fTime)
{
	return 0;
}

void CDx11Terrain::Render(float fTime)
{
}

CDx11Terrain * CDx11Terrain::Clone()
{
	return new CDx11Terrain(*this);
}

void CDx11Terrain::CreataTerrain(const string & strKey, 
	UINT iNumX, UINT iNumY, const string & strDiffuseTexKey, 
	TCHAR * pDiffuseTexName, char * pHeightName, 
	const string & strHeightMapPathKey)
{

	vector<BUMPVERTEX>	vecVtx;
	vector<UINT>		vecIndex;

	BYTE*	pHeight = NULL;
	int		iPixelFmt = 1;

	// 파일로부터 높이맵 정보를 읽어온다.
	if (pHeightName)
	{
		FILE*	pFile = NULL;

		wstring strPath = DX11_GET_SINGLE(CDx11FilePathManager)->FindPath(strHeightMapPathKey);
		char	strHeightPath[MAX_PATH] = {};
		if (!strPath.empty())
		{
			WideCharToMultiByte(CP_ACP, 0, strPath.c_str(), -1,
				strHeightPath, strPath.length() * 2, 0, 0);
		}

		strcat_s(strHeightPath, pHeightName);
		fopen_s(&pFile, strHeightPath, "rb");

		BITMAPFILEHEADER	fh;
		BITMAPINFOHEADER	ih;

		fread(&fh, sizeof(fh), 1, pFile);
		fread(&ih, sizeof(ih), 1, pFile);

		m_iNumX = ih.biWidth;
		m_iNumY = ih.biHeight;

		pHeight = new BYTE[fh.bfSize - fh.bfOffBits];

		fread(pHeight, fh.bfSize - fh.bfOffBits, 1, pFile);

		if (ih.biBitCount == 32)
			iPixelFmt = 4;

		else if (ih.biBitCount == 24)
			iPixelFmt = 3;

		else if (ih.biBitCount == 8)
			iPixelFmt = 1;

		fclose(pFile);
	}

	// 높이맵이 아닌 경우
	else
	{
		m_iNumX = iNumX;
		m_iNumY = iNumY;

		pHeight = new BYTE[iNumX * iNumY];
		memset(pHeight, 0, sizeof(BYTE) * iNumX * iNumY);
	}

	vecVtx.reserve(iNumX * iNumY);
	vecIndex.reserve((iNumX - 1) * (iNumY - 1) * 2 * 3);
	m_vecPos.reserve(iNumX * iNumY);


	// 정점
	for (int i = 0; i < iNumY; ++i)
	{
		for (int j = 0; j < iNumX; ++j)
		{
			BUMPVERTEX tVtx = {};
			float	fY = 0.f;

			fY = pHeight[(i * iNumX + j) * iPixelFmt] / (255.f / 2.f);
			tVtx.vPos = Vec3(j, fY, iNumY - 1 - i);

			m_vecPos.push_back(tVtx.vPos);

			tVtx.vNormal = Vec3(0.f, 1.f, 0.f);
			tVtx.vUV = Vec2(j, i);

			vecVtx.push_back(tVtx);
		}
	}

	SAFE_DELETE_ARRAY(pHeight);


	// 인덱스
	// 사각형 갯수만큼 루프를 돈다.
	for (int i = 0; i < iNumY - 1; ++i)
	{
		for (int j = 0; j < iNumX - 1; ++j)
		{
			// 각 사각형의 첫번째 정점
			int iAddr = i * iNumX + j;

			// 우측 상단 삼각형 인덱스
			vecIndex.push_back(iAddr);
			vecIndex.push_back(iAddr + 1);
			vecIndex.push_back(iAddr + iNumX + 1);

			// 좌측 하단 삼각형 인덱스
			vecIndex.push_back(iAddr);
			vecIndex.push_back(iAddr + iNumX + 1);
			vecIndex.push_back(iAddr + iNumX);
		}
	}

	CDx11Mesh*	pMesh = DX11_GET_SINGLE(CDx11ResourcesManager)->CreateMesh(
		strKey, sizeof(BUMPVERTEX), iNumX * iNumY, &vecVtx[0],
		sizeof(UINT), vecIndex.size(), &vecIndex[0], DXGI_FORMAT_R32_UINT);
	SAFE_RELEASE(pMesh);


	CDx11Renderer*	pRenderer = m_pGameObject->AddComponent<CDx11Renderer>("TerrainRenderer");
	pRenderer->SetMesh(strKey);
	pRenderer->SetShader(DEFAULT_BUMP_SHADER);
	pRenderer->CreateMaterial(strDiffuseTexKey, pDiffuseTexName);
	SAFE_RELEASE(pRenderer);


}

float CDx11Terrain::GetHeight(const Vec3 & vPos)
{
	Vec3	vTPos = m_pTransform->GetWorldPos();

	vTPos += vPos;

	Vec3	vScale = m_pTransform->GetWorldScale();

	int	x = (int)(vTPos.x / (int)vScale.x);

	if (x < 0 || x >= m_iNumX)
		return 0.f;

	int z = m_iNumY - (int)(vTPos.z / (int)vScale.z) - 2;

	if (z < 0 || z >= m_iNumY)
		return 0.f;

	int	idx = z * m_iNumX + x;

	// 사각형을 구성하는 4개의 Y값을 얻어온다.
	float	fY[4] = {};
	fY[0] = m_vecPos[idx].y;
	fY[1] = m_vecPos[idx + 1].y;
	fY[2] = m_vecPos[idx + m_iNumX].y;
	fY[3] = m_vecPos[idx + m_iNumX + 1].y;

	vTPos /= vScale;
	vTPos.x -= (int)vTPos.x;
	vTPos.z -= (int)vTPos.z;

	// 오른쪽 위 삼각형
	if (vTPos.x >= vTPos.z)
		return fY[0] + (fY[1] - fY[0]) * vTPos.x + (fY[3] - fY[1]) * vTPos.z;

	return fY[0] + (fY[3] - fY[2]) * vTPos.x + (fY[2] - fY[0]) * vTPos.z;
}


