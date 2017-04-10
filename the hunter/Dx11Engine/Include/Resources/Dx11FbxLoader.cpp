#include "Dx11FbxLoader.h"

DX11_USING

CDx11FbxLoader::CDx11FbxLoader()	:
	m_pManager(NULL),
	m_pScene(NULL)
{
}


CDx11FbxLoader::~CDx11FbxLoader()
{
	// 매쉬 데이터의 크기만큼 루프를 돌고,
	// 각각의 매쉬 컨테이너안의 재질 벡터를 Safe_Delete
	for (size_t i = 0; i < m_vecMeshData.size(); ++i)
	{
		Safe_Delete_VecList(m_vecMeshData[i]->vecMaterial);
	}
	Safe_Delete_VecList(m_vecMeshData);
	
	m_pScene->Destroy();
	m_pManager->Destroy();
}

vector<PMESHDATA>& CDx11FbxLoader::GetMeshContainer()
{
	return m_vecMeshData;
}

int CDx11FbxLoader::GetContainerCount()
{
	// 매쉬 컨테이너의 크기
	return m_vecMeshData.size();
}

int CDx11FbxLoader::GetSubsetCount(int iContainer)
{
	// i번째 매쉬 컨테이너의 인덱스 벡터의 크기
	return m_vecMeshData[iContainer]->vecIndex.size();
}

bool CDx11FbxLoader::LoadMesh(char * pFileName)
{
	// Fbx Manager Create
	m_pManager = FbxManager::Create();//1

	if (!m_pManager)
		return false;

	// 입출력 세팅을 만든다.
	FbxIOSettings* ios = FbxIOSettings::Create(m_pManager, IOSROOT);//2.
																	// 매니저에 만들어준 입출력 세팅을 지정한다.
	m_pManager->SetIOSettings(ios);

	FbxString lPath = FbxGetApplicationDirectory();//3.
	m_pManager->LoadPluginsDirectory(lPath.Buffer());

	FbxImporter* pImporter = FbxImporter::Create(m_pManager, "");//4.

	// Importer의 Initialize를 해주면 인자로 들어간 파일명의 fbx를 읽어올 수 있도록
	// 준비시킨다.
	if (!pImporter->Initialize(pFileName, -1, m_pManager->GetIOSettings()))//5.
		return false;

	m_pScene = FbxScene::Create(m_pManager, "FbxScene");

	pImporter->Import(m_pScene);//7.

	// Fbx 축을 맞춰준다.
	FbxAxisSystem SceneAxisSystem = m_pScene->GetGlobalSettings().GetAxisSystem();

	if (SceneAxisSystem != FbxAxisSystem::Max)
		FbxAxisSystem::Max.ConvertScene(m_pScene);

	Triangulate(m_pScene->GetRootNode());

	if (!LoadNodeData(m_pScene->GetRootNode()))
		return false;

	pImporter->Destroy();

	return true;
}

void CDx11FbxLoader::Triangulate(FbxNode * pNode)
{
	// 폴리곤 타입이 삼각형이 아닐 경우 삼각형으로 변환해준다.
	// 노드의 속성을 받아와서 타입을 검사한다.
	FbxNodeAttribute*	pAttr = pNode->GetNodeAttribute();

	if (pAttr)
	{
		if (pAttr->GetAttributeType() == FbxNodeAttribute::eMesh ||
			pAttr->GetAttributeType() == FbxNodeAttribute::eNurbs ||
			pAttr->GetAttributeType() == FbxNodeAttribute::eNurbsSurface)
		{
			FbxGeometryConverter	converter(m_pManager);
			converter.Triangulate(pAttr, true);
		}
	}

	// 차일드 노드도 순회해서 동일한 처리를 해준다.
	int iChildCount = pNode->GetChildCount();
	for (int i = 0; i < iChildCount; ++i)
	{
		Triangulate(pNode->GetChild(i));
	}
}

bool CDx11FbxLoader::LoadNodeData(FbxNode * pNode)
{
	// 노드의 속성을 받아온다.
	FbxNodeAttribute*	pAttr = pNode->GetNodeAttribute();

	if (pAttr)
	{	
		// 매쉬 속성을 갖는다면
		if (pAttr->GetAttributeType() == FbxNodeAttribute::eMesh)
		{
			// 매쉬를 받아오고, 불러온다.
			FbxMesh*	pMesh = pNode->GetMesh();

			if (pMesh)
			{
				// 매쉬의 데이터를 로드한다.
				if (!LoadMeshData(pMesh))
					return false;

				// 매쉬가 가진 재질의 카운트만큼 루프를 돌린다.
				int iMtrlCount = pNode->GetMaterialCount();
				for (int i = 0; i < iMtrlCount; ++i)
				{
					// i번째 재질을 뽑아온다.
					FbxSurfaceMaterial* pMaterial = pNode->GetMaterial(i);
					if (!pMaterial)
						continue;

					// 뽑아온 재질을 로드한다.
					LoadMaterialData(pMaterial);
				}
			}
		}
	}

	// 마찬가지로 갖고 있는 차일드를 모두 순회해준다.
	int iChildCount = pNode->GetChildCount();
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

	PMESHDATA	pMeshData = new MESHDATA;
	pMeshData->bAnimation = false;
	pMeshData->bBump = false;
	m_vecMeshData.push_back(pMeshData);

	// 폴리곤 갯수를 얻어온다.
	// 컨트롤 포인트를 얻어온다. ControlPoint == 실제 정점의 수
	int iPolygonCount = pMesh->GetPolygonCount();
	FbxVector4* pControlPoints = pMesh->GetControlPoints();
	int iControlPointsCount = pMesh->GetControlPointsCount();

	// 벡터의 공간을 미리 확보해놓는다.
	pMeshData->vecPos.reserve(iControlPointsCount);
	pMeshData->vecNormal.reserve(iControlPointsCount);
	pMeshData->vecUV.reserve(iControlPointsCount);
	pMeshData->vecTangent.reserve(iControlPointsCount);
	pMeshData->vecBinormal.reserve(iControlPointsCount);
	pMeshData->vecWeights.reserve(iControlPointsCount);
	pMeshData->vecIndices.reserve(iControlPointsCount);

	// 정점의 위치 정보를 읽어온다.
	for (int i = 0; i < iControlPointsCount; ++i)
	{
		Vec3	vPos;
		vPos.x = (float)pControlPoints[i].mData[0];
		vPos.y = (float)pControlPoints[i].mData[2];
		vPos.z = (float)pControlPoints[i].mData[1];

		pMeshData->vecPos.push_back(vPos);

		//나머지 정보는 초기화만 해줌
		pMeshData->vecNormal.push_back(Vec3Zero);
		pMeshData->vecUV.push_back(Vec2(0.f, 0.f));
		pMeshData->vecTangent.push_back(Vec3Zero);
		pMeshData->vecBinormal.push_back(Vec3Zero);
		pMeshData->vecWeights.push_back(Vec4(0.f, 0.f, 0.f, 0.f));
		pMeshData->vecIndices.push_back(Vec4(0.f, 0.f, 0.f, 0.f));
	}

	pMeshData->iPolygonCount = iPolygonCount;

	UINT	iVertexIdx = 0;
	
	int iMtrlElementCount = pMesh->GetElementMaterialCount();
	// 재질의 수를 얻어온다.
	int iMtrlCount = pMesh->GetNode()->GetMaterialCount();
	for (int i = 0; i < iMtrlCount; ++i)
	{
		vector<UINT>	vecIndex;
		pMeshData->vecIndex.push_back(vecIndex);
	}

	// 폴리곤 수만큼 반복을 돌며 각 폴리곤의 정점 정보를 얻어온다.
	for (int i = 0; i < iPolygonCount; ++i)
	{
		// 해당 인덱스의 폴리곤 크기(정점의 갯수)를 얻어온다.
		int iPolygonSize = pMesh->GetPolygonSize(i);
		int iMaterialID = 0;

		if (iMtrlElementCount > 0)
		{
			FbxGeometryElementMaterial*	pMaterial = pMesh->GetElementMaterial(0);
			iMaterialID = pMaterial->GetIndexArray().GetAt(i);
		}

		int iIndex[3] = {};

		for (int j = 0; j < iPolygonSize; ++j)
		{
			// 삼각형을 구성하기 위한 정점의 인덱스 정보를 얻어온다.
			int iControlIndex = pMesh->GetPolygonVertex(i, j);
			
			iIndex[j] = iControlIndex;
			
			// 노말 정보를 얻어온다.
			// 정점 하나당 노말이 몇개 있을 지는 미지수이므로 루프를 돌려 처리한다.
			int iNormalCount = pMesh->GetElementNormalCount();

			for (int k = 0; k < iNormalCount; ++k)
			{
				FbxGeometryElementNormal* pNormal = pMesh->GetElementNormal(k);
				if (pNormal->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
				{
					int iNormalIndex = 0;
					FbxVector4	vFbxNormal;
					Vec3		vNormal;
					switch (pNormal->GetReferenceMode())
					{
					case FbxGeometryElement::eDirect:
						vFbxNormal = pNormal->GetDirectArray().GetAt(iVertexIdx);
						vNormal.x = vFbxNormal.mData[0];
						vNormal.y = vFbxNormal.mData[2];
						vNormal.z = vFbxNormal.mData[1];
						pMeshData->vecNormal[iControlIndex] = vNormal;
					//	pMeshData->vecNormal.push_back(vNormal);
						break;
					case FbxGeometryElement::eIndexToDirect:
						iNormalIndex = pNormal->GetIndexArray().GetAt(iVertexIdx);
						vFbxNormal = pNormal->GetDirectArray().GetAt(iNormalIndex);
						vNormal.x = vFbxNormal.mData[0];
						vNormal.y = vFbxNormal.mData[2];
						vNormal.z = vFbxNormal.mData[1];
						pMeshData->vecNormal[iControlIndex] = vNormal;
						break;
					}
				}
			}


			// 텍스쳐 UV 정보를 읽어온다.
			for (int k = 0; k < pMesh->GetElementUVCount(); ++k)
			{
				FbxGeometryElementUV*	pUV = pMesh->GetElementUV(k);

				if (pUV)
				{
					FbxVector2	vFbxUV;
					Vec2		vUV;
					int			iUVIndex = 0;
					switch (pUV->GetMappingMode())
					{
					case FbxGeometryElement::eByControlPoint:
						switch (pUV->GetReferenceMode())
						{
						case FbxGeometryElement::eDirect:
							vFbxUV = pUV->GetDirectArray().GetAt(iControlIndex);
							vUV.x = vFbxUV.mData[0];
							vUV.y = 1.f - vFbxUV.mData[1];
							pMeshData->vecUV[iControlIndex] = vUV;
							break;
						case FbxGeometryElement::eIndexToDirect:
							iUVIndex = pUV->GetIndexArray().GetAt(iControlIndex);
							vFbxUV = pUV->GetDirectArray().GetAt(iUVIndex);
							vUV.x = vFbxUV.mData[0];
							vUV.y = 1.f - vFbxUV.mData[1];
							pMeshData->vecUV[iControlIndex] = vUV;
							break;
						}
						break;
					case FbxGeometryElement::eByPolygonVertex:
						iUVIndex = pMesh->GetTextureUVIndex(i, j);
						switch (pUV->GetReferenceMode())
						{
						case FbxGeometryElement::eDirect:
						case FbxGeometryElement::eIndexToDirect:
							vFbxUV = pUV->GetDirectArray().GetAt(iUVIndex);
							vUV.x = vFbxUV.mData[0];
							vUV.y = 1.f - vFbxUV.mData[1];
							pMeshData->vecUV[iControlIndex] = vUV;
							break;
						}
						break;
					}
				}
			}


			// Tangent 정보를 읽어온다.
			int iTangentCount = pMesh->GetElementTangentCount();
			for (int k = 0; k < iTangentCount; ++k)
			{
				FbxGeometryElementTangent*	pTangent = pMesh->GetElementTangent(k);
				if (pTangent)
				{
					int iTangentIndex = 0;
					FbxVector4	vFbxTangent;
					Vec3		vTangent;
					switch (pTangent->GetMappingMode())
					{
					case FbxGeometryElement::eByPolygonVertex:
						switch (pTangent->GetReferenceMode())
						{
						case FbxGeometryElement::eDirect:
							vFbxTangent = pTangent->GetDirectArray().GetAt(iVertexIdx);
							vTangent.x = vFbxTangent.mData[0];
							vTangent.y = vFbxTangent.mData[2];
							vTangent.z = vFbxTangent.mData[1];
							
							pMeshData->vecTangent[iControlIndex] = vTangent;
							pMeshData->bBump = true;
							break;
						case FbxGeometryElement::eIndexToDirect:
							iTangentIndex = pTangent->GetIndexArray().GetAt(iVertexIdx);
							vFbxTangent = pTangent->GetDirectArray().GetAt(iTangentIndex);
							vTangent.x = vFbxTangent.mData[0];
							vTangent.y = vFbxTangent.mData[2];
							vTangent.z = vFbxTangent.mData[1];

							pMeshData->vecTangent[iControlIndex] = vTangent;
							pMeshData->bBump = true;
							break;
						}
						break;
					}
				}
			}

			// 바이노말 정보를 읽어온다.
			int iBinormalCount = pMesh->GetElementBinormalCount();
			for (int k = 0; k < iBinormalCount; ++k)
			{
				FbxGeometryElementBinormal* pBinormal = pMesh->GetElementBinormal(k);
				if (pBinormal)
				{
					int iBinormalIndex = 0;
					FbxVector4	vFbxBinormal;
					Vec3		vBinormal;
					switch (pBinormal->GetMappingMode())
					{
					case FbxGeometryElement::eByPolygonVertex:
						switch (pBinormal->GetReferenceMode())
						{
						case FbxGeometryElement::eDirect:
							vFbxBinormal = pBinormal->GetDirectArray().GetAt(iVertexIdx);
							vBinormal.x = vFbxBinormal.mData[0];
							vBinormal.y = vFbxBinormal.mData[2];
							vBinormal.z = vFbxBinormal.mData[1];
							pMeshData->vecBinormal[iControlIndex] = vBinormal;
							break;
						case FbxGeometryElement::eIndexToDirect:
							iBinormalIndex = pBinormal->GetIndexArray().GetAt(iVertexIdx);
							vFbxBinormal = pBinormal->GetDirectArray().GetAt(iBinormalIndex);
							vBinormal.x = vFbxBinormal.mData[0];
							vBinormal.y = vFbxBinormal.mData[2];
							vBinormal.z = vFbxBinormal.mData[1];
							pMeshData->vecBinormal[iControlIndex] = vBinormal;
							break;
						}
						break;
					}
				}
			}// Binormal

			++iVertexIdx;
		}// PolygonSize

		pMeshData->vecIndex[iMaterialID].push_back(iIndex[0]);
		pMeshData->vecIndex[iMaterialID].push_back(iIndex[2]);
		pMeshData->vecIndex[iMaterialID].push_back(iIndex[1]);

	}// PolygonCount

	return true;
}

bool CDx11FbxLoader::LoadMaterialData(FbxSurfaceMaterial * pMaterial)
{
	if (!pMaterial)
		return false;

	// 재질 정보 변수를 동적으로 할당
	// 현재 매쉬 컨테이너는 벡터의 가장 마지막 원소
	PFBXMATERIALDATA	pMtrlData = new FBXMATERIALDATA;
	PMESHDATA			pContainer = m_vecMeshData.back();

	pContainer->vecMaterial.push_back(pMtrlData);

	// 재질 노드 타입 설정
	if (pMaterial->GetClassId().Is(FbxSurfaceLambert::ClassId))
		pMtrlData->eNode = FMN_LAMBERT;

	else if (pMaterial->GetClassId().Is(FbxSurfacePhong::ClassId))
		pMtrlData->eNode = FMN_PHONG;


	// Diffuse
	pMtrlData->vDif = GetMaterialProperty(pMaterial, pMtrlData,
		FbxSurfaceMaterial::sDiffuse, FbxSurfaceMaterial::sDiffuseFactor);
	// Diffuse Texture
	GetMaterialTextureName(pMaterial, pMtrlData, FbxSurfaceMaterial::sDiffuse);

	// Ambient
	pMtrlData->vAmb = GetMaterialProperty(pMaterial, pMtrlData,
		FbxSurfaceMaterial::sAmbient, FbxSurfaceMaterial::sAmbientFactor);

	// Specular
	pMtrlData->vSpc = GetMaterialProperty(pMaterial, pMtrlData,
		FbxSurfaceMaterial::sSpecular, FbxSurfaceMaterial::sSpecularFactor);
	// Specular Texture
	GetMaterialTextureName(pMaterial, pMtrlData, FbxSurfaceMaterial::sSpecular);

	// Emissive
	pMtrlData->vEmv = GetMaterialProperty(pMaterial, pMtrlData,
		FbxSurfaceMaterial::sEmissive, FbxSurfaceMaterial::sEmissiveFactor);

	// Specular Power
	FbxProperty fbxPower = pMaterial->FindProperty(FbxSurfaceMaterial::sShininess);
	if (fbxPower.IsValid())
	{
		double dPower = fbxPower.Get<FbxDouble>();
		pMtrlData->fSpcPower = (float)dPower;
	}

	// TransparencyFactor
	FbxProperty	fbxTransparency = pMaterial->FindProperty(FbxSurfaceMaterial::sTransparencyFactor);
	if (fbxTransparency.IsValid())
	{
		double dTransparencyFactor = fbxTransparency.Get<FbxDouble>();
		pMtrlData->fTransparencyFactor = (float)dTransparencyFactor;
	}

	// NormalMap Texture
	GetMaterialTextureName(pMaterial, pMtrlData, FbxSurfaceMaterial::sNormalMap);

	return true;
}

Vec3 CDx11FbxLoader::GetMaterialProperty(FbxSurfaceMaterial * pMaterial,
	PFBXMATERIALDATA pMtrlData, const char * pPropertyName, 
	const char * pPropertyFactorName)
{
	// 재질의 속성을 읽어 저장하는 함수이다.
	// 디폴트로 타입을 None으로 설정.
	pMtrlData->eType = FMT_NONE;

	Vec3	vMtrl = Vec3Zero;

	// 재질의 Property, PropertyFactor를 얻어오고,
	// 유효한지 체크한다.
	FbxProperty fbxProperty = pMaterial->FindProperty(pPropertyName);
	FbxProperty fbxPropertyFactor = pMaterial->FindProperty(pPropertyFactorName);
	if (fbxProperty.IsValid() && fbxPropertyFactor.IsValid())
	{
		FbxDouble3	vResult = fbxProperty.Get<FbxDouble3>();
		double	dFactor = fbxPropertyFactor.Get<FbxDouble>();

		if (dFactor != 0)
		{
			vResult[0] *= dFactor;
			vResult[1] *= dFactor;
			vResult[2] *= dFactor;
		}

		// 값을 설정하고 타입을 COLOR로 셋
		vMtrl.x = vResult[0];
		vMtrl.y = vResult[1];
		vMtrl.z = vResult[2];
		pMtrlData->eType = FMT_COLOR;
	}

	return vMtrl;
}

bool CDx11FbxLoader::GetMaterialTextureName(FbxSurfaceMaterial * pMaterial, PFBXMATERIALDATA pMtrlData, const char * pPropertyName)
{
	// 재질의 텍스처를 읽어 저장하는 함수이다.

	// 재질의 Property을 얻어오고,
	// 유효한지 체크.
	FbxProperty fbxProperty = pMaterial->FindProperty(pPropertyName);
	if (fbxProperty.IsValid())
	{
		// 존재하는 텍스쳐 수를 설정한다.
		int iExistTextureCount = 0;
		// GetSrcObjectCount는 Template에 지정된 타입의 객체 수를 리턴해준다.
		const int iTextureCount = fbxProperty.GetSrcObjectCount<FbxFileTexture>();

		for (int i = 0; i < iTextureCount; ++i)
		{
			// TextureCount 만큼 루프를 돌면서
			// 해당 인덱스의 FbxFileTexture를 얻어온다.
			FbxFileTexture* pFileTexture = fbxProperty.GetSrcObject<FbxFileTexture>(i);
			if (!pFileTexture)
				continue;

			string strPath = pFileTexture->GetFileName();
			if (strcmp(pPropertyName, FbxSurfaceMaterial::sDiffuse) == 0)
				pMtrlData->strDiffuseTex = strPath;

			else if (strcmp(pPropertyName, FbxSurfaceMaterial::sSpecular) == 0)
				pMtrlData->strSpecularTex = strPath;

			else if (strcmp(pPropertyName, FbxSurfaceMaterial::sNormalMap) == 0)
				pMtrlData->strNormalMapTex = strPath;

			++iExistTextureCount;
		}

		// 레이어드 되어있을 경우도 처리한다.
		const int iLayeredTextureCount = fbxProperty.GetSrcObjectCount<FbxLayeredTexture>();
		for (int i = 0; i < iLayeredTextureCount; ++i)
		{
			FbxLayeredTexture*	pLayeredTexture = fbxProperty.GetSrcObject<FbxLayeredTexture>(i);
			const int	iTextureFileCount = pLayeredTexture->GetSrcObjectCount<FbxFileTexture>();
			for (int j = 0; j < iTextureFileCount; ++j)
			{
				FbxFileTexture*	pFileTexture = fbxProperty.GetSrcObject<FbxFileTexture>(j);
				if (!pFileTexture)
					continue;

				string strPath = pFileTexture->GetFileName();
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
	return true;
}
