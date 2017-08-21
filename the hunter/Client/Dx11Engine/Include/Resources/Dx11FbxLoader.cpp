#include "Dx11FbxLoader.h"

DX11_USING

#define	FBXLIB_SAFE_DELETE(p)	if(p)	{delete p; p = NULL;}

template <typename T>
void FbxLib_Safe_Delete_VecList(T& p)
{
	for (size_t i = 0; i < p.size(); ++i)
	{
		FBXLIB_SAFE_DELETE(p[i]);
	}

	p.clear();
}

CDx11FbxLoader::CDx11FbxLoader() :
	m_pManager(NULL),
	m_pScene(NULL),
	m_iMeshCount(0)
{
}

CDx11FbxLoader::~CDx11FbxLoader()
{
	for (size_t i = 0; i < m_vecMeshData.size(); ++i)
	{
		SAFE_DELETE(m_vecMeshData[i]->pAnimationClip);
		Safe_Delete_VecList(m_vecMeshData[i]->vecMaterial);
	}

	for (int i = 0; i < m_AnimNameArray.Size(); ++i)
	{
		m_AnimNameArray[i]->Clear();
	}
	m_AnimNameArray.Clear();

	Safe_Delete_VecList(m_vecMeshData);
	Safe_Delete_VecList(m_vecAnimationClip);
	m_pScene->Destroy();

	m_pManager->Destroy();
}

vector<PFBXMESHDATA>& CDx11FbxLoader::GetMeshContainer()
{
	return m_vecMeshData;
}

vector<FBXBONE>& CDx11FbxLoader::GetBones()
{
	return m_vecBone;
}

vector<PFBXANIMATIONCLIP>& CDx11FbxLoader::GetAnimationClip()
{
	return m_vecAnimationClip;
}

bool CDx11FbxLoader::LoadMesh(char * pFileName)
{
	m_iMeshCount = 0;

	// Fbx Manager를 생성한다.
	m_pManager = FbxManager::Create();

	FbxIOSettings*	pIOSet = FbxIOSettings::Create(m_pManager, IOSROOT);
	m_pManager->SetIOSettings(pIOSet);

	// Fbx 파일을 읽어오기 위한 Importer를 만들어준다.
	FbxImporter*	pImporter = FbxImporter::Create(m_pManager, "");

	// 이 파일이 읽어올 수 있는 파일인지를 검사한다.
	if (!pImporter->Initialize(pFileName, -1, m_pManager->GetIOSettings()))
		return false;

	// Scene을 만들어준다.
	m_pScene = FbxScene::Create(m_pManager, "FbxScene");

	// Scene에 Fbx파일에 있는 정보들을 읽어와서 구성해준다.
	pImporter->Import(m_pScene);

	//FbxAxisSystem OurAxisSystem(FbxAxisSystem::eYAxis, FbxAxisSystem::eParityOdd, FbxAxisSystem::eLeftHanded);
	FbxAxisSystem OurAxisSystem = FbxAxisSystem::Max;

	FbxAxisSystem SceneAxisSystem = m_pScene->GetGlobalSettings().GetAxisSystem();

	if (SceneAxisSystem != OurAxisSystem)
	{
		OurAxisSystem.ConvertScene(m_pScene);
	}

	// 메쉬를 삼각형 집합으로 만든다.
	Triangulate(m_pScene->GetRootNode());

	// 애니메이션 이름을 얻어온다.
	m_pScene->FillAnimStackNameArray(m_AnimNameArray);

	// 본정보를 읽어온다.
	LoadSkeleton(m_pScene->GetRootNode());

	// 애니메이션 본 보간정보
	LoadAnimationLerp(m_pScene->GetRootNode());

	// 애니메이션 클립 정보를 읽어온다.
	LoadAnimationClipData();

	// 노드의 데이터를 얻어온다.
	if (!LoadNodeData(m_pScene->GetRootNode()))
		return false;

	pImporter->Destroy();

	return true;
}

void CDx11FbxLoader::LoadAnimationLerp(FbxNode * pNode)
{
	FbxGeometry*	pGeometry = pNode->GetGeometry();

	if (pGeometry)
	{
		//FbxSkin*	pSkin = (FbxSkin*)pGeometry->GetDeformer(0, FbxDeformer::eSkin);

		//FbxSkin::EType	eSkinningType = pSkin->GetSkinningType();

		//if (eSkinningType != FbxSkin::eRigid && eSkinningType != FbxSkin::eLinear)
		//	return;

		/*int	iSkinCount = pMesh->GetDeformerCount(FbxDeformer::eSkin);

		for (int i = 0; i < iSkinCount; ++i)
		{
		pSkin = (FbxSkin*)pMesh->GetDeformer(i, FbxDeformer::eSkin);

		int	iClusterCount = pSkin->GetClusterCount();

		for (int j = 0; j < iClusterCount; ++j)
		{
		FbxCluster*	pCluster = pSkin->GetCluster(j);

		if (!pCluster->GetLink())
		continue;

		string	strBoneName = pCluster->GetLink()->GetName();
		int	iBoneIndex = FindBoneFromName(strBoneName);
		FbxAMatrix	matGeometryTransform = GetTransform(pMesh->GetNode());

		// 가중치와 인덱스 정보를 얻어온다.
		LoadWeightIndicesData(pCluster, iBoneIndex, pMeshData);
		// Offset 행렬을 얻어온다.
		LoadOffsetMatrixData(pCluster, matGeometryTransform, iBoneIndex, pMeshData);
		LoadTimeTransform(pMesh->GetNode(), pCluster, matGeometryTransform, iBoneIndex,
		pMeshData);
		}
		}*/
	}

	int	iChildCount = pNode->GetChildCount();
	for (int i = 0; i < iChildCount; ++i)
	{
		LoadAnimationLerp(pNode->GetChild(i));
	}
}

void CDx11FbxLoader::Triangulate(FbxNode * pNode)
{
	FbxNodeAttribute*	pAttr = pNode->GetNodeAttribute();

	if (pAttr)
	{
		FbxNodeAttribute::EType	eType = pAttr->GetAttributeType();
		if (eType == FbxNodeAttribute::eMesh ||
			eType == FbxNodeAttribute::eNurbs ||
			eType == FbxNodeAttribute::eNurbsSurface ||
			eType == FbxNodeAttribute::ePatch)
		{
			FbxGeometryConverter	converter(m_pManager);
			converter.Triangulate(pAttr, true);
		}
	}

	int	iChildCount = pNode->GetChildCount();
	for (int i = 0; i < iChildCount; ++i)
	{
		Triangulate(pNode->GetChild(i));
	}
}

bool CDx11FbxLoader::LoadNodeData(FbxNode * pNode)
{
	FbxNodeAttribute*	pAttr = pNode->GetNodeAttribute();

	if (pAttr)
	{
		if (pAttr->GetAttributeType() == FbxNodeAttribute::eMesh)
		{
			FbxMesh*	pMesh = pNode->GetMesh();

			if (pMesh)
			{
				if (!LoadMeshData(pMesh))
					return false;

			}
		}
	}

	int	iMaterialCount = pNode->GetMaterialCount();
	for (int i = 0; i < iMaterialCount; ++i)
	{
		FbxSurfaceMaterial*	pMaterial = pNode->GetMaterial(i);

		if (!pMaterial)
			continue;

		LoadMaterialData(pMaterial);
	}

	int	iChildCount = pNode->GetChildCount();
	for (int i = 0; i < iChildCount; ++i)
	{
		if (!LoadNodeData(pNode->GetChild(i)))
			return false;
	}

	return true;
}

bool CDx11FbxLoader::LoadMeshData(FbxMesh * pMesh)
{
	if (!pMesh->GetNode())
		return false;

	PFBXMESHDATA	pMeshData = new FBXMESHDATA;

	pMeshData->pAnimationClip = NULL;
	pMeshData->bAnimation = false;
	pMeshData->bBump = false;

	m_vecMeshData.push_back(pMeshData);

	int	iPolygonCount = pMesh->GetPolygonCount();
	FbxVector4*	pControlPoints = pMesh->GetControlPoints();

	// 정점 개수를 얻어온다.
	int	iVertexCount = pMesh->GetControlPointsCount();

	pMeshData->vecPos.reserve(iVertexCount);
	pMeshData->vecNormal.reserve(iVertexCount);
	pMeshData->vecUV.reserve(iVertexCount);
	pMeshData->vecTangent.reserve(iVertexCount);
	pMeshData->vecBinormal.reserve(iVertexCount);
	pMeshData->vecBlendWeights.reserve(iVertexCount);
	pMeshData->vecBlendIndices.reserve(iVertexCount);

	FbxAxisSystem SceneAxisSystem = m_pScene->GetGlobalSettings().GetAxisSystem();

	pMeshData->vecNormal.resize(iVertexCount);
	pMeshData->vecUV.resize(iVertexCount);
	pMeshData->vecTangent.resize(iVertexCount);
	pMeshData->vecBinormal.resize(iVertexCount);
	pMeshData->vecBlendWeights.resize(iVertexCount);
	pMeshData->vecBlendIndices.resize(iVertexCount);

	for (int i = 0; i < iVertexCount; ++i)
	{
		FbxVector4	vPos;
		vPos.mData[0] = pControlPoints[i].mData[0];
		vPos.mData[1] = pControlPoints[i].mData[2];
		vPos.mData[2] = pControlPoints[i].mData[1];
		pMeshData->vecPos.push_back(vPos);
	}

	UINT	iVertexID = 0;

	pMeshData->iPolygonCount = iPolygonCount;

	// 재질 개수만큼 인덱스 버퍼를 할당해둔다.
	int	iMaterialCount = pMesh->GetNode()->GetMaterialCount();
	for (int i = 0; i < iMaterialCount; ++i)
	{
		vector<UINT>	vec;
		pMeshData->vecIndex.push_back(vec);
	}

	// Element Material Count를 얻어온다.
	int	iMaterialElementCount = pMesh->GetElementMaterialCount();

	for (int i = 0; i < iPolygonCount; ++i)
	{
		int	iPolygonSize = pMesh->GetPolygonSize(i);

		int	iMaterialID = -1;

		int	iIndex[3] = {};

		// 현재 폴리곤의 재질 ID를 얻어온다.
		FbxGeometryElementMaterial*	pMaterial = pMesh->GetElementMaterial(0);

		iMaterialID = pMaterial->GetIndexArray().GetAt(i);

		for (int j = 0; j < iPolygonSize; ++j)
		{
			// 정점의 위치정보를 읽어온다.
			int	iControlIndex = pMesh->GetPolygonVertex(i, j);

			iIndex[j] = iControlIndex;

			// Normal 정보를 읽어온다.
			LoadNormal(pMesh, pMeshData, iVertexID, iControlIndex);

			// UV 정보를 읽어온다.
			LoadUV(pMesh, pMeshData, iControlIndex, i, j);

			// Tangent 정보를 읽어온다.
			LoadTangent(pMesh, pMeshData, iVertexID, iControlIndex);

			// Binormal 정보를 읽어온다.
			LoadBinormal(pMesh, pMeshData, iVertexID, iControlIndex);

			++iVertexID;
		}// PolygonSize

		pMeshData->vecIndex[iMaterialID].push_back(iIndex[0]);
		pMeshData->vecIndex[iMaterialID].push_back(iIndex[2]);
		pMeshData->vecIndex[iMaterialID].push_back(iIndex[1]);
	}// PolygonCount

	 // 애니메이션 정보를 얻어온다.
	LoadAnimationData(pMesh, pMeshData);

	++m_iMeshCount;

	return true;
}

void CDx11FbxLoader::LoadNormal(FbxMesh * pMesh, PFBXMESHDATA pMeshData, UINT iVertexID,
	UINT iControlIndex)
{
	int	iNormalCount = pMesh->GetElementNormalCount();

	for (int i = 0; i < iNormalCount; ++i)
	{
		FbxGeometryElementNormal*	pNormal = pMesh->GetElementNormal(i);

		if (pNormal->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
		{
			FbxVector4	vFbxNormal;
			FbxVector4	vNormal;
			int			iNormalIndex = 0;
			switch (pNormal->GetReferenceMode())
			{
			case FbxGeometryElement::eDirect:
				vFbxNormal = pNormal->GetDirectArray().GetAt(iVertexID);
				vNormal.mData[0] = vFbxNormal.mData[0];
				vNormal.mData[1] = vFbxNormal.mData[2];
				vNormal.mData[2] = vFbxNormal.mData[1];
				pMeshData->vecNormal[iControlIndex] = vNormal;
				//pMeshData->vecNormal.push_back(vNormal);
				break;
			case FbxGeometryElement::eIndexToDirect:
				iNormalIndex = pNormal->GetIndexArray().GetAt(iVertexID);
				vFbxNormal = pNormal->GetDirectArray().GetAt(iNormalIndex);
				vNormal.mData[0] = vFbxNormal.mData[0];
				vNormal.mData[1] = vFbxNormal.mData[2];
				vNormal.mData[2] = vFbxNormal.mData[1];
				pMeshData->vecNormal[iControlIndex] = vNormal;
				//pMeshData->vecNormal.push_back(vNormal);
				break;
			}
		}
	}
}

void CDx11FbxLoader::LoadUV(FbxMesh * pMesh, PFBXMESHDATA pMeshData, UINT iControlIndex,
	UINT iPolygonIdx, UINT iPolygonVertexIdx)
{
	int	iUVCount = pMesh->GetElementUVCount();

	for (int i = 0; i < iUVCount; ++i)
	{
		FbxGeometryElementUV*	pUV = pMesh->GetElementUV(i);

		FbxVector2	vFbxUV;
		FbxVector2	vUV;
		int			iTextureIndex = 0;

		switch (pUV->GetMappingMode())
		{
		case FbxGeometryElement::eByControlPoint:
			switch (pUV->GetReferenceMode())
			{
			case FbxGeometryElement::eDirect:
				vFbxUV = pUV->GetDirectArray().GetAt(iControlIndex);
				vUV.mData[0] = vFbxUV.mData[0];
				vUV.mData[1] = 1.f - vFbxUV.mData[1];
				pMeshData->vecUV[iControlIndex] = vUV;
				//pMeshData->vecUV.push_back(vUV);
				break;
			case FbxGeometryElement::eIndexToDirect:
				iTextureIndex = pUV->GetIndexArray().GetAt(iControlIndex);
				vFbxUV = pUV->GetDirectArray().GetAt(iTextureIndex);
				vUV.mData[0] = vFbxUV.mData[0];
				vUV.mData[1] = 1.f - vFbxUV.mData[1];
				pMeshData->vecUV[iControlIndex] = vUV;
				//pMeshData->vecUV.push_back(vUV);
				break;
			}
			break;
		case FbxGeometryElement::eByPolygonVertex:
			iTextureIndex = pMesh->GetTextureUVIndex(iPolygonIdx, iPolygonVertexIdx);

			switch (pUV->GetReferenceMode())
			{
			case FbxGeometryElement::eDirect:
			case FbxGeometryElement::eIndexToDirect:
				vFbxUV = pUV->GetDirectArray().GetAt(iTextureIndex);
				vUV.mData[0] = vFbxUV.mData[0];
				vUV.mData[1] = 1.f - vFbxUV.mData[1];
				pMeshData->vecUV[iControlIndex] = vUV;
				//pMeshData->vecUV.push_back(vUV);
				break;
			}
			break;
		}
	}
}

void CDx11FbxLoader::LoadTangent(FbxMesh * pMesh, PFBXMESHDATA pMeshData, UINT iVertexID,
	UINT iControlIndex)
{
	int	iTangentCount = pMesh->GetElementTangentCount();

	for (int i = 0; i < iTangentCount; ++i)
	{
		FbxGeometryElementTangent*	pTangent = pMesh->GetElementTangent(i);

		if (pTangent->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
		{
			FbxVector4	vFbxTangent;
			FbxVector4	vTangent;
			int			iTangentIndex = 0;
			switch (pTangent->GetReferenceMode())
			{
			case FbxGeometryElement::eDirect:
				vFbxTangent = pTangent->GetDirectArray().GetAt(iVertexID);
				vTangent.mData[0] = vFbxTangent.mData[0];
				vTangent.mData[1] = vFbxTangent.mData[2];
				vTangent.mData[2] = vFbxTangent.mData[1];
				pMeshData->bBump = true;
				pMeshData->vecTangent[iControlIndex] = vTangent;
				//pMeshData->vecTangent.push_back(vTangent);
				break;
			case FbxGeometryElement::eIndexToDirect:
				iTangentIndex = pTangent->GetIndexArray().GetAt(iVertexID);
				vFbxTangent = pTangent->GetDirectArray().GetAt(iTangentIndex);
				vTangent.mData[0] = vFbxTangent.mData[0];
				vTangent.mData[1] = vFbxTangent.mData[2];
				vTangent.mData[2] = vFbxTangent.mData[1];
				pMeshData->bBump = true;
				pMeshData->vecTangent[iControlIndex] = vTangent;
				//pMeshData->vecTangent.push_back(vTangent);
				break;
			}
		}
	}
}

void CDx11FbxLoader::LoadBinormal(FbxMesh * pMesh, PFBXMESHDATA pMeshData, UINT iVertexID,
	UINT iControlIndex)
{
	int	iBinormalCount = pMesh->GetElementBinormalCount();

	for (int i = 0; i < iBinormalCount; ++i)
	{
		FbxGeometryElementBinormal*	pBinormal = pMesh->GetElementBinormal(i);

		if (pBinormal->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
		{
			FbxVector4	vFbxBinormal;
			FbxVector4		vBinormal;
			int			iBinormalIndex = 0;

			switch (pBinormal->GetReferenceMode())
			{
			case FbxGeometryElement::eDirect:
				vFbxBinormal = pBinormal->GetDirectArray().GetAt(iVertexID);
				vBinormal.mData[0] = vFbxBinormal.mData[0];
				vBinormal.mData[1] = vFbxBinormal.mData[2];
				vBinormal.mData[2] = vFbxBinormal.mData[1];
				pMeshData->vecBinormal[iControlIndex] = vBinormal;
				//pMeshData->vecBinormal.push_back(vBinormal);
				break;
			case FbxGeometryElement::eIndexToDirect:
				iBinormalIndex = pBinormal->GetIndexArray().GetAt(iVertexID);
				vFbxBinormal = pBinormal->GetDirectArray().GetAt(iBinormalIndex);
				vBinormal.mData[0] = vFbxBinormal.mData[0];
				vBinormal.mData[1] = vFbxBinormal.mData[2];
				vBinormal.mData[2] = vFbxBinormal.mData[1];
				pMeshData->vecBinormal[iControlIndex] = vBinormal;
				//pMeshData->vecBinormal.push_back(vBinormal);
				break;
			}
		}
	}
}

bool CDx11FbxLoader::LoadMaterialData(FbxSurfaceMaterial * pMaterial)
{
	if (!pMaterial)
		return false;

	PFBXMATERIALDATA	pMtrlData = new FBXMATERIALDATA;

	PFBXMESHDATA	pContainer = m_vecMeshData.back();
	pContainer->vecMaterial.push_back(pMtrlData);

	// 재질의 종류를 판단한다.
	if (pMaterial->GetClassId().Is(FbxSurfaceLambert::ClassId))
		pMtrlData->eNode = FMN_LAMBERT;

	else if (pMaterial->GetClassId().Is(FbxSurfacePhong::ClassId))
		pMtrlData->eNode = FMN_PHONG;

	// Diffuse를 읽어온다. 이때 Diffuse Texture가 같이 읽어진다.
	FbxDouble3	vDiffuse = GetMaterialProperty(pMaterial, pMtrlData,
		FbxSurfaceMaterial::sDiffuse, FbxSurfaceMaterial::sDiffuseFactor);
	ConvertFbxDouble3ToVec4(vDiffuse, pMtrlData->vDif);

	// Ambient를 읽어온다.
	FbxDouble3	vAmbient = GetMaterialProperty(pMaterial, pMtrlData,
		FbxSurfaceMaterial::sAmbient, FbxSurfaceMaterial::sAmbientFactor);
	ConvertFbxDouble3ToVec4(vAmbient, pMtrlData->vAmb);

	// Specular를 읽어온다.
	FbxDouble3	vSpecular = GetMaterialProperty(pMaterial, pMtrlData,
		FbxSurfaceMaterial::sSpecular, FbxSurfaceMaterial::sSpecularFactor);
	ConvertFbxDouble3ToVec4(vSpecular, pMtrlData->vSpc);

	// Emissive를 읽어온다.
	FbxDouble3	vEmissive = GetMaterialProperty(pMaterial, pMtrlData,
		FbxSurfaceMaterial::sEmissive, FbxSurfaceMaterial::sEmissiveFactor);
	ConvertFbxDouble3ToVec4(vEmissive, pMtrlData->vEmv);

	// Power를 읽어온다.
	FbxProperty	fbxPower = pMaterial->FindProperty(FbxSurfaceMaterial::sShininess);
	if (fbxPower.IsValid())
	{
		double dPower = fbxPower.Get<FbxDouble>();
		pMtrlData->fPower = (float)dPower;
	}

	FbxProperty	fbxTransparency = pMaterial->FindProperty(FbxSurfaceMaterial::sTransparencyFactor);

	if (fbxTransparency.IsValid())
	{
		double dTransparencyFactor = fbxTransparency.Get<FbxDouble>();
		pMtrlData->fTransparencyFactor = (float)dTransparencyFactor;
	}

	// Bump Texture 이름을 읽어온다.
	GetMaterialProperty(pMaterial, pMtrlData, FbxSurfaceMaterial::sBump,
		FbxSurfaceMaterial::sBumpFactor);

	// NormalMap Texture
	GetMaterialProperty(pMaterial, pMtrlData, FbxSurfaceMaterial::sNormalMap,
		FbxSurfaceMaterial::sBumpFactor);

	return true;
}

FbxDouble3 CDx11FbxLoader::GetMaterialProperty(FbxSurfaceMaterial * pMaterial,
	PFBXMATERIALDATA pMtrlData, const char * pPropertyName,
	const char * pPropertyFactorName)
{
	pMtrlData->eType = FMT_NONE;

	FbxDouble3	vResult(0, 0, 0);
	// Material Property를 얻어온다.
	FbxProperty	fbxProperty = pMaterial->FindProperty(pPropertyName);
	// Material Property Factor를 얻어온다.
	FbxProperty	fbxPropertyFactor = pMaterial->FindProperty(pPropertyFactorName);

	// 얻어온 속성과 속성Factor가  유효한지 체크한다.
	if (fbxProperty.IsValid() && fbxPropertyFactor.IsValid())
	{
		// 현재 속성의 값을 얻어온다.
		vResult = fbxProperty.Get<FbxDouble3>();
		double	dFactor = fbxPropertyFactor.Get<FbxDouble>();

		// Factor가 1이 아닐 경우 Factor를 각각의 값에 곱해준다.
		if (dFactor != 1)
		{
			vResult[0] *= dFactor;
			vResult[1] *= dFactor;
			vResult[2] *= dFactor;
		}

		pMtrlData->eType = FMT_COLOR;
	}

	// 속성이 유효한지 체크한다.
	if (fbxProperty.IsValid())
	{
		// 존재하는 텍스쳐 수를 설정한다.
		int	iExistTextureCount = 0;
		// GetSrcObjectCount는 Template에 지정된 타입의 객체 수를 리턴해준다.
		const int	iTextureCount = fbxProperty.GetSrcObjectCount<FbxFileTexture>();

		for (int i = 0; i < iTextureCount; ++i)
		{
			// FbxFileTexture를 얻어온다.
			FbxFileTexture*	pFileTexture = fbxProperty.GetSrcObject<FbxFileTexture>(i);

			if (!pFileTexture)
				continue;

			FbxString	strUvSetName = pFileTexture->UVSet.Get();
			string	struvSetString = strUvSetName.Buffer();
			string	strPath = pFileTexture->GetFileName();

			if (strcmp(pPropertyName, FbxSurfaceMaterial::sDiffuse) == 0)
				pMtrlData->strDiffuseTex = strPath;

			else if (strcmp(pPropertyName, FbxSurfaceMaterial::sSpecular) == 0)
				pMtrlData->strSpecularTex = strPath;

			else if (strcmp(pPropertyName, FbxSurfaceMaterial::sNormalMap) == 0)
				pMtrlData->strNormalMapTex = strPath;

			//pMtrlData->strTextureName = strPath;
			//pMtrlData->mapTextureName[struvSetString].push_back(strPath);
			//pMtrlData->vecUvSet.push_back(struvSetString);

			++iExistTextureCount;
		}// Texture Count

		const int	iLayeredTextureCount = fbxProperty.GetSrcObjectCount<FbxLayeredTexture>();

		for (int i = 0; i < iLayeredTextureCount; ++i)
		{
			FbxLayeredTexture*	pLayeredTexture = fbxProperty.GetSrcObject<FbxLayeredTexture>(i);

			const int	iTextureFileCount = pLayeredTexture->GetSrcObjectCount<FbxFileTexture>();

			for (int j = 0; j < iTextureFileCount; ++j)
			{
				FbxFileTexture*	pFileTexture = pLayeredTexture->GetSrcObject<FbxFileTexture>(j);

				if (!pFileTexture)
					continue;

				FbxString	strUvSetName = pFileTexture->UVSet.Get();
				string	struvSetString = strUvSetName.Buffer();
				string	strPath = pFileTexture->GetFileName();

				if (strcmp(pPropertyName, FbxSurfaceMaterial::sDiffuse) == 0)
					pMtrlData->strDiffuseTex = strPath;

				else if (strcmp(pPropertyName, FbxSurfaceMaterial::sSpecular) == 0)
					pMtrlData->strSpecularTex = strPath;

				else if (strcmp(pPropertyName, FbxSurfaceMaterial::sNormalMap) == 0)
					pMtrlData->strNormalMapTex = strPath;

				++iExistTextureCount;
			}
		}// LayeredTexture

		if (iExistTextureCount > 0)
		{
			if (pMtrlData->eType == FMT_COLOR)
				pMtrlData->eType = FMT_BOTH;

			else
				pMtrlData->eType = FMT_TEXTURE;
		}
	}

	return vResult;
}

void CDx11FbxLoader::ConvertFbxDouble3ToVec4(const FbxDouble3 & vD, FbxVector4 & vResult)
{
	vResult.mData[0] = vD[0];
	vResult.mData[1] = vD[1];
	vResult.mData[2] = vD[2];
}

void CDx11FbxLoader::LoadAnimationData(FbxMesh * pMesh, PFBXMESHDATA pMeshData)
{
	bool	bVertexCache = pMesh->GetDeformerCount(FbxDeformer::eVertexCache) &&
		((FbxVertexCacheDeformer*)pMesh->GetDeformer(0, FbxDeformer::eVertexCache))->Active.Get();
	bool	bShape = pMesh->GetShapeCount() > 0;
	bool	bSkin = pMesh->GetDeformerCount(FbxDeformer::eSkin) > 0;
	bool	bDeformer = bSkin;

	pMeshData->bAnimation = bDeformer;

	if (bDeformer)
	{
		//if (!LoadAnimationClipData())
		//	return;

		// Skin 정보를 얻어온다.
		if (!LoadSkinningData(pMesh, pMeshData))
			return;
	}
}

bool CDx11FbxLoader::LoadAnimationClipData()
{
	int iStackCount = m_AnimNameArray.GetCount();
	for (int i = 0; i < iStackCount; ++i)
	{
		// 애니메이션 이름을 이용해서 Stack을 얻어온다.
		FbxAnimStack*	pAnimStack = m_pScene->FindMember<FbxAnimStack>(m_AnimNameArray[i]->Buffer());

		if (!pAnimStack)
			return false;

		int iLayerCount = pAnimStack->GetMemberCount<FbxAnimLayer>();

		for (int j = 0; j < iLayerCount; ++j)
		{
			FbxAnimLayer*	pAnimLayer = pAnimStack->GetMember<FbxAnimLayer>(j);

			int	iClusterCount = pAnimLayer->GetSrcObjectCount<FbxCluster>();
			int iSkinCount = pAnimLayer->GetSrcObjectCount<FbxSkin>();

			int	iClusterCount1 = pAnimLayer->GetDstObjectCount<FbxCluster>();
			int iSkinCount1 = pAnimLayer->GetDstObjectCount<FbxSkin>();
			int a = 10;
		}

		PFBXANIMATIONCLIP	pClip = new FBXANIMATIONCLIP;

		//m_vecMeshData[m_iMeshCount]->pAnimationClip = pClip;
		m_vecAnimationClip.push_back(pClip);

		FbxString	strAnimStackName = pAnimStack->GetName();

		pClip->strName = strAnimStackName.Buffer();

		FbxTakeInfo*	pInfo = m_pScene->GetTakeInfo(strAnimStackName);

		FbxTime	startTime = pInfo->mLocalTimeSpan.GetStart();
		FbxTime	endTime = pInfo->mLocalTimeSpan.GetStop();
		FbxTime::EMode	eTimeMode = m_pScene->GetGlobalSettings().GetTimeMode();

		pClip->startTime = startTime;
		pClip->endTime = endTime;
		pClip->lTimeLength = endTime.GetFrameCount(eTimeMode) -
			startTime.GetFrameCount(eTimeMode);
	}

	return true;
}

void CDx11FbxLoader::LoadSkeleton(FbxNode * pNode)
{
	for (int i = 0; i < pNode->GetChildCount(); ++i)
	{
		LoadSkeleton(pNode->GetChild(i), 0, 0, -1);
	}
}

void CDx11FbxLoader::LoadSkeleton(FbxNode * pNode, int iDepth, int iIndex,
	int iParentIndex)
{
	FbxNodeAttribute*	pAttr = pNode->GetNodeAttribute();

	if (pAttr && pAttr->GetAttributeType() == FbxNodeAttribute::eSkeleton)
	{
		FBXBONE	tBone = {};

		tBone.strName = pNode->GetName();
		tBone.iDepth = iDepth;
		tBone.iParentIndex = iParentIndex;

		m_vecBone.push_back(tBone);
	}

	for (int i = 0; i < pNode->GetChildCount(); ++i)
	{
		LoadSkeleton(pNode->GetChild(i), iDepth + 1, m_vecBone.size(), iIndex);
	}
}

bool CDx11FbxLoader::LoadSkinningData(FbxMesh * pMesh, PFBXMESHDATA pMeshData)
{
	FbxSkin*	pSkin = (FbxSkin*)pMesh->GetDeformer(0, FbxDeformer::eSkin);
	FbxSkin::EType	eSkinningType = pSkin->GetSkinningType();

	if (eSkinningType != FbxSkin::eRigid && eSkinningType != FbxSkin::eLinear)
		return false;

	int	iSkinCount = pMesh->GetDeformerCount(FbxDeformer::eSkin);

	for (int i = 0; i < iSkinCount; ++i)
	{
		pSkin = (FbxSkin*)pMesh->GetDeformer(i, FbxDeformer::eSkin);

		int	iClusterCount = pSkin->GetClusterCount();

		for (int j = 0; j < iClusterCount; ++j)
		{
			FbxCluster*	pCluster = pSkin->GetCluster(j);

			if (!pCluster->GetLink())
				continue;

			string	strBoneName = pCluster->GetLink()->GetName();
			int	iBoneIndex = FindBoneFromName(strBoneName);
			FbxAMatrix	matGeometryTransform = GetTransform(pMesh->GetNode());

			// 가중치와 인덱스 정보를 얻어온다.
			LoadWeightIndicesData(pCluster, iBoneIndex, pMeshData);
			// Offset 행렬을 얻어온다.
			LoadOffsetMatrixData(pCluster, matGeometryTransform, iBoneIndex, pMeshData);
			LoadTimeTransform(pMesh->GetNode(), pCluster, matGeometryTransform, iBoneIndex,
				pMeshData);
		}
	}

	ChangeVertexWeightAndIndices(pMesh, pMeshData);

	return true;
}

int CDx11FbxLoader::FindBoneFromName(const string & strName)
{
	for (size_t i = 0; i < m_vecBone.size(); ++i)
	{
		if (m_vecBone[i].strName == strName)
			return i;
	}

	return -2;
}

FbxAMatrix CDx11FbxLoader::GetTransform(FbxNode * pNode)
{
	const FbxVector4	vT = pNode->GetGeometricTranslation(FbxNode::eSourcePivot);
	const FbxVector4	vR = pNode->GetGeometricRotation(FbxNode::eSourcePivot);
	const FbxVector4	vS = pNode->GetGeometricScaling(FbxNode::eSourcePivot);

	return FbxAMatrix(vT, vR, vS);
}

void CDx11FbxLoader::LoadWeightIndicesData(FbxCluster * pCluster, int iBoneIndex,
	PFBXMESHDATA pMeshData)
{
	int	iWeightCount = pCluster->GetControlPointIndicesCount();
	int	iClusterIndex = 0;

	for (int i = 0; i < iWeightCount; ++i)
	{
		FBXWEIGHTINDICES	tWI = {};

		tWI.iBoneIndex = iBoneIndex;
		tWI.dWeight = pCluster->GetControlPointWeights()[i];

		iClusterIndex = pCluster->GetControlPointIndices()[i];

		pMeshData->mapWI[iClusterIndex].push_back(tWI);
	}
}

void CDx11FbxLoader::LoadOffsetMatrixData(FbxCluster * pCluster,
	const FbxAMatrix & matGeometryTransform, int iBoneIndex, PFBXMESHDATA pMeshData)
{
	FbxAMatrix	matTransform;
	FbxAMatrix	matTransformLink;

	pCluster->GetTransformMatrix(matTransform);
	pCluster->GetTransformLinkMatrix(matTransformLink);

	FbxVector4	v1 = { 1, 0, 0, 0 };
	FbxVector4	v2 = { 0, 0, 1, 0 };
	FbxVector4	v3 = { 0, 1, 0, 0 };
	FbxVector4	v4 = { 0, 0, 0, 1 };
	FbxAMatrix	matReflect;
	matReflect.mData[0] = v1;
	matReflect.mData[1] = v2;
	matReflect.mData[2] = v3;
	matReflect.mData[3] = v4;

	FbxAMatrix	matOffset;

	matOffset = matTransformLink.Inverse() * matTransform * matGeometryTransform;
	matOffset = matReflect * matOffset * matReflect;

	m_vecBone[iBoneIndex].matOffset = matOffset;
}

void CDx11FbxLoader::LoadTimeTransform(FbxNode * pNode, FbxCluster * pCluster,
	const FbxAMatrix & matGeometryTransform, int iBoneIndex, PFBXMESHDATA pMeshData)
{
	FbxTime::EMode	eTimeMode = m_pScene->GetGlobalSettings().GetTimeMode();

	FbxVector4	v1 = { 1, 0, 0, 0 };
	FbxVector4	v2 = { 0, 0, 1, 0 };
	FbxVector4	v3 = { 0, 1, 0, 0 };
	FbxVector4	v4 = { 0, 0, 0, 1 };
	FbxAMatrix	matReflect;
	matReflect.mData[0] = v1;
	matReflect.mData[1] = v2;
	matReflect.mData[2] = v3;
	matReflect.mData[3] = v4;

	FbxLongLong	lStart = m_vecAnimationClip[0]->startTime.GetFrameCount(FbxTime::eFrames30);
	FbxLongLong	lEnd = m_vecAnimationClip[0]->endTime.GetFrameCount(FbxTime::eFrames30);

	for (FbxLongLong i = lStart; i <= lEnd; ++i)
	{
		FBXKEYFRAME		tKeyFrame;
		FbxTime	tCurTime;

		tCurTime.SetFrame(i, eTimeMode);

		FbxAMatrix	matOffset = pNode->EvaluateGlobalTransform(tCurTime) * matGeometryTransform;
		FbxAMatrix	matCurTransform = matOffset.Inverse() * pCluster->GetLink()->EvaluateGlobalTransform(tCurTime);

		matCurTransform = matReflect * matCurTransform * matReflect;

		tKeyFrame.dTime = tCurTime.GetSecondDouble();
		tKeyFrame.matTransform = matCurTransform;

		m_vecBone[iBoneIndex].vecKeyFrame.push_back(tKeyFrame);
	}
}

void CDx11FbxLoader::ChangeVertexWeightAndIndices(FbxMesh * pMesh, PFBXMESHDATA pMeshData)
{
	// 가중치와 인덱스가 저장된 map을 반복한다.
	unordered_map<int, vector<FBXWEIGHTINDICES>>::iterator	iter;
	unordered_map<int, vector<FBXWEIGHTINDICES>>::iterator	iterEnd = pMeshData->mapWI.end();

	const FbxVector4*	pControlPoints = pMesh->GetControlPoints();

	for (iter = pMeshData->mapWI.begin(); iter != iterEnd; ++iter)
	{
		if (iter->second.size() > 4)
		{
			// 가중치 값에 따라서 내림차순으로 정렬한다.
			sort(iter->second.begin(), iter->second.end(), [](const FBXWEIGHTINDICES& lhs, const FBXWEIGHTINDICES& rhs)
			{
				return lhs.dWeight > rhs.dWeight;
			});

			// 가중치의 총합을 구한 후에 가중치의 총합이 1이 될 수 있도록 1에서 총합을 빼준다.
			float	fSumWeight = 0.f, fInterpolateWeight = 0.f;

			for (int i = 0; i < 4; ++i)
			{
				fSumWeight += iter->second[i].dWeight;
			}

			fInterpolateWeight = 1.f - fSumWeight;

			vector<FBXWEIGHTINDICES>::iterator	iterErase = iter->second.begin() + 4;
			iter->second.erase(iterErase, iter->second.end());

			iter->second[0].dWeight += fInterpolateWeight;
		}

		float	fWeight[4] = {};
		int		iIndices[4] = {};

		for (size_t i = 0; i < iter->second.size(); ++i)
		{
			fWeight[i] = iter->second[i].dWeight;
			iIndices[i] = iter->second[i].iBoneIndex;
		}

		FbxVector4	vWeight;
		FbxVector4	vIndices;

		for (int i = 0; i < 4; ++i)
		{
			vWeight.mData[i] = fWeight[i];
			vIndices.mData[i] = iIndices[i];
		}

		pMeshData->vecBlendWeights[iter->first] = vWeight;
		pMeshData->vecBlendIndices[iter->first] = vIndices;
	}
}
