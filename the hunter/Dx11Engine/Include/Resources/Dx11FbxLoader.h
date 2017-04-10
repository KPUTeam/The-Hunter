#pragma once
#include "../Dx11Object.h"

DX11_BEGIN

// ���� ��� ����
enum FBX_MATERIAL_NODE
{
	FMN_LAMBERT,	// Diffuse�� ó��
	FMN_PHONG		// �Ϲ������� �� ���. (�ȼ� ������ ó��)
};

// ���� Ÿ��
enum FBX_MATERIAL_TYPE
{
	FMT_NONE,
	FMT_COLOR,		// Dif, Amb, Spc �� ����
	FMT_TEXTURE,	// Diffuse Texture �� ����
	FMT_BOTH,		// �Ѵ� ����
	FMT_MAX
};


// ���� ������ ����ü
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


// �Ž� ������ ����ü
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