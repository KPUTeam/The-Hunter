#include "Dx11ResourcesManager.h"
#include "Dx11Mesh.h"
#include "Dx11Texture.h"
#include "../Core/Dx11Debug.h"

DX11_USING

DX11_SINGLE_DEFINITION(CDx11ResourcesManager)

CDx11ResourcesManager::CDx11ResourcesManager()
{
	SetTypeName<CDx11ResourcesManager>();
}

CDx11ResourcesManager::~CDx11ResourcesManager()
{
	Safe_Release_Map(m_mapTexture);
	Safe_Release_Map(m_mapMesh);
}

bool CDx11ResourcesManager::Init()
{
	COLORVERTEX		tVtx[3]	=
	{
		{ Vec3(0.f, 0.5f, 0.f), Vec4(1.f, 0.f, 0.f, 1.f) },
		{ Vec3(0.5f, -0.5f, 0.f), Vec4(0.f, 1.f, 0.f, 1.f) },
		{ Vec3(-0.5f, -0.5f, 0.f), Vec4(0.f, 0.f, 1.f, 1.f)  }
	};

	WORD	wIndex[3] = {0, 1, 2};

	CDx11Mesh*	pMesh = CreateMesh("Triangle", tVtx, 3, sizeof(COLORVERTEX),
		wIndex, 3, sizeof(WORD), DXGI_FORMAT_R16_UINT);

	SAFE_RELEASE(pMesh);

	TEXVERTEX		tVtxTex[3] =
	{
		{ Vec3(0.f, 0.5f, 0.f), Vec3(0.f, 0.f, 1.f), Vec2(0.5f, 0.f) },
		{ Vec3(0.5f, -0.5f, 0.f), Vec3(0.f, 0.f, 1.f), Vec2(1.f, 1.f) },
		{ Vec3(-0.5f, -0.5f, 0.f), Vec3(0.f, 0.f, 1.f), Vec2(0.f, 1.f) }
	};

	WORD	wIndexTex[3] = { 0, 1, 2 };

	pMesh = CreateMesh("TriangleTex", tVtxTex, 3, sizeof(TEXVERTEX),
		wIndexTex, 3, sizeof(WORD), DXGI_FORMAT_R16_UINT);

	SAFE_RELEASE(pMesh);

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

	pMesh =  CreateMesh("CubeMesh", tTexVtx2, 24, sizeof(TEXVERTEX), iIndex3, 36, sizeof(UINT), DXGI_FORMAT_R32_UINT);
	SAFE_RELEASE(pMesh);

	// 프러스텀 매쉽
	TEXVERTEX	tTexVtx3[24] =
	{
		//아래
		{ Vec3(-1, -1, 1), Vec3(-0.1f, -0.1f, 0.1f), Vec2(0.f, 0.f) },
		{ Vec3(1, -1, 1), Vec3(0.1f, -0.1f, 0.1f), Vec2(1.f, 0.f) },
		{ Vec3(1, -1, -1), Vec3(0.1f, -0.1f, -0.1f), Vec2(1.f, 1.f) },
		{ Vec3(-1, -1, -1), Vec3(-0.1f, -0.1f, -0.1f), Vec2(0.f, 1.f) },

		//위
		{ Vec3(-1, 1, 1), Vec3(-0.1f, 0.1f, 0.1f), Vec2(0.f, 0.f) },
		{ Vec3(1, 1, 1), Vec3(0.1f, 0.1f, 0.1f), Vec2(0.f, 1.f) },
		{ Vec3(1, 1, -1), Vec3(0.1f, 0.1f, -0.1f), Vec2(1.f, 1.f) },
		{ Vec3(-1, 1, -1), Vec3(-0.1f, 0.1f, -0.1f), Vec2(1.f, 0.f) },

		//좌
		{ Vec3(-1, -1, 1), Vec3(-0.1f, -0.1f, 0.1f), Vec2(0.f, 0.f) },
		{ Vec3(-1, 1, 1), Vec3(-0.1f, 0.1f, 0.1f), Vec2(0.f, 1.f) },
		{ Vec3(-1, 1, -1), Vec3(-0.1f, 0.1f, -0.1f), Vec2(1.f, 1.f) },
		{ Vec3(-1, -1, -1), Vec3(-0.1f, -0.1f, -0.1f), Vec2(1.f, 0.f) },

		//우
		{ Vec3(1, -1, 1), Vec3(0.1f, -0.1f, 0.1f), Vec2(0.f, 0.f) },
		{ Vec3(1, -1, -1), Vec3(0.1f, -0.1f, -0.1f), Vec2(0.f, 1.f) },
		{ Vec3(1, 1, -1), Vec3(0.1f, 0.1f, -0.1f), Vec2(1.f, 1.f) },
		{ Vec3(1, 1, 1), Vec3(0.1f, 0.1f, 0.1f), Vec2(1.f, 0.f) },

		//앞
		{ Vec3(-1, -1, -1), Vec3(-0.1f, -0.1f, -0.1f), Vec2(0.f, 0.f) },
		{ Vec3(-1, 1, -1), Vec3(-0.1f, 0.1f, -0.1f), Vec2(0.f, 1.f) },
		{ Vec3(1, 1, -1), Vec3(0.1f, 0.1f, -0.1f), Vec2(1.f, 1.f) },
		{ Vec3(1, -1, -1), Vec3(0.1f, -0.1f, -0.1f), Vec2(1.f, 0.f) },

		//뒤
		{ Vec3(-1, -1, 1), Vec3(-0.1f, -0.1f, 0.1f), Vec2(0.f, 0.f) },
		{ Vec3(1, -1, 1), Vec3(0.1f, -0.1f, 0.1f), Vec2(0.f, 1.f) },
		{ Vec3(1, 1, 1), Vec3(0.1f, 0.1f, 0.1f), Vec2(1.f, 1.f) },
		{ Vec3(-1, 1, 1), Vec3(-0.1f, 0.1f, 0.1f), Vec2(1.f, 0.f) },
	};

	UINT	iIndex4[36] = {
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

	//for (int i = 0; i < 24; ++i)
	//{
	//	tTexVtx3[i].vNormal = tTexVtx3[i].vNormal.Normalize();
	//}

	pMesh = CreateMesh(FRUSTRUM_MESH, tTexVtx3, 24, sizeof(TEXVERTEX), iIndex4, 36, sizeof(UINT), DXGI_FORMAT_R32_UINT);
	SAFE_RELEASE(pMesh);

	// UI용 사각형메쉬 생성
	VERTEX2D		tUIVtx[4] =
	{
		{ Vec3(0.f, 0.f, 0.f), Vec2(0.f, 0.f) },
		{ Vec3(1.f, 0.f, 0.f), Vec2(1.f, 0.f) },
		{ Vec3(0.f, 1.f, 0.f), Vec2(0.f, 1.f) },
		{ Vec3(1.f, 1.f, 0.f), Vec2(1.f, 1.f) }
	};

	WORD	wUIIndex[6] = { 0, 1, 3, 0, 3, 2 };

	pMesh = CreateMesh("UIMesh", tUIVtx, 4, sizeof(VERTEX2D),
		wUIIndex, 6, sizeof(WORD), DXGI_FORMAT_R16_UINT);

	SAFE_RELEASE(pMesh);

	// Effect용 센터 사각형 메쉬 생성
	VERTEX2D		tEffectCenterVtx[4] =
	{
		{ Vec3(-0.5f, 0.5f, 0.f), Vec2(0.f, 0.f) },
		{ Vec3(0.5f, 0.5f, 0.f), Vec2(1.f, 0.f) },
		{ Vec3(-0.5f, -0.5f, 0.f), Vec2(0.f, 1.f) },
		{ Vec3(0.5f, -0.5f, 0.f), Vec2(1.f, 1.f) }
	};

	WORD	wEffectCenterIndex[6] = { 0, 1, 3, 0, 3, 2 };

	pMesh = CreateMesh("EffectCenterMesh", tEffectCenterVtx, 4, sizeof(VERTEX2D),
		wEffectCenterIndex, 6, sizeof(WORD), DXGI_FORMAT_R16_UINT);

	SAFE_RELEASE(pMesh);

	// Effect용 LT 사각형 메쉬 생성
	VERTEX2D		tEffectLTVtx[4] =
	{
		{ Vec3(0.f, 0.f, 0.f), Vec2(0.f, 0.f) },
		{ Vec3(1.f, 0.f, 0.f), Vec2(1.f, 0.f) },
		{ Vec3(0.f, -1.f, 0.f), Vec2(0.f, 1.f) },
		{ Vec3(1.f, -1.f, 0.f), Vec2(1.f, 1.f) }
	};

	WORD	wEffectLTIndex[6] = { 0, 1, 3, 0, 3, 2 };

	pMesh = CreateMesh(EFFECT_LT_MESH, tEffectLTVtx, 4, sizeof(VERTEX2D),
		wEffectLTIndex, 6, sizeof(WORD), DXGI_FORMAT_R16_UINT);

	SAFE_RELEASE(pMesh);

	// 피라미드 생성
	Vec3	vN1(1.f, -2.f, -1.f);
	vN1 /= vN1.Length();

	Vec3	vN2(-1.f, -1.f, -1.f);
	vN2 /= vN2.Length();

	Vec3	vN3(-1.f, -2.f, 1.f);
	vN3 /= vN3.Length();

	Vec3	vN4(1.f, -1.f, 1.f);
	vN4 /= vN4.Length();

	TEXVERTEX		tVtxPyramidTex[8] =
	{
		{ Vec3(0.f, 0.5f, 0.f), Vec3(0.f, 0.f, -1.f), Vec2(0.f, 1.f) },
		{ Vec3(0.5f, -0.5f, -0.5f), Vec3(0.f, 0.f, -1.f), Vec2(0.5f, 1.f) },
		{ Vec3(-0.5f, -0.5f, -0.5f), Vec3(0.f, 0.f, -1.f), Vec2(0.f, 0.5f) },
		{ Vec3(-0.5f, -0.5f, 0.5f), Vec3(0.f, 0.f, -1.f), Vec2(0.5f, 0.f) },
		{ Vec3(0.5f, -0.5f, 0.5f), Vec3(0.f, 0.f, -1.f), Vec2(1.f, 0.5f) },
		{ Vec3(0.f, 0.5f, 0.f), Vec3(1.f, 0.f, 0.f), Vec2(0.f, 0.f) },
		{ Vec3(0.f, 0.5f, 0.f), Vec3(0.f, 0.f, 1.f), Vec2(1.f, 0.f) },
		{ Vec3(0.f, 0.5f, 0.f), Vec3(-1.f, 0.f, 0.f), Vec2(1.f, 1.f) },
	};


	WORD		wIndexPyramidTex[18] = { 0, 1, 2 , 5, 2, 3, 6, 3, 4, 7, 4, 1, 1, 4, 3, 2, 1, 3 };

	pMesh = CreateMesh("Pyramid", tVtxPyramidTex, 8, sizeof(TEXVERTEX),
		wIndexPyramidTex, 18, sizeof(WORD), DXGI_FORMAT_R16_UINT);

	SAFE_RELEASE(pMesh);

	MESHDATA	tMeshData;

	//CreateBox(tMeshData, 1, 1, 1);
	CreateSphere(tMeshData, 1.f, 17, 17);
	vector<COLORVERTEX>	vecSphereV;

	for (size_t i = 0; i < tMeshData.vecVertices.size(); ++i)
	{
		COLORVERTEX	v = {};
		v.vColor = Vec4(1.f, 0.f, 0.f, 1.f);
		v.vPos = tMeshData.vecVertices[i].vPos;
		vecSphereV.push_back(v);
	}
	pMesh = CreateMesh("ColorSphere", &vecSphereV[0], vecSphereV.size(),
		sizeof(COLORVERTEX), &tMeshData.vecIndices[0], tMeshData.vecIndices.size(),
		sizeof(UINT), DXGI_FORMAT_R32_UINT);

	SAFE_RELEASE(pMesh);

	vector<POSVERTEX>	vecSphereP;

	for (size_t i = 0; i < tMeshData.vecVertices.size(); ++i)
	{
		POSVERTEX	v = {};
		v.vPos = tMeshData.vecVertices[i].vPos;
		vecSphereP.push_back(v);
	}

	pMesh	= CreateMesh("ColliderSphere", &vecSphereP[0], vecSphereP.size(),
		sizeof(POSVERTEX), &tMeshData.vecIndices[0], tMeshData.vecIndices.size(),
		sizeof(UINT), DXGI_FORMAT_R32_UINT);

	SAFE_RELEASE(pMesh);

	//pMesh = LoadMesh("tuttle", L"tuttle.FBX");

	return true;
}

CDx11Mesh * CDx11ResourcesManager::CreateMesh(const string & strKey, 
	void * pVertices, UINT iVertexCount, UINT iVertexStride, void * pIndices,
	UINT iIndexCount, UINT iIndexStride, DXGI_FORMAT eIndexFmt, 
	D3D11_PRIMITIVE_TOPOLOGY ePrimitive)
{
	CDx11Mesh*	pMesh = FindMesh(strKey);

	if (pMesh)
		return pMesh;

	pMesh = new CDx11Mesh;

	if (!pMesh->CreateBuffer(pVertices, iVertexCount, iVertexStride, pIndices, iIndexCount,
		iIndexStride, eIndexFmt, ePrimitive))
	{
		SAFE_RELEASE(pMesh);
		return NULL;
	}

	pMesh->AddRef();

	m_mapMesh.insert(make_pair(strKey, pMesh));

	return pMesh;
}

CDx11Mesh * CDx11ResourcesManager::LoadMesh(const string & strKey, TCHAR* pFileName,
	const string & strPathKey)
{
	LARGE_INTEGER	tFreq;
	QueryPerformanceFrequency(&tFreq);

	LARGE_INTEGER	tStart;
	QueryPerformanceCounter(&tStart);

	CDx11Mesh*	pMesh = FindMesh(strKey);

	if (pMesh)
		return pMesh;

	pMesh = new CDx11Mesh;

	if (!pMesh->LoadMesh(strKey, pFileName, strPathKey))
	{
		SAFE_RELEASE(pMesh);
		return NULL;
	}

	LARGE_INTEGER	tEnd;
	QueryPerformanceCounter(&tEnd);

	float	fTime = (float)(tEnd.QuadPart - tStart.QuadPart) / tFreq.QuadPart;

	char	strTime[128] = {};
	sprintf_s(strTime, "Load Time : %f", fTime);
	DX11_GET_SINGLE(CDx11Debug)->OutputVisualDebug(strTime);

	pMesh->AddRef();

	m_mapMesh.insert(make_pair(strKey, pMesh));

	return pMesh;
}

CDx11Mesh * CDx11ResourcesManager::LoadMesh(const string & strKey, const wchar_t* pFullPath)
{
	LARGE_INTEGER	tFreq;
	QueryPerformanceFrequency(&tFreq);

	LARGE_INTEGER	tStart;
	QueryPerformanceCounter(&tStart);

	CDx11Mesh*	pMesh = FindMesh(strKey);

	if (pMesh)
		return pMesh;

	pMesh = new CDx11Mesh;

	if (!pMesh->LoadMesh(strKey, pFullPath))
	{
		SAFE_RELEASE(pMesh);
		return NULL;
	}

	LARGE_INTEGER	tEnd;
	QueryPerformanceCounter(&tEnd);

	float	fTime = (float)(tEnd.QuadPart - tStart.QuadPart) / tFreq.QuadPart;

	char	strTime[128] = {};
	sprintf_s(strTime, "Load Time : %f", fTime);
	DX11_GET_SINGLE(CDx11Debug)->OutputVisualDebug(strTime);

	pMesh->AddRef();

	m_mapMesh.insert(make_pair(strKey, pMesh));

	return pMesh;
}

bool CDx11ResourcesManager::CreateBox(MESHDATA& tMeshData,
	UINT iWidth, UINT iHeight, UINT iDepth)
{
	BUMPVERTEX v[24];

	float w2 = 0.5f*iWidth;
	float h2 = 0.5f*iHeight;
	float d2 = 0.5f*iDepth;

	// Fill in the front face vertex data.
	v[0] = BUMPVERTEX(-w2, -h2, -d2, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	v[1] = BUMPVERTEX(-w2, +h2, -d2, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	v[2] = BUMPVERTEX(+w2, +h2, -d2, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	v[3] = BUMPVERTEX(+w2, -h2, -d2, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f);

	// Fill in the back face vertex data.
	v[4] = BUMPVERTEX(-w2, -h2, +d2, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f);
	v[5] = BUMPVERTEX(+w2, -h2, +d2, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	v[6] = BUMPVERTEX(+w2, +h2, +d2, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	v[7] = BUMPVERTEX(-w2, +h2, +d2, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f);

	// Fill in the top face vertex data.
	v[8] = BUMPVERTEX(-w2, +h2, -d2, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	v[9] = BUMPVERTEX(-w2, +h2, +d2, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	v[10] = BUMPVERTEX(+w2, +h2, +d2, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	v[11] = BUMPVERTEX(+w2, +h2, -d2, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f);

	// Fill in the bottom face vertex data.
	v[12] = BUMPVERTEX(-w2, -h2, -d2, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f);
	v[13] = BUMPVERTEX(+w2, -h2, -d2, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	v[14] = BUMPVERTEX(+w2, -h2, +d2, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	v[15] = BUMPVERTEX(-w2, -h2, +d2, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f);

	// Fill in the left face vertex data.
	v[16] = BUMPVERTEX(-w2, -h2, +d2, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	v[17] = BUMPVERTEX(-w2, +h2, +d2, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
	v[18] = BUMPVERTEX(-w2, +h2, -d2, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);
	v[19] = BUMPVERTEX(-w2, -h2, -d2, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f);

	// Fill in the right face vertex data.
	v[20] = BUMPVERTEX(+w2, -h2, -d2, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f);
	v[21] = BUMPVERTEX(+w2, +h2, -d2, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f);
	v[22] = BUMPVERTEX(+w2, +h2, +d2, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f);
	v[23] = BUMPVERTEX(+w2, -h2, +d2, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);

	tMeshData.vecVertices.assign(&v[0], &v[24]);

	//
	// Create the indices.
	//

	UINT i[36];

	// Fill in the front face index data
	i[0] = 0; i[1] = 1; i[2] = 2;
	i[3] = 0; i[4] = 2; i[5] = 3;

	// Fill in the back face index data
	i[6] = 4; i[7] = 5; i[8] = 6;
	i[9] = 4; i[10] = 6; i[11] = 7;

	// Fill in the top face index data
	i[12] = 8; i[13] = 9; i[14] = 10;
	i[15] = 8; i[16] = 10; i[17] = 11;

	// Fill in the bottom face index data
	i[18] = 12; i[19] = 13; i[20] = 14;
	i[21] = 12; i[22] = 14; i[23] = 15;

	// Fill in the left face index data
	i[24] = 16; i[25] = 17; i[26] = 18;
	i[27] = 16; i[28] = 18; i[29] = 19;

	// Fill in the right face index data
	i[30] = 20; i[31] = 21; i[32] = 22;
	i[33] = 20; i[34] = 22; i[35] = 23;

	tMeshData.vecIndices.assign(&i[0], &i[36]);

	return true;
}

bool CDx11ResourcesManager::CreateSphere(MESHDATA & tMeshData, 
	float fRadius, UINT iSlice, UINT iStack)
{
	tMeshData.vecVertices.clear();
	tMeshData.vecIndices.clear();

	BUMPVERTEX topVertex(0.0f, +fRadius, 0.0f, 0.0f, +1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	BUMPVERTEX bottomVertex(0.0f, -fRadius, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);

	tMeshData.vecVertices.push_back(topVertex);

	float phiStep = DX11_PI / iStack;
	float thetaStep = 2.0f*DX11_PI / iSlice;

	// Compute vertices for each stack ring (do not count the poles as rings).
	for (UINT i = 1; i <= iStack - 1; ++i)
	{
		float phi = i*phiStep;

		// Vertices of ring.
		for (UINT j = 0; j <= iSlice; ++j)
		{
			float theta = j*thetaStep;

			BUMPVERTEX v;

			// spherical to cartesian
			v.vPos.x = fRadius*sinf(phi)*cosf(theta);
			v.vPos.y = fRadius*cosf(phi);
			v.vPos.z = fRadius*sinf(phi)*sinf(theta);

			// Partial derivative of P with respect to theta
			v.vTangent.x = -fRadius*sinf(phi)*sinf(theta);
			v.vTangent.y = 0.0f;
			v.vTangent.z = +fRadius*sinf(phi)*cosf(theta);

			XMVECTOR T = XMLoadFloat3(&v.vTangent);
			XMStoreFloat3(&v.vTangent, XMVector3Normalize(T));

			XMVECTOR p = XMLoadFloat3(&v.vPos);
			XMStoreFloat3(&v.vNormal, XMVector3Normalize(p));

			v.vTex.x = theta / DX11_PI;
			v.vTex.y = phi / DX11_PI;

			tMeshData.vecVertices.push_back(v);
		}
	}

	tMeshData.vecVertices.push_back(bottomVertex);


	for (UINT i = 1; i <= iSlice; ++i)
	{
		tMeshData.vecIndices.push_back(0);
		tMeshData.vecIndices.push_back(i + 1);
		tMeshData.vecIndices.push_back(i);
	}

	//
	// Compute indices for inner stacks (not connected to poles).
	//

	// Offset the indices to the index of the first vertex in the first ring.
	// This is just skipping the top pole vertex.
	UINT baseIndex = 1;
	UINT ringVertexCount = iSlice + 1;
	for (UINT i = 0; i < iStack - 2; ++i)
	{
		for (UINT j = 0; j < iSlice; ++j)
		{
			tMeshData.vecIndices.push_back(baseIndex + i*ringVertexCount + j);
			tMeshData.vecIndices.push_back(baseIndex + i*ringVertexCount + j + 1);
			tMeshData.vecIndices.push_back(baseIndex + (i + 1)*ringVertexCount + j);

			tMeshData.vecIndices.push_back(baseIndex + (i + 1)*ringVertexCount + j);
			tMeshData.vecIndices.push_back(baseIndex + i*ringVertexCount + j + 1);
			tMeshData.vecIndices.push_back(baseIndex + (i + 1)*ringVertexCount + j + 1);
		}
	}

	UINT southPoleIndex = (UINT)tMeshData.vecVertices.size() - 1;

	// Offset the indices to the index of the first vertex in the last ring.
	baseIndex = southPoleIndex - ringVertexCount;

	for (UINT i = 0; i < iSlice; ++i)
	{
		tMeshData.vecIndices.push_back(southPoleIndex);
		tMeshData.vecIndices.push_back(baseIndex + i);
		tMeshData.vecIndices.push_back(baseIndex + i + 1);
	}

	return true;
}

CDx11Mesh * CDx11ResourcesManager::FindMesh(const string & strKey)
{
	unordered_map<string, class CDx11Mesh*>::iterator	iter = m_mapMesh.find(strKey);

	if (iter == m_mapMesh.end())
		return NULL;

	iter->second->AddRef();

	return iter->second;
}

CDx11Texture * CDx11ResourcesManager::LoadTexture(
	const string & strKey, TCHAR * pFileName, 
	const string & strPathKey)
{
	CDx11Texture*	pTexture = FindTexture(strKey);

	if (pTexture)
		return pTexture;

	pTexture = new CDx11Texture;

	if (!pTexture->LoadTexture(pFileName, strPathKey))
	{
		SAFE_RELEASE(pTexture);
		return NULL;
	}

	pTexture->SetTag(strKey);

	pTexture->AddRef();
	m_mapTexture.insert(make_pair(strKey, pTexture));

	return pTexture;
}

CDx11Texture * CDx11ResourcesManager::LoadTexture(const string & strKey, 
	const string & strPath)
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

	pTexture->SetTag(strKey);

	pTexture->AddRef();
	m_mapTexture.insert(make_pair(strKey, pTexture));

	return pTexture;
}

CDx11Texture * CDx11ResourcesManager::CreateTexture(const string & strKey,
	UINT iWidth, UINT iHeight, UINT iArraySize, DXGI_FORMAT eFmt,
	D3D11_USAGE eUsage, D3D11_BIND_FLAG eBindFlag, int iCpuFlag)
{
	CDx11Texture*	pTexture = FindTexture(strKey);

	if (pTexture)
		return pTexture;

	pTexture = new CDx11Texture;

	if (!pTexture->CreateTexture(iWidth, iHeight, iArraySize, eFmt,
		eUsage, eBindFlag, iCpuFlag))
	{
		SAFE_RELEASE(pTexture);
		return NULL;
	}

	pTexture->SetTag(strKey);

	pTexture->AddRef();
	m_mapTexture.insert(make_pair(strKey, pTexture));

	return pTexture;
}

CDx11Texture * CDx11ResourcesManager::CreateTexture(const string & strKey,
	UINT iLength, UINT iArraySize, DXGI_FORMAT eFmt, D3D11_USAGE eUsage,
	D3D11_BIND_FLAG eBindFlag, int iCpuFlag)
{
	CDx11Texture*	pTexture = FindTexture(strKey);

	if (pTexture)
		return pTexture;

	pTexture = new CDx11Texture;

	if (!pTexture->CreateTexture(iLength, iArraySize, eFmt,
		eUsage, eBindFlag, iCpuFlag))
	{
		SAFE_RELEASE(pTexture);
		return NULL;
	}

	pTexture->SetTag(strKey);

	pTexture->AddRef();
	m_mapTexture.insert(make_pair(strKey, pTexture));

	return pTexture;
}

CDx11Texture * CDx11ResourcesManager::FindTexture(const string & strKey)
{
	unordered_map<string, class CDx11Texture*>::iterator	iter = m_mapTexture.find(strKey);

	if (iter == m_mapTexture.end())
		return NULL;

	iter->second->AddRef();

	return iter->second;
}

