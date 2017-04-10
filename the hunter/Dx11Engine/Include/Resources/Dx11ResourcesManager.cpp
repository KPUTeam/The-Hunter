#include "Dx11ResourcesManager.h"
#include "Dx11Mesh.h"
#include "Dx11Texture.h"
#include "Dx11Sampler.h"

DX11_USING

DX11_DEFINITION_SINGLE(CDx11ResourcesManager)

CDx11ResourcesManager::CDx11ResourcesManager()
{
	SetTypeName<CDx11ResourcesManager>();
}

CDx11ResourcesManager::~CDx11ResourcesManager()
{
	Safe_Release_Map(m_mapMesh);
	Safe_Release_Map(m_mapTexture);
	Safe_Release_Map(m_mapSampler);
}

bool CDx11ResourcesManager::Init()
{
	//삼각형 매쉬
	COLORVERTEX	tColorVtx[3] =
	{
		{ Vec3(0.f, 0.5f, 0.f), Vec3(0.f, 1.f, 0.f), Vec4(1.f, 0.f, 0.f, 1.f) },
		{ Vec3(0.5f, -0.5f, 0.f), Vec3(0.f, 1.f, 0.f), Vec4(0.f, 1.f, 0.f, 1.f) },
		{ Vec3(-0.5f, -0.5f, 0.f), Vec3(0.f, 1.f, 0.f), Vec4(0.f, 0.f, 1.f, 1.f) }
	};
	UINT	iIndex[3] = { 0, 1, 2 };

	CDx11Mesh*	pTriangleMesh = CreateMesh(TRIANGLE_MESH, sizeof(COLORVERTEX),
		3, tColorVtx, sizeof(UINT), 3, iIndex,
		DXGI_FORMAT_R32_UINT);
	SAFE_RELEASE(pTriangleMesh);


	//피라미드 매쉬
	COLORVERTEX	tColorVtx2[5] =
	{
		{ Vec3(0.f, 0.75f, 0.f), Vec3(0.f, 1.f, 0.f), Vec4(1.f, 0.f, 0.f, 1.f) },		//윗 꼭지점
		{ Vec3(0.5f, -0.5f, 0.5f), Vec3(0.2f, -0.1f, 0.2f), Vec4(0.f, 1.f, 0.f, 1.f) },
		{ Vec3(0.5f, -0.5f, -0.5f), Vec3(0.2f, -0.1f, -0.2f), Vec4(0.f, 0.f, 1.f, 1.f) },
		{ Vec3(-0.5f, -0.5f, -0.5f), Vec3(-0.2f, -0.1f, -0.2f), Vec4(1.f, 1.f, 0.f, 1.f) },
		{ Vec3(-0.5f, -0.5f, 0.5f), Vec3(-0.2f, -0.1f, 0.2f), Vec4(0.f, 1.f, 1.f, 1.f) }
	};
	UINT	iIndex2[18] = {
		0, 1, 2,
		0, 2, 3,
		0, 3, 4,
		0, 4, 1,
		1, 4, 3,
		3, 2, 1
	};

	for (int i = 0; i < 5; ++i)
	{
		tColorVtx2[i].vNormal = tColorVtx2[i].vNormal.Normalize();
	}

	CDx11Mesh*	pPyramidMesh = CreateMesh(PYRAMID_MESH, sizeof(COLORVERTEX),
		5, tColorVtx2, sizeof(UINT), 18, iIndex2,
		DXGI_FORMAT_R32_UINT);
	SAFE_RELEASE(pPyramidMesh);


	//텍스쳐 피라미드 매쉬
	TEXVERTEX	tTexVtx3[] =
	{
		{ Vec3(0.f, 0.75f, 0.f), Vec3(0.f, 1.f, 0.f), Vec2(0.5f, 1.f) },
		{ Vec3(0.5f, -0.5f, 0.5f), Vec3(0.2f, -0.1f, 0.2f), Vec2(1.f, 1.f) },
		{ Vec3(0.5f, -0.5f, -0.5f), Vec3(0.2f, -0.1f, -0.2f), Vec2(0.f, 0.f) },

		{ Vec3(0.f, 0.75f, 0.f), Vec3(0.f, 1.f, 0.f), Vec2(0.5f, 1.f) },
		{ Vec3(0.5f, -0.5f, -0.5f), Vec3(0.2f, -0.1f, -0.2f), Vec2(1.f, 1.f) },
		{ Vec3(-0.5f, -0.5f, -0.5f), Vec3(-0.2f, -0.1f, -0.2f), Vec2(0.f, 0.f) },

		{ Vec3(0.f, 0.75f, 0.f), Vec3(0.f, 1.f, 0.f), Vec2(0.5f, 1.f) },
		{ Vec3(-0.5f, -0.5f, -0.5f), Vec3(-0.2f, -0.1f, -0.2f), Vec2(1.f, 1.f) },
		{ Vec3(-0.5f, -0.5f, 0.5f), Vec3(-0.2f, -0.1f, 0.2f), Vec2(0.f, 0.f) },

		{ Vec3(0.f, 0.75f, 0.f), Vec3(0.f, 1.f, 0.f), Vec2(0.5f, 1.f) },
		{ Vec3(-0.5f, -0.5f, 0.5f), Vec3(-0.2f, -0.1f, 0.2f), Vec2(1.f, 1.f) },
		{ Vec3(0.5f, -0.5f, 0.5f), Vec3(0.2f, -0.1f, 0.2f), Vec2(0.f, 0.f) },

		{ Vec3(0.5f, -0.5f, 0.5f), Vec3(0.2f, -0.1f, 0.2f), Vec2(0.5f, 1.f) },
		{ Vec3(-0.5f, -0.5f, 0.5f), Vec3(-0.2f, -0.1f, 0.2f), Vec2(0.f, 0.f) },
		{ Vec3(-0.5f, -0.5f, -0.5f), Vec3(-0.2f, -0.1f, -0.2f), Vec2(1.f, 1.f) },

		{ Vec3(-0.5f, -0.5f, -0.5f), Vec3(-0.2f, -0.1f, -0.2f), Vec2(0.5f, 1.f) },
		{ Vec3(0.5f, -0.5f, -0.5f), Vec3(0.2f, -0.1f, -0.2f), Vec2(0.0f, 0.f) },
		{ Vec3(0.5f, -0.5f, 0.5f), Vec3(0.2f, -0.1f, 0.2f), Vec2(1.f, 1.f) }
	};
	UINT	iIndex5[18] = {
		0, 1, 2,
		3, 4, 5,
		6, 7, 8,
		9, 10, 11,
		12, 13, 14,
		15, 16, 17
	};

	for (int i = 0; i < 18; ++i)
	{
		tTexVtx3[i].vNormal = tTexVtx3[i].vNormal.Normalize();
	}

	CDx11Mesh*	pPyramidTexMesh = CreateMesh("PyramidTexMesh", sizeof(TEXVERTEX),
		18, tTexVtx3, sizeof(UINT), 18, iIndex5,
		DXGI_FORMAT_R32_UINT);
	SAFE_RELEASE(pPyramidTexMesh);




	//텍스쳐 피라미드 매쉬2
	TEXVERTEX	tTexVtx5[] =
	{
		{ Vec3(0.f, 0.75f, 0.f), Vec3(0.f, 1.f, 0.f), Vec2(0.5f, 1.f) },
		{ Vec3(0.5f, -0.5f, 0.1f), Vec3(0.2f, -0.1f, 0.2f), Vec2(1.f, 1.f) },
		{ Vec3(0.5f, -0.5f, -0.1f), Vec3(0.2f, -0.1f, -0.2f), Vec2(0.f, 0.f) },

		{ Vec3(0.f, 0.75f, 0.f), Vec3(0.f, 1.f, 0.f), Vec2(0.5f, 1.f) },
		{ Vec3(0.5f, -0.5f, -0.1f), Vec3(0.2f, -0.1f, -0.2f), Vec2(1.f, 1.f) },
		{ Vec3(-0.5f, -0.5f, -0.1f), Vec3(-0.2f, -0.1f, -0.2f), Vec2(0.f, 0.f) },

		{ Vec3(0.f, 0.75f, 0.f), Vec3(0.f, 1.f, 0.f), Vec2(0.5f, 1.f) },
		{ Vec3(-0.5f, -0.5f, -0.1f), Vec3(-0.2f, -0.1f, -0.2f), Vec2(1.f, 1.f) },
		{ Vec3(-0.5f, -0.5f, 0.1f), Vec3(-0.2f, -0.1f, 0.2f), Vec2(0.f, 0.f) },

		{ Vec3(0.f, 0.75f, 0.f), Vec3(0.f, 1.f, 0.f), Vec2(0.5f, 1.f) },
		{ Vec3(-0.5f, -0.5f, 0.1f), Vec3(-0.2f, -0.1f, 0.2f), Vec2(1.f, 1.f) },
		{ Vec3(0.5f, -0.5f, 0.1f), Vec3(0.2f, -0.1f, 0.2f), Vec2(0.f, 0.f) },

		{ Vec3(0.5f, -0.5f, 0.1f), Vec3(0.2f, -0.1f, 0.2f), Vec2(0.5f, 1.f) },
		{ Vec3(-0.5f, -0.5f, 0.1f), Vec3(-0.2f, -0.1f, 0.2f), Vec2(0.f, 0.f) },
		{ Vec3(-0.5f, -0.5f, -0.1f), Vec3(-0.2f, -0.1f, -0.2f), Vec2(1.f, 1.f) },

		{ Vec3(-0.5f, -0.5f, -0.1f), Vec3(-0.2f, -0.1f, -0.2f), Vec2(0.5f, 1.f) },
		{ Vec3(0.5f, -0.5f, -0.1f), Vec3(0.2f, -0.1f, -0.2f), Vec2(0.0f, 0.f) },
		{ Vec3(0.5f, -0.5f, 0.1f), Vec3(0.2f, -0.1f, 0.2f), Vec2(1.f, 1.f) }
	};
	UINT	iIndex6[18] = {
		0, 1, 2,
		3, 4, 5,
		6, 7, 8,
		9, 10, 11,
		12, 13, 14,
		15, 16, 17
	};

	for (int i = 0; i < 18; ++i)
	{
		tTexVtx5[i].vNormal = tTexVtx3[i].vNormal.Normalize();
	}

	pPyramidTexMesh = CreateMesh("PyramidTexMesh2", sizeof(TEXVERTEX),
		18, tTexVtx5, sizeof(UINT), 18, iIndex6,
		DXGI_FORMAT_R32_UINT);
	SAFE_RELEASE(pPyramidTexMesh);



	//큐브 매쉬
	TEXVERTEX	tTexVtx2[24] =
	{	
		//아래
		{ Vec3(-0.5f, -0.5f, 0.5f), Vec3(-0.1f, -0.1f, 0.1f), Vec2(0.f, 0.f) },
		{ Vec3(0.5f, -0.5f, 0.5f), Vec3(0.1f, -0.1f, 0.1f), Vec2(1.f, 0.f) },
		{ Vec3(0.5f, -0.5f, -0.5f), Vec3(0.1f, -0.1f, -0.1f), Vec2(1.f, 1.f) },
		{ Vec3(-0.5f, -0.5f, -0.5f), Vec3(-0.1f, -0.1f, -0.1f), Vec2(0.f, 1.f) },

		//위
		{ Vec3(-0.5f, 0.5f, 0.5f), Vec3(-0.1f, 0.1f, 0.1f), Vec2(0.f, 0.f) },
		{ Vec3(0.5f, 0.5f, 0.5f), Vec3(0.1f, 0.1f, 0.1f), Vec2(0.f, 1.f) },
		{ Vec3(0.5f, 0.5f, -0.5f), Vec3(0.1f, 0.1f, -0.1f), Vec2(1.f, 1.f) },
		{ Vec3(-0.5f, 0.5f, -0.5f), Vec3(-0.1f, 0.1f, -0.1f), Vec2(1.f, 0.f) },

		//좌
		{ Vec3(-0.5f, -0.5f, 0.5f), Vec3(-0.1f, -0.1f, 0.1f), Vec2(0.f, 0.f) },
		{ Vec3(-0.5f, 0.5f, 0.5f), Vec3(-0.1f, 0.1f, 0.1f), Vec2(0.f, 1.f) },
		{ Vec3(-0.5f, 0.5f, -0.5f), Vec3(-0.1f, 0.1f, -0.1f), Vec2(1.f, 1.f) },
		{ Vec3(-0.5f, -0.5f, -0.5f), Vec3(-0.1f, -0.1f, -0.1f), Vec2(1.f, 0.f) },
		
		//우
		{ Vec3(0.5f, -0.5f, 0.5f), Vec3(0.1f, -0.1f, 0.1f), Vec2(0.f, 0.f) },
		{ Vec3(0.5f, -0.5f, -0.5f), Vec3(0.1f, -0.1f, -0.1f), Vec2(0.f, 1.f) },
		{ Vec3(0.5f, 0.5f, -0.5f), Vec3(0.1f, 0.1f, -0.1f), Vec2(1.f, 1.f) },
		{ Vec3(0.5f, 0.5f, 0.5f), Vec3(0.1f, 0.1f, 0.1f), Vec2(1.f, 0.f) },
		
		//앞
		{ Vec3(-0.5f, -0.5f, -0.5f), Vec3(-0.1f, -0.1f, -0.1f), Vec2(0.f, 0.f) },
		{ Vec3(-0.5f, 0.5f, -0.5f), Vec3(-0.1f, 0.1f, -0.1f), Vec2(0.f, 1.f) },
		{ Vec3(0.5f, 0.5f, -0.5f), Vec3(0.1f, 0.1f, -0.1f), Vec2(1.f, 1.f) },
		{ Vec3(0.5f, -0.5f, -0.5f), Vec3(0.1f, -0.1f, -0.1f), Vec2(1.f, 0.f) },

		//뒤
		{ Vec3(-0.5f, -0.5f, 0.5f), Vec3(-0.1f, -0.1f, 0.1f), Vec2(0.f, 0.f) },
		{ Vec3(0.5f, -0.5f, 0.5f), Vec3(0.1f, -0.1f, 0.1f), Vec2(0.f, 1.f) },
		{ Vec3(0.5f, 0.5f, 0.5f), Vec3(0.1f, 0.1f, 0.1f), Vec2(1.f, 1.f) },
		{ Vec3(-0.5f, 0.5f, 0.5f), Vec3(-0.1f, 0.1f, 0.1f), Vec2(1.f, 0.f) },
	};
	UINT	iIndex3[36] = {
		0, 3, 2,
		0, 2, 1,

		4, 5, 6,
		4, 6, 7,

		8, 9, 10,
		8, 10, 11,

		12,13,14,
		12,14,15,

		16,17,18,
		16,18,19,

		20,21,22,
		20,22,23

	};

	for (int i = 0; i < 24; ++i)
	{
		tTexVtx2[i].vNormal = tTexVtx2[i].vNormal.Normalize();
	}

	CDx11Mesh*	pCubeMesh = CreateMesh("CubeMesh", sizeof(TEXVERTEX),
		24, tTexVtx2, sizeof(UINT), 36, iIndex3,
		DXGI_FORMAT_R32_UINT);
	SAFE_RELEASE(pCubeMesh);


	// 사각형 매쉬
	TEXVERTEX	tTexVtx[4] =
	{
		{ Vec3(-0.5f, 0.5f, 0.f), Vec3(0.f, 0.f, -1.f), Vec2(0.f, 0.f) },
		{ Vec3(0.5f, 0.5f, 0.f), Vec3(0.f, 0.f, -1.f), Vec2(1.f, 0.f) },
		{ Vec3(-0.5f, -0.5f, 0.f), Vec3(0.f, 0.f, -1.f), Vec2(0.f, 1.f) },
		{ Vec3(0.5f, -0.5f, 0.f), Vec3(0.f, 0.f, -1.f), Vec2(1.f, 1.f) }
	};

	UINT	iIndex4[6] = { 0, 1, 3, 0, 3, 2 };
	
	CDx11Mesh*	pRectangleMesh = CreateMesh("RectMesh", sizeof(TEXVERTEX),
		4, tTexVtx, sizeof(UINT), 6, iIndex4,
		DXGI_FORMAT_R32_UINT);
	SAFE_RELEASE(pRectangleMesh);


	// UI 사각형 매쉬
	UIVERTEX	tUIVtx[4] =
	{
		{ Vec3(0.f, 0.f, 0.f), Vec2(0.f, 0.f) },
		{ Vec3(1.f, 0.f, 0.f), Vec2(1.f, 0.f) },
		{ Vec3(0.f, 1.f, 0.f), Vec2(0.f, 1.f) },
		{ Vec3(1.f, 1.f, 0.f), Vec2(1.f, 1.f) }
	};

	UINT	iUIIndex[6] = { 0, 1, 3, 0, 3, 2 };

	pRectangleMesh = CreateMesh(RECT_UI_MESH, sizeof(UIVERTEX),
		4, tUIVtx, sizeof(UINT), 6, iUIIndex,
		DXGI_FORMAT_R32_UINT);
	SAFE_RELEASE(pRectangleMesh);



	// Effect용 센터 사각형 메쉬 생성
	UIVERTEX		tEffectCenterVtx[4] =
	{
		{ Vec3(-0.5f, 0.5f, 0.f), Vec2(0.f, 0.f) },
		{ Vec3(0.5f, 0.5f, 0.f), Vec2(1.f, 0.f) },
		{ Vec3(-0.5f, -0.5f, 0.f), Vec2(0.f, 1.f) },
		{ Vec3(0.5f, -0.5f, 0.f), Vec2(1.f, 1.f) }
	};

	WORD	wEffectCenterIndex[6] = { 0, 1, 3, 0, 3, 2 };

	CDx11Mesh* pEffectMesh = CreateMesh(EFFECT_MESH, sizeof(UIVERTEX), 4, tEffectCenterVtx,
		 sizeof(WORD), 6, wEffectCenterIndex, DXGI_FORMAT_R16_UINT);

	SAFE_RELEASE(pEffectMesh);


	// Effect용 LT 사각형 메쉬 생성
	UIVERTEX		tEffectLTVtx[4] =
	{
		{ Vec3(0.f, 0.f, 0.f), Vec2(0.f, 0.f) },
		{ Vec3(1.f, 0.f, 0.f), Vec2(1.f, 0.f) },
		{ Vec3(0.f, -1.f, 0.f), Vec2(0.f, 1.f) },
		{ Vec3(1.f, -1.f, 0.f), Vec2(1.f, 1.f) }
	};

	WORD	wEffectLTIndex[6] = { 0, 1, 3, 0, 3, 2 };

	pEffectMesh = CreateMesh(EFFECT_LT_MESH, sizeof(UIVERTEX),  4, tEffectLTVtx,
		sizeof(WORD), 6, wEffectLTIndex,DXGI_FORMAT_R16_UINT);

	SAFE_RELEASE(pEffectMesh);












	// 구 매쉬
	CDx11Mesh* pSphereMesh = CreateSphere("SphereMesh", 0.5f, 100, 100);
	SAFE_RELEASE(pSphereMesh);



	// 여기서 pSampler는 CreateSampler 함수 호출을 위해 맞춰준 반환형 타입일뿐 여기서 받아와서 쓰겠다는 의미가 아님.
	// Create하면서 Map에 삽입되고, RefCount가 1증가하여 2가됬으므로 릴리즈해서 1로만들어준다.

	CDx11Sampler*	pSampler = CreateSampler(DEFAULT_SAMPLER);
	SAFE_RELEASE(pSampler);

	return true;
}

CDx11Mesh * CDx11ResourcesManager::CreateMesh(const string & strKey,
	UINT iVtxSize, UINT iVtxCount, void * pVertices,
	UINT iIdxSize, UINT iIdxCount, void * pIndices,
	DXGI_FORMAT eIdxFmt,
	D3D11_PRIMITIVE_TOPOLOGY eTopology)
{
	CDx11Mesh*	pMesh = FindMesh(strKey);

	if (pMesh)
		return pMesh;

	pMesh = new CDx11Mesh;

	if (!pMesh->CreateMesh(iVtxSize, iVtxCount, pVertices,
		iIdxSize, iIdxCount, pIndices, eIdxFmt, eTopology))
	{
		SAFE_RELEASE(pMesh);
		return NULL;
	}

	pMesh->AddRef();
	m_mapMesh.insert(make_pair(strKey, pMesh));

	return pMesh;
}

CDx11Mesh * CDx11ResourcesManager::LoadMesh(const string & strKey, 
	TCHAR * pFileName, const string & strPathKey)
{
	CDx11Mesh*	pMesh = FindMesh(strKey);

	if (pMesh)
		return pMesh;
	
	pMesh = new CDx11Mesh;

	if (!pMesh->LoadMesh(strKey, pFileName, strPathKey))
	{
		SAFE_RELEASE(pMesh);
		return NULL;
	}

	pMesh->AddRef();
	m_mapMesh.insert(make_pair(strKey, pMesh));

	return pMesh;
}

CDx11Mesh * CDx11ResourcesManager::LoadMesh(const string & strKey, TCHAR * pFileName, vector<vector<TCHAR*>>& vecTexName, const string & strPathKey)
{
	CDx11Mesh*	pMesh = FindMesh(strKey);

	if (pMesh)
		return pMesh;

	pMesh = new CDx11Mesh;

	if (!pMesh->LoadMesh(strKey, pFileName, vecTexName, strPathKey))
	{
		SAFE_RELEASE(pMesh);
		return NULL;
	}

	pMesh->AddRef();
	m_mapMesh.insert(make_pair(strKey, pMesh));

	return pMesh;
}

CDx11Mesh * CDx11ResourcesManager::CreateSphere(const string & strKey, float fRadius, UINT iSliceCount, UINT iStackCount)
{
	CDx11Mesh*	pMesh = FindMesh(strKey);

	if (pMesh)
		return pMesh;

	pMesh = new CDx11Mesh;

	if (!pMesh->CreateSphere(fRadius, iSliceCount, iStackCount))
	{
		SAFE_RELEASE(pMesh);
		return NULL;
	}

	pMesh->AddRef();
	m_mapMesh.insert(make_pair(strKey, pMesh));

	return pMesh;
}

CDx11Mesh * CDx11ResourcesManager::FindMesh(const string & strKey)
{
	unordered_map<string, class CDx11Mesh*>::iterator	iter = m_mapMesh.find(strKey);

	if (iter == m_mapMesh.end())
		return NULL;

	iter->second->AddRef();

	return iter->second;
}

CDx11Texture * CDx11ResourcesManager::LoadTexture(const string & strKey, TCHAR * pFileName, const string & strPathKey)
{
	CDx11Texture*	pTexture = FindTexture(strKey);

	if (pTexture)
		return pTexture;

	if (!pFileName)
		return NULL;

	pTexture = new CDx11Texture;

	if (!pTexture->LoadTexture(pFileName, strPathKey))
	{
		SAFE_RELEASE(pTexture);
		return NULL;
	}
	pTexture->AddRef();
	m_mapTexture.insert(make_pair(strKey, pTexture));

	return pTexture;
}

CDx11Texture * CDx11ResourcesManager::LoadTexture(const string & strKey, string strPath)
{
	CDx11Texture*	pTexture = FindTexture(strKey);

	if (pTexture)
		return pTexture;

	pTexture = new CDx11Texture;

	if (!pTexture->LoadTexture(strPath))
	{
		SAFE_RELEASE(pTexture);
		return NULL;
	}
	pTexture->AddRef();
	m_mapTexture.insert(make_pair(strKey, pTexture));

	return pTexture;
}

CDx11Texture * CDx11ResourcesManager::FindTexture(const string & strKey)
{
	unordered_map<string, CDx11Texture*>::iterator	iter = m_mapTexture.find(strKey);

	if (iter == m_mapTexture.end())
		return NULL;

	iter->second->AddRef();

	return iter->second;
}

CDx11Sampler * CDx11ResourcesManager::CreateSampler(const string & strKey,
	D3D11_FILTER eFilter, D3D11_TEXTURE_ADDRESS_MODE u, 
	D3D11_TEXTURE_ADDRESS_MODE v, D3D11_TEXTURE_ADDRESS_MODE w, 
	D3D11_COMPARISON_FUNC eFunc)
{
	CDx11Sampler*	pSampler = FindSampler(strKey);
	if (pSampler)
		return pSampler;

	pSampler = new CDx11Sampler;

	if (!pSampler->CreateSampler(eFilter, u, v, w, eFunc))
	{
		SAFE_RELEASE(pSampler);
		return NULL;
	}

	pSampler->AddRef();
	m_mapSampler.insert(make_pair(strKey, pSampler));
	return pSampler;
}

CDx11Sampler * CDx11ResourcesManager::FindSampler(const string & strKey)
{
	unordered_map<string, CDx11Sampler*>::iterator	iter = m_mapSampler.find(strKey);

	if (iter == m_mapSampler.end())
		return NULL;

	iter->second->AddRef();

	return iter->second;
}
