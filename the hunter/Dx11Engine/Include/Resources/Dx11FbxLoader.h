#pragma once
#include "../Dx11Object.h"

DX11_BEGIN

// 재질 노드 종류
enum FBX_MATERIAL_NODE
{
	FMN_LAMBERT,	// Diffuse만 처리
	FMN_PHONG		// 일반적으로 퐁 방식. (픽셀 단위의 처리)
};

// 재질 타입
enum FBX_MATERIAL_TYPE
{
	FMT_NONE,
	FMT_COLOR,		// Dif, Amb, Spc 만 존재
	FMT_TEXTURE,	// Diffuse Texture 만 존재
	FMT_BOTH,		// 둘다 존재
	FMT_MAX
};


// 재질 데이터 구조체
typedef struct _tagMaterialData
{
	FBX_MATERIAL_NODE	eNode;
	FBX_MATERIAL_TYPE	eType;
	Vec3	vDif;
	Vec3	vAmb;
	Vec3	vSpc;
	Vec3	vEmv;
	float	fSpcPower;
	float	fTransparencyFactor;
	string	strDiffuseTex;
	string	strSpecularTex;
	string	strNormalMapTex;
}FBXMATERIALDATA, *PFBXMATERIALDATA;


// 매쉬 데이터 구조체
typedef struct _tagMeshData
{
	int				iPolygonCount;
	vector<Vec3>	vecPos;
	vector<Vec3>	vecNormal;
	vector<Vec2>	vecUV;
	vector<Vec3>	vecTangent;
	vector<Vec3>	vecBinormal;
	vector<Vec4>	vecWeights;
	vector<Vec4>	vecIndices;
	vector<vector<UINT>>		vecIndex;
	vector<PFBXMATERIALDATA>	vecMaterial;
	bool			bAnimation;
	bool			bBump;
}MESHDATA, *PMESHDATA;


class DX11_DLL CDx11FbxLoader :
	public CDx11Object
{
private:
	friend class CDx11Mesh;

private:
	CDx11FbxLoader();
	~CDx11FbxLoader();

private:
	FbxManager*	m_pManager;
	FbxScene*	m_pScene;
	vector<PMESHDATA> m_vecMeshData;

public:
	vector<PMESHDATA>& GetMeshContainer();
	int GetContainerCount();
	int GetSubsetCount(int iContainer);
	bool LoadMesh(char* pFileName);

private:
	void Triangulate(FbxNode* pNode);
	bool LoadNodeData(FbxNode* pNode);
	bool LoadMeshData(FbxMesh* pMesh);
	bool LoadMaterialData(FbxSurfaceMaterial* pMaterial);
	Vec3 GetMaterialProperty(FbxSurfaceMaterial* pMaterial, PFBXMATERIALDATA pMtrlData,
		const char* pPropertyName, const char* pPropertyFactorName);
	bool GetMaterialTextureName(FbxSurfaceMaterial* pMaterial, PFBXMATERIALDATA pMtrlData,
		const char* pPropertyName);
};

DX11_END