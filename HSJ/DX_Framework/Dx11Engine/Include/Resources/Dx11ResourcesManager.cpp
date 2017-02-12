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
	Safe_Release_Map(m_mapSampler);
	Safe_Release_Map(m_mapTexture);
	Safe_Release_Map(m_mapMesh);
}

bool CDx11ResourcesManager::Init()
{
	COLORVERTEX	tColorVtx[3] = 
	{
		{ Vec3(0.f, 0.5f, 0.f), Vec4(1.f, 0.f, 0.f, 1.f) },
		{ Vec3(0.5f, -0.5f, 0.f), Vec4(0.f, 1.f, 0.f, 1.f) },
		{ Vec3(-0.5f, -0.5f, 0.f), Vec4(0.f, 0.f, 1.f, 1.f) }
	};

	UINT	iIndex[3] = { 0, 1, 2 };

	CDx11Mesh*	pMesh = CreateMesh(TRIANGLE_MESH, sizeof(COLORVERTEX),
		3, tColorVtx, sizeof(UINT), 3, iIndex,
		DXGI_FORMAT_R32_UINT);

	SAFE_RELEASE(pMesh);

	TEXVERTEX	tTexVtx[4]	=
	{
		{ Vec3(-0.5f, 0.5f, 0.f), Vec3(0.f, 0.f, -1.f), Vec2(0.f, 0.f) },
		{ Vec3(0.5f, 0.5f, 0.f), Vec3(0.f, 0.f, -1.f), Vec2(1.f, 0.f) },
		{ Vec3(-0.5f, -0.5f, 0.f), Vec3(0.f, 0.f, -1.f), Vec2(0.f, 1.f) },
		{ Vec3(0.5f, -0.5f, 0.f), Vec3(0.f, 0.f, -1.f), Vec2(1.f, 1.f) }
	};

	UINT	iIndex1[6] = { 0, 1, 3, 0, 3, 2 };

	pMesh = CreateMesh(RECT_TEX_MESH, sizeof(TEXVERTEX),
		4, tTexVtx, sizeof(UINT), 6, iIndex1,
		DXGI_FORMAT_R32_UINT);

	SAFE_RELEASE(pMesh);
	
	CDx11Sampler*	pSampler = CreateSampler(DEFAULT_SAMPLER);

	SAFE_RELEASE(pSampler);

	// 기본 구 메쉬 생성
	pMesh = CreateSphere(SPHERE_MESH, 0.5f, 100, 100);

	SAFE_RELEASE(pMesh);

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

CDx11Texture * CDx11ResourcesManager::LoadTexture(const string & strKey,
	TCHAR * pFileName, const string & strPathKey)
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
	unordered_map<string, class CDx11Sampler*>::iterator	iter = m_mapSampler.find(strKey);

	if (iter == m_mapSampler.end())
		return NULL;

	iter->second->AddRef();

	return iter->second;
}
