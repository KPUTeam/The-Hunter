#pragma once
#include "Dx11Resources.h"

DX11_BEGIN

enum FBX_MATERIAL_NODE
{
	FMN_LAMBERT,
	FMN_PHONG
};

enum FBX_MATERIAL_TYPE
{
	FMT_NONE,
	FMT_COLOR,
	FMT_TEXTURE,
	FMT_BOTH,
	FMT_MAX,
};

typedef struct _tagFbxWeightIndices
{
	UINT	iBoneIndex;
	double	dWeight;
}FBXWEIGHTINDICES, *PFBXWEIGHTINDICES;

typedef struct _tagFbxMaterialData
{
	FBX_MATERIAL_NODE	eNode;
	FBX_MATERIAL_TYPE	eType;
	FbxVector4	vDif;
	FbxVector4	vAmb;
	FbxVector4	vSpc;
	FbxVector4	vEmv;
	float	fPower;
	float	fTransparencyFactor;
	string	strDiffuseTex;
	string	strSpecularTex;
	string	strNormalMapTex;
}FBXMATERIALDATA, *PFBXMATERIALDATA;

typedef struct _tagFbxKeyFrame
{
	FbxAMatrix		matTransform;
	double			dTime;
}FBXKEYFRAME, *PFBXKEYFRAME;

// Bone
typedef struct _tagFbxBone
{
	string		strName;
	int			iParentIndex;
	int			iDepth;
	FbxAMatrix	matTransform;
	FbxAMatrix	matOffset;
	vector<FBXKEYFRAME>	vecKeyFrame;
}FBXBONE, *PFBXBONE;

typedef struct _tagFbxAnimationClip
{
	string		strName;
	FbxTime		startTime;
	FbxTime		endTime;
	FbxLongLong	lTimeLength;
}FBXANIMATIONCLIP, *PFBXANIMATIONCLIP;

typedef struct _tagFbxMeshData
{
	UINT			iPolygonCount;
	bool			bAnimation;
	bool			bBump;
	vector<FbxVector4>	vecPos;
	vector<FbxVector4>	vecNormal;
	vector<FbxVector2>	vecUV;
	vector<FbxVector4>	vecTangent;
	vector<FbxVector4>	vecBinormal;
	vector<FbxVector4>	vecBlendWeights;
	vector<FbxVector4>	vecBlendIndices;
	vector<vector<UINT>>	vecIndex;
	vector<PFBXMATERIALDATA>	vecMaterial;
	PFBXANIMATIONCLIP	pAnimationClip;
	unordered_map<int, vector<FBXWEIGHTINDICES>>	mapWI;
}FBXMESHDATA, *PFBXMESHDATA;

class DX11_DLL CDx11FbxLoader
{
private:
	friend class CDx11Mesh;

public:
	CDx11FbxLoader();
	~CDx11FbxLoader();

private:
	FbxManager*		m_pManager;
	FbxScene*		m_pScene;
	vector<PFBXMESHDATA>		m_vecMeshData;
	vector<FBXBONE>				m_vecBone;
	vector<PFBXANIMATIONCLIP>	m_vecAnimationClip;
	FbxArray<FbxString*>		m_AnimNameArray;
	UINT						m_iMeshCount;

public:
	vector<PFBXMESHDATA>& GetMeshContainer();
	vector<FBXBONE>& GetBones();
	vector<PFBXANIMATIONCLIP>& GetAnimationClip();
	bool LoadMesh(char* pFileName);

private:
	void LoadAnimationLerp(FbxNode* pNode);
	void Triangulate(FbxNode* pNode);
	bool LoadNodeData(FbxNode* pNode);
	bool LoadMeshData(FbxMesh* pMesh);
	void LoadNormal(FbxMesh* pMesh, PFBXMESHDATA pMeshData, UINT iVertexID,
		UINT iControlIndex);
	void LoadUV(FbxMesh* pMesh, PFBXMESHDATA pMeshData, UINT iControlIndex,
		UINT iPolygonIdx, UINT iPolygonVertexIdx);
	void LoadTangent(FbxMesh* pMesh, PFBXMESHDATA pMeshData, UINT iVertexID,
		UINT iControlIndex);
	void LoadBinormal(FbxMesh* pMesh, PFBXMESHDATA pMeshData, UINT iVertexID,
		UINT iControlIndex);
	bool LoadMaterialData(FbxSurfaceMaterial* pMaterial);
	FbxDouble3 GetMaterialProperty(FbxSurfaceMaterial* pMaterial,
		PFBXMATERIALDATA pMtrlData, const char* pPropertyName,
		const char* pPropertyFactorName);
	void ConvertFbxDouble3ToVec4(const FbxDouble3 & vD, FbxVector4 & vResult);
	void LoadAnimationData(FbxMesh* pMesh, PFBXMESHDATA pMeshData);
	bool LoadAnimationClipData();
	void LoadSkeleton(FbxNode* pNode);
	void LoadSkeleton(FbxNode* pNode, int iDepth, int iIndex, int iParentIndex);
	bool LoadSkinningData(FbxMesh* pMesh, PFBXMESHDATA pMeshData);
	int FindBoneFromName(const string& strName);
	FbxAMatrix GetTransform(FbxNode* pNode);
	void LoadWeightIndicesData(FbxCluster* pCluster, int iBoneIndex, PFBXMESHDATA pMeshData);
	void LoadOffsetMatrixData(FbxCluster* pCluster, const FbxAMatrix& matGeometryTransform,
		int iBoneIndex, PFBXMESHDATA pMeshData);
	void LoadTimeTransform(FbxNode* pNode, FbxCluster* pCluster, const FbxAMatrix& matGeometryTransform,
		int iBoneIndex, PFBXMESHDATA pMeshData);
	void ChangeVertexWeightAndIndices(FbxMesh* pMesh, PFBXMESHDATA pMeshData);
};

DX11_END