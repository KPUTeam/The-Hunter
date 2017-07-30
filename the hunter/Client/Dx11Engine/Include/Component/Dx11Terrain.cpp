#include "Dx11Terrain.h"
#include "Dx11Renderer.h"
#include "../Resources/Dx11ResourcesManager.h"
#include "../Resources/Dx11Mesh.h"
#include "../GameObject/Dx11GameObject.h"
#include "Dx11Material.h"
#include "../Core/Dx11PathManager.h"
#include "../Core/Dx11File.h"
#include "Dx11ColliderTerrain.h"
#include "Dx11Transform.h"

DX11_USING

CDx11Terrain::CDx11Terrain()
{
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

void CDx11Terrain::CreateTerrian(
	UINT iW,
	UINT iH,
	float fWeight,
	char* pHeightMap,
	TCHAR* pDiffuseTex,
	TCHAR* pSpecTex,
	TCHAR* pNormalTex,
	const string& strPathKey)
{
	m_iWidth = iW;
	m_iHeight = iH;

	float*	fY = new float[iW * iH];

	if (pHeightMap)
	{
		char	strExt[_MAX_EXT] = {};
		for (int i = strlen(pHeightMap) - 1; i >= 0; --i)
		{
			if (pHeightMap[i] == '.')
			{
				memcpy(strExt, &pHeightMap[i + 1], strlen(pHeightMap) - (i + 1));
				break;
			}
		}

		if (strcmp(strExt, "bmp") == 0 || strcmp(strExt, "BMP") == 0)
		{
			string	strName = DX11_GET_SINGLE(CDx11PathManager)->FindPathToMultiByte(strPathKey);
			strName += pHeightMap;

			CDx11File	file;

			if (file.Open(strName.c_str(), "rb"))
			{
				BITMAPFILEHEADER	fh;
				BITMAPINFOHEADER	ih;

				file.Read(&fh, sizeof(fh));
				file.Read(&ih, sizeof(ih));

				int	iPixelSize = file.GetFileSize() - (sizeof(fh) + sizeof(ih));
				BYTE*	pPixel = new BYTE[iPixelSize];
				file.Read(pPixel, iPixelSize);

				if (ih.biBitCount == 32)
				{
					for (int i = 0; i < iW * iH; ++i)
					{
						fY[i] = pPixel[i * 4] / fWeight;
					}
				}

				else if (ih.biBitCount == 24)
				{
					for (int i = 0; i < iW * iH; ++i)
					{
						fY[i] = pPixel[i * 3] / fWeight;
					}
				}

				else if (ih.biBitCount == 8)
				{
					for (int i = 0; i < iW * iH; ++i)
					{
						fY[i] = pPixel[i] / fWeight;
					}
				}


				SAFE_DELETE_ARRAY(pPixel);
			}
		}

		else if (strcmp(strExt, "trn") == 0 || strcmp(strExt, "TRN") == 0)
		{
		}
	}

	else
	{
		for (UINT i = 0; i < iH; ++i)
		{
			for (UINT j = 0; j < iW; ++j)
			{
				fY[i * iW + j] = rand() % 3001 / 1000.f;
			}
		}
	}

	// 정점정보
	vector<TEXVERTEX> vecVtx;
	m_vecPos.reserve(iW * iH);

	for (UINT i = 0; i < iH; ++i)
	{
		for (UINT j = 0; j < iW; ++j)
		{
			TEXVERTEX	tVtx = {};

			tVtx.vPos = Vec3((float)j, fY[i * iW + j], (float)iH - 1 - (float)i);
			tVtx.vNormal = Vec3(0.f, 1.f, 0.f);
			tVtx.vTex = Vec2((float)j, (float)i);

			m_vecPos.push_back(tVtx.vPos);

			vecVtx.push_back(tVtx);
		}
	}

	SAFE_DELETE_ARRAY(fY);

	//인덱스 정보
	vector<UINT> vecIdx;
	for (UINT i = 0; i < iH - 1; ++i)
	{
		for (UINT j = 0; j < iW - 1; ++j)
		{
			// 구성하고자 하는 사각형의Left, Top 인덱스를 구한다
			int iAddr = i * iW + j;

			// 오른쪽 상단 삼각형
			vecIdx.push_back(iAddr);
			vecIdx.push_back(iAddr + 1);
			vecIdx.push_back(iAddr + iW + 1);

			// 오른쪽 하단 삼각형
			vecIdx.push_back(iAddr);
			vecIdx.push_back(iAddr + iW + 1);
			vecIdx.push_back(iAddr + iW);
		}
	}

	CDx11Mesh* pMesh = DX11_GET_SINGLE(CDx11ResourcesManager)->CreateMesh(
		"Terrain1", &vecVtx[0], iW * iH, sizeof(TEXVERTEX),
		&vecIdx[0], vecIdx.size(), sizeof(UINT), DXGI_FORMAT_R32_UINT);

	SAFE_RELEASE(pMesh);

	CDx11Renderer* pRenderer = m_pGameObject->AddComponent<CDx11Renderer>("TerrainRenderer");

	pRenderer->SetMesh("Terrain1");
	pRenderer->SetShader("TexShader");
	pRenderer->CreateMaterial("TerraiDiffuse", pDiffuseTex, strPathKey);

	CDx11Material*	pMaterial = pRenderer->GetMaterial();

	if (!pSpecTex)
		pSpecTex = pDiffuseTex;

	pMaterial->SetSpecularTexture("TerrainSpec", pSpecTex, strPathKey);

	if (pNormalTex)
		pMaterial->SetNormalMapTexture("TerrainNormal", pNormalTex, strPathKey);

	// pMaterial->SetDiffuseTexture("TerraiDiffuse", pDiffuseTex, strPathKey);

	SAFE_RELEASE(pMaterial);

	SAFE_RELEASE(pRenderer);
}

TRIANGLE2RAY CDx11Terrain::GetHeight(const CDx11GameObject* obj)
{
	TRIANGLE2RAY Result;

	CDx11Transform*	pObjTr = obj->GetTransform();
	Vec3	vObjPos = pObjTr->GetWorldPos();
	SAFE_RELEASE(pObjTr);

	//
	Vec3	vTerrainScale = m_pTransform->GetWorldScale();
	Vec3	vTerrainPos = m_pTransform->GetWorldPos();

	// 인덱스를 구한다.
	int	iX = (int)(vObjPos.x / vTerrainScale.x - vTerrainPos.x);
	int	iY = (m_iHeight - 1) - ((int)(vObjPos.z / vTerrainScale.z - vTerrainPos.z) + 1);

	if (iX < 0 || iX >= m_iWidth || iY < 0 || iY >= m_iHeight)
		return Result;

	int		idx = iY * m_iWidth + iX;

	// 사각형을 구성하는 4개의 Y값을 얻어온다.
	Vec3 P[4] = {};

	P[0] = m_vecPos[idx] * vTerrainScale + vTerrainPos;
	P[1] = m_vecPos[idx + 1] * vTerrainScale + vTerrainPos;
	P[2] = m_vecPos[idx + m_iWidth] * vTerrainScale + vTerrainPos;
	P[3] = m_vecPos[idx + m_iWidth + 1] * vTerrainScale + vTerrainPos;

	// 삼각형의 법선벡터를 구한다.
	Vec3 n = {};

	float	fX = vObjPos.x / vTerrainScale.x - m_vecPos[idx].x;
	float	fZ = m_vecPos[idx].z - vObjPos.z / vTerrainScale.z;

	float m = P[3].z - P[0].z / P[3].x - P[0].x;
	float objm = vObjPos.z - P[0].z / vObjPos.x - P[0].x;

	// 우상단
	if (m < objm)
		n = Vec3(P[3] - P[1]).Cross(Vec3(P[0] - P[1]));

	// 좌하단
	else
		n = Vec3(P[3] - P[2]).Cross(Vec3(P[0] - P[2]));

	n = n.Normalize();
	Result.Normal = n;

	// x, z 위치에 높이는  -1 을 지나는 직선
	Vec3 Pl = { vObjPos.x, -100, vObjPos.z };

	// 직선의 길이
	Vec3 vDir = { 0, 1000, 0 };

	float t = (n.Dot(P[0]) - n.Dot(Pl)) / n.Dot(vDir);

	Result.Pos = Pl + (vDir*t);

	return Result;

	//char str0[100] = {};
	//char str1[100] = {};
	//char str2[100] = {};
	//char str3[100] = {};

	//sprintf(str0, "0: %.3f, %.3f, %.3f\n", P[0].x, P[0].y, P[0].z);
	//sprintf(str1, "1: %.3f, %.3f, %.3f\n", P[1].x, P[1].y, P[1].z);
	//sprintf(str2, "2: %.3f, %.3f, %.3f\n", P[2].x, P[2].y, P[2].z);
	//sprintf(str3, "3: %.3f, %.3f, %.3f\n", P[3].x, P[3].y, P[3].z);

	//_cprintf(str0);
	//_cprintf(str1);
	//_cprintf(str2);
	//_cprintf(str3);
}

Vec3 CDx11Terrain::GetTex(const Vec3 vPos, int iIndexs)
{
	Vec3	vObjPos = vPos;
	Vec3	vTerrainScale = m_pTransform->GetWorldScale();
	Vec3	vTerrainPos = m_pTransform->GetWorldPos();

	// 인덱스를 구한다.
	int	iX = (int)(vObjPos.x / vTerrainScale.x - vTerrainPos.x);
	int	iY = (m_iHeight - 1) - ((int)(vObjPos.z / vTerrainScale.z - vTerrainPos.z) + 1);

	int		idx = iY * m_iWidth + iX;

	// 주변 좌표를 구한다.
	Vec3 P[9] = {};

	P[0] = vTerrainPos + vTerrainScale*m_vecPos[idx - m_iWidth - 1];
	P[1] = vTerrainPos + vTerrainScale*m_vecPos[idx - m_iWidth];
	P[2] = vTerrainPos + vTerrainScale*m_vecPos[idx - m_iWidth + 1];
	P[3] = vTerrainPos + vTerrainScale*m_vecPos[idx - 1];
	P[4] = vTerrainPos + vTerrainScale*m_vecPos[idx];
	P[5] = vTerrainPos + vTerrainScale*m_vecPos[idx + 1];
	P[6] = vTerrainPos + vTerrainScale*m_vecPos[idx + m_iWidth - 1];
	P[7] = vTerrainPos + vTerrainScale*m_vecPos[idx + m_iWidth];
	P[8] = vTerrainPos + vTerrainScale*m_vecPos[idx + m_iWidth + 1];

	return P[iIndexs];
}
