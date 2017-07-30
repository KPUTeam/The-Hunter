#include "Dx11Mesh.h"
#include "../Core/Dx11Device.h"
#include "../Core/Dx11PathManager.h"
#include "../Component/Dx11Material.h"
#include "../Component/Dx11Animation3D.h"
#include "Dx11FbxLoader.h"

DX11_USING

CDx11Mesh::CDx11Mesh()
{
	SetTypeName<CDx11Mesh>();
	m_pAnimation = NULL;
}

CDx11Mesh::~CDx11Mesh()
{
	SAFE_RELEASE(m_pAnimation);

	for (size_t i = 0; i < m_vecMeshContainer.size(); ++i)
	{
		SAFE_RELEASE(m_vecMeshContainer[i]->tVB.pVB);

		for (size_t j = 0; j < m_vecMeshContainer[i]->vecIB.size(); ++j)
		{
			SAFE_RELEASE(m_vecMeshContainer[i]->vecIB[j].pIB);
		}

		for (size_t j = 0; j < m_vecMeshContainer[i]->vecMaterial.size(); ++j)
		{
			SAFE_RELEASE(m_vecMeshContainer[i]->vecMaterial[j]);
		}

		SAFE_DELETE(m_vecMeshContainer[i]);
	}

	m_vecMeshContainer.clear();
}

size_t CDx11Mesh::GetContainerCount() const
{
	return m_vecMeshContainer.size();
}

size_t CDx11Mesh::GetSubsetCount(int iContainer) const
{
	return m_vecMeshContainer[iContainer]->vecIB.size();
}

bool CDx11Mesh::IsValidMaterial() const
{
	if (m_vecMeshContainer[0]->vecMaterial.empty())
		return false;

	return true;
}

CDx11Material * CDx11Mesh::CloneMaterial(int iContainer, int iSubset) const
{
	if (m_vecMeshContainer.size() <= iContainer)
		return NULL;

	else if (m_vecMeshContainer[iContainer]->vecMaterial.size() <= iSubset)
		return NULL;

	return m_vecMeshContainer[iContainer]->vecMaterial[iSubset]->Clone();
}

bool CDx11Mesh::CreateBuffer(void * pVertices, UINT iVertexCount, UINT iVertexStride,
	void* pIndices, UINT iIndexCount, UINT iIndexStride, DXGI_FORMAT eIndexFmt,
	D3D11_PRIMITIVE_TOPOLOGY ePrimitive)
{
	PMESHCONTAINER	pContainer = new MESHCONTAINER;

	m_vecMeshContainer.push_back(pContainer);

	m_ePrimitive = ePrimitive;

	if (!CreateVertexBuffer(pVertices, iVertexCount, iVertexStride))
		return false;

	if (!CreateIndexBuffer(pIndices, iIndexCount, iIndexStride, eIndexFmt))
		return false;

	return true;
}

bool CDx11Mesh::LoadMesh(const string& strKey, TCHAR * pFileName, const string & strPathKey)
{
	wstring	strPath = DX11_GET_SINGLE(CDx11PathManager)->FindPath(strPathKey);
	strPath += pFileName;

	if (!LoadMesh(strKey, strPath.c_str()))
		return false;

	return true;
}

bool CDx11Mesh::LoadMesh(const string & strKey, const wchar_t* pFileName)
{
	char	strFullPath[MAX_PATH] = {};

	WideCharToMultiByte(CP_ACP, 0, pFileName, -1, strFullPath,
		lstrlen(pFileName) + 1, 0, 0);

	CDx11FbxLoader	fbxLoader;

	if (!fbxLoader.LoadMesh(strFullPath))
		return false;

	vector<PFBXMESHDATA>&	vecMeshContainer = fbxLoader.GetMeshContainer();
	bool	bAnimation = false;

	for (size_t i = 0; i < vecMeshContainer.size(); ++i)
	{
		PMESHCONTAINER		pContainer = new MESHCONTAINER;

		m_vecMeshContainer.push_back(pContainer);

		// 애니메이션이 있는지 검사한다.
		if (vecMeshContainer[i]->bAnimation)
		{
			bAnimation = true;
			// Tangent정보가 있는지 검사한다.
			if (vecMeshContainer[i]->bBump)
			{
				vector<ANIMBUMPVERTEX>	vecAnimBumpVtx;

				for (size_t j = 0; j < vecMeshContainer[i]->vecPos.size(); ++j)
				{
					ANIMBUMPVERTEX	tVtx;
					tVtx.vPos.x = vecMeshContainer[i]->vecPos[j].mData[0];
					tVtx.vPos.y = vecMeshContainer[i]->vecPos[j].mData[1];
					tVtx.vPos.z = vecMeshContainer[i]->vecPos[j].mData[2];

					tVtx.vNormal.x = vecMeshContainer[i]->vecNormal[j].mData[0];
					tVtx.vNormal.y = vecMeshContainer[i]->vecNormal[j].mData[1];
					tVtx.vNormal.z = vecMeshContainer[i]->vecNormal[j].mData[2];

					tVtx.vTex.x = vecMeshContainer[i]->vecUV[j].mData[0];
					tVtx.vTex.y = vecMeshContainer[i]->vecUV[j].mData[1];

					tVtx.vTangent.x = vecMeshContainer[i]->vecTangent[j].mData[0];
					tVtx.vTangent.y = vecMeshContainer[i]->vecTangent[j].mData[1];
					tVtx.vTangent.z = vecMeshContainer[i]->vecTangent[j].mData[2];

					tVtx.vBinormal.x = vecMeshContainer[i]->vecBinormal[j].mData[0];
					tVtx.vBinormal.y = vecMeshContainer[i]->vecBinormal[j].mData[1];
					tVtx.vBinormal.z = vecMeshContainer[i]->vecBinormal[j].mData[2];

					tVtx.vWeights.x = vecMeshContainer[i]->vecBlendWeights[j].mData[0];
					tVtx.vWeights.y = vecMeshContainer[i]->vecBlendWeights[j].mData[1];
					tVtx.vWeights.z = vecMeshContainer[i]->vecBlendWeights[j].mData[2];
					tVtx.vWeights.w = vecMeshContainer[i]->vecBlendWeights[j].mData[3];

					tVtx.vIndices.x = vecMeshContainer[i]->vecBlendIndices[j].mData[0];
					tVtx.vIndices.y = vecMeshContainer[i]->vecBlendIndices[j].mData[1];
					tVtx.vIndices.z = vecMeshContainer[i]->vecBlendIndices[j].mData[2];
					tVtx.vIndices.w = vecMeshContainer[i]->vecBlendIndices[j].mData[3];

					vecAnimBumpVtx.push_back(tVtx);
				}

				CreateVertexBuffer(&vecAnimBumpVtx[0], vecAnimBumpVtx.size(),
					sizeof(ANIMBUMPVERTEX));
			}

			else
			{
				vector<ANIMTEXVERTEX>	vecAnimTexVtx;

				for (size_t j = 0; j < vecMeshContainer[i]->vecPos.size(); ++j)
				{
					ANIMTEXVERTEX	tVtx;
					tVtx.vPos.x = vecMeshContainer[i]->vecPos[j].mData[0];
					tVtx.vPos.y = vecMeshContainer[i]->vecPos[j].mData[1];
					tVtx.vPos.z = vecMeshContainer[i]->vecPos[j].mData[2];

					tVtx.vNormal.x = vecMeshContainer[i]->vecNormal[j].mData[0];
					tVtx.vNormal.y = vecMeshContainer[i]->vecNormal[j].mData[1];
					tVtx.vNormal.z = vecMeshContainer[i]->vecNormal[j].mData[2];

					tVtx.vTex.x = vecMeshContainer[i]->vecUV[j].mData[0];
					tVtx.vTex.y = vecMeshContainer[i]->vecUV[j].mData[1];

					tVtx.vWeights.x = vecMeshContainer[i]->vecBlendWeights[j].mData[0];
					tVtx.vWeights.y = vecMeshContainer[i]->vecBlendWeights[j].mData[1];
					tVtx.vWeights.z = vecMeshContainer[i]->vecBlendWeights[j].mData[2];
					tVtx.vWeights.w = vecMeshContainer[i]->vecBlendWeights[j].mData[3];

					tVtx.vIndices.x = vecMeshContainer[i]->vecBlendIndices[j].mData[0];
					tVtx.vIndices.y = vecMeshContainer[i]->vecBlendIndices[j].mData[1];
					tVtx.vIndices.z = vecMeshContainer[i]->vecBlendIndices[j].mData[2];
					tVtx.vIndices.w = vecMeshContainer[i]->vecBlendIndices[j].mData[3];

					vecAnimTexVtx.push_back(tVtx);
				}

				CreateVertexBuffer(&vecAnimTexVtx[0], vecAnimTexVtx.size(),
					sizeof(ANIMTEXVERTEX));
			}
		}

		else
		{
			// Tangent정보가 있는지 검사한다.
			if (vecMeshContainer[i]->bBump)
			{
				vector<BUMPVERTEX>	vecBumpVtx;

				for (size_t j = 0; j < vecMeshContainer[i]->vecPos.size(); ++j)
				{
					BUMPVERTEX	tVtx;
					tVtx.vPos.x = vecMeshContainer[i]->vecPos[j].mData[0];
					tVtx.vPos.y = vecMeshContainer[i]->vecPos[j].mData[1];
					tVtx.vPos.z = vecMeshContainer[i]->vecPos[j].mData[2];

					tVtx.vNormal.x = vecMeshContainer[i]->vecNormal[j].mData[0];
					tVtx.vNormal.y = vecMeshContainer[i]->vecNormal[j].mData[1];
					tVtx.vNormal.z = vecMeshContainer[i]->vecNormal[j].mData[2];

					tVtx.vTex.x = vecMeshContainer[i]->vecUV[j].mData[0];
					tVtx.vTex.y = vecMeshContainer[i]->vecUV[j].mData[1];

					tVtx.vTangent.x = vecMeshContainer[i]->vecTangent[j].mData[0];
					tVtx.vTangent.y = vecMeshContainer[i]->vecTangent[j].mData[1];
					tVtx.vTangent.z = vecMeshContainer[i]->vecTangent[j].mData[2];

					tVtx.vBinormal.x = vecMeshContainer[i]->vecBinormal[j].mData[0];
					tVtx.vBinormal.y = vecMeshContainer[i]->vecBinormal[j].mData[1];
					tVtx.vBinormal.z = vecMeshContainer[i]->vecBinormal[j].mData[2];

					vecBumpVtx.push_back(tVtx);
				}

				CreateVertexBuffer(&vecBumpVtx[0], vecBumpVtx.size(),
					sizeof(BUMPVERTEX));
			}

			else
			{
				vector<TEXVERTEX>	vecTexVtx;

				for (size_t j = 0; j < vecMeshContainer[i]->vecPos.size(); ++j)
				{
					TEXVERTEX	tVtx;
					tVtx.vPos.x = vecMeshContainer[i]->vecPos[j].mData[0];
					tVtx.vPos.y = vecMeshContainer[i]->vecPos[j].mData[1];
					tVtx.vPos.z = vecMeshContainer[i]->vecPos[j].mData[2];

					tVtx.vNormal.x = vecMeshContainer[i]->vecNormal[j].mData[0];
					tVtx.vNormal.y = vecMeshContainer[i]->vecNormal[j].mData[1];
					tVtx.vNormal.z = vecMeshContainer[i]->vecNormal[j].mData[2];

					tVtx.vTex.x = vecMeshContainer[i]->vecUV[j].mData[0];
					tVtx.vTex.y = vecMeshContainer[i]->vecUV[j].mData[1];

					vecTexVtx.push_back(tVtx);
				}

				CreateVertexBuffer(&vecTexVtx[0], vecTexVtx.size(),
					sizeof(TEXVERTEX));
			}
		}

		for (size_t j = 0; j < vecMeshContainer[i]->vecIndex.size(); ++j)
		{
			CreateIndexBuffer(&vecMeshContainer[i]->vecIndex[j][0],
				vecMeshContainer[i]->vecIndex[j].size(), sizeof(UINT),
				DXGI_FORMAT_R32_UINT);
		}

		// 재질 개수만큼 반복문을 돌면서 재질을 만들어준다.
		for (size_t j = 0; j < vecMeshContainer[i]->vecMaterial.size(); ++j)
		{
			CDx11Material*	pMaterial = new CDx11Material;

			if (!pMaterial->Init())
			{
				SAFE_RELEASE(pMaterial);
				return false;
			}

			Vec4	vDif, vAmb, vSpc, vEmv;

			vDif.x = vecMeshContainer[i]->vecMaterial[j]->vDif.mData[0];
			vDif.y = vecMeshContainer[i]->vecMaterial[j]->vDif.mData[1];
			vDif.z = vecMeshContainer[i]->vecMaterial[j]->vDif.mData[2];

			vAmb.x = vecMeshContainer[i]->vecMaterial[j]->vAmb.mData[0];
			vAmb.y = vecMeshContainer[i]->vecMaterial[j]->vAmb.mData[1];
			vAmb.z = vecMeshContainer[i]->vecMaterial[j]->vAmb.mData[2];

			vSpc.x = vecMeshContainer[i]->vecMaterial[j]->vSpc.mData[0];
			vSpc.y = vecMeshContainer[i]->vecMaterial[j]->vSpc.mData[1];
			vSpc.z = vecMeshContainer[i]->vecMaterial[j]->vSpc.mData[2];

			vEmv.x = vecMeshContainer[i]->vecMaterial[j]->vEmv.mData[0];
			vEmv.y = vecMeshContainer[i]->vecMaterial[j]->vEmv.mData[1];
			vEmv.z = vecMeshContainer[i]->vecMaterial[j]->vEmv.mData[2];

			pMaterial->SetDiffuse(vDif);
			pMaterial->SetAmbient(vAmb);
			pMaterial->SetSpecular(vSpc);
			pMaterial->SetEmissive(vEmv);
			pMaterial->SetSpecularPower(vecMeshContainer[i]->vecMaterial[j]->fPower);

			char	strTexKey[256] = {};
			sprintf_s(strTexKey, "%s_%dContainer_%dSubset_Dif", strKey.c_str(),
				i, j);
			pMaterial->SetDiffuseTexture(strTexKey,
				vecMeshContainer[i]->vecMaterial[j]->strDiffuseTex);

			if (!vecMeshContainer[i]->vecMaterial[j]->strSpecularTex.empty())
			{
				memset(strTexKey, 0, 256);
				sprintf_s(strTexKey, "%s_%dContainer_%dSubset_Spc", strKey.c_str(),
					i, j);
				pMaterial->SetSpecularTexture(strTexKey,
					vecMeshContainer[i]->vecMaterial[j]->strSpecularTex);
			}

			if (!vecMeshContainer[i]->vecMaterial[j]->strNormalMapTex.empty())
			{
				memset(strTexKey, 0, 256);
				sprintf_s(strTexKey, "%s_%dContainer_%dSubset_Nrm", strKey.c_str(),
					i, j);
				pMaterial->SetNormalMapTexture(strTexKey,
					vecMeshContainer[i]->vecMaterial[j]->strNormalMapTex);
			}

			m_vecMeshContainer[i]->vecMaterial.push_back(pMaterial);
		}
	}

	// 애니메이션 정보를 읽어온다.
	if (bAnimation)
	{
		m_pAnimation = new CDx11Animation3D;

		if (!m_pAnimation->Init())
			return false;

		char	strFileName[MAX_PATH] = {};

		WideCharToMultiByte(CP_ACP, 0, pFileName, -1, strFileName,
			lstrlen(pFileName) + 1, NULL, NULL);

		m_pAnimation->SetMeshName(strFileName);

		// Animatino Clip정보를 생성한다.
		vector<PFBXANIMATIONCLIP>& vecClip = fbxLoader.GetAnimationClip();
		for (size_t i = 0; i < vecClip.size(); ++i)
		{
			ANIMATIONCLIP	tClip = {};

			tClip.strName = vecClip[i]->strName;

			tClip.iStartFrame = vecClip[i]->startTime.GetFrameCount(FbxTime::eFrames30);
			tClip.iEndFrame = vecClip[i]->endTime.GetFrameCount(FbxTime::eFrames30);
			tClip.iFrameLength = tClip.iEndFrame - tClip.iStartFrame;

			tClip.dStartTime = vecClip[i]->startTime.GetSecondDouble();
			tClip.dEndTime = vecClip[i]->endTime.GetSecondDouble();
			tClip.dTimeLength = tClip.dEndTime - tClip.dStartTime;

			m_pAnimation->SetAllClip(tClip);
		}

		// 로더에서 본정보를 얻어온다.
		vector<FBXBONE>&	vecBones = fbxLoader.GetBones();
		vector<BONE>	vecBoneHierarchy;

		vecBoneHierarchy.reserve(vecBones.size());

		for (size_t i = 0; i < vecBones.size(); ++i)
		{
			BONE	tBone = {};

			tBone.matBone = XMMatrixIdentity();

			tBone.strName = vecBones[i].strName;
			tBone.iParentIndex = vecBones[i].iParentIndex;
			tBone.iDepth = vecBones[i].iDepth;
			float	matBone[16];

			for (int j = 0; j < 4; ++j)
			{
				for (int k = 0; k < 4; ++k)
				{
					matBone[j * 4 + k] = vecBones[i].matTransform.Get(j, k);
				}
			}

			memcpy(&tBone.matTransform, matBone, sizeof(float) * 16);

			for (int j = 0; j < 4; ++j)
			{
				for (int k = 0; k < 4; ++k)
				{
					matBone[j * 4 + k] = vecBones[i].matOffset.Get(j, k);
				}
			}

			memcpy(&tBone.matOffset, matBone, sizeof(float) * 16);

			FbxVector4		vT, vS;
			FbxQuaternion	qRot;

			vT = vecBones[i].matOffset.GetT();
			vS = vecBones[i].matOffset.GetS();
			qRot = vecBones[i].matOffset.GetQ();

			Vec3	vScale;
			vScale.x = vS.mData[0];
			vScale.y = vS.mData[1];
			vScale.z = vS.mData[2];

			Vec3	vPos;
			vPos.x = vT.mData[0];
			vPos.y = vT.mData[1];
			vPos.z = vT.mData[2];

			Quaternion	q;
			q.x = qRot.mData[0];
			q.y = qRot.mData[1];
			q.z = qRot.mData[2];
			q.w = qRot.mData[3];

			Vector	vZero = XMVectorSet(0.f, 0.f, 0.f, 1.f);
			tBone.matOffset = XMMatrixAffineTransformation(vScale.Convert(),
				vZero, q.Convert(), vPos.Convert());

			for (size_t j = 0; j < vecBones[i].vecKeyFrame.size(); ++j)
			{
				ANIMATIONKEYFRAME	tKeyFrame = {};

				tKeyFrame.fAnimationTime = vecBones[i].vecKeyFrame[j].dTime;
				FbxAMatrix	matTransform = vecBones[i].vecKeyFrame[j].matTransform;

				vT = matTransform.GetT();
				vS = matTransform.GetS();
				qRot = matTransform.GetQ();

				tKeyFrame.vTranslate.x = vT.mData[0];
				tKeyFrame.vTranslate.y = vT.mData[1];
				tKeyFrame.vTranslate.z = vT.mData[2];

				tKeyFrame.vScale.x = vS.mData[0];
				tKeyFrame.vScale.y = vS.mData[1];
				tKeyFrame.vScale.z = vS.mData[2];

				tKeyFrame.qRot.x = qRot.mData[0];
				tKeyFrame.qRot.y = qRot.mData[1];
				tKeyFrame.qRot.z = qRot.mData[2];
				tKeyFrame.qRot.w = qRot.mData[3];

				tBone.vecKeyFrame.push_back(tKeyFrame);
			}

			vecBoneHierarchy.push_back(tBone);
		}

		m_pAnimation->CreateBones(vecBoneHierarchy);
	}

	m_ePrimitive = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	return true;
}

bool CDx11Mesh::CreateVertexBuffer(void * pVertices, UINT iVertexCount, UINT iVertexStride)
{
	PMESHCONTAINER	pContainer = m_vecMeshContainer.back();
	pContainer->tVB.iVertexCount = iVertexCount;
	pContainer->tVB.iVertexStride = iVertexStride;

	D3D11_BUFFER_DESC	tBuffer = {};

	tBuffer.ByteWidth = pContainer->tVB.iVertexCount * pContainer->tVB.iVertexStride;
	tBuffer.Usage = D3D11_USAGE_DEFAULT;
	tBuffer.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA	tData = {};
	tData.pSysMem = pVertices;

	if (FAILED(DX11_DEVICE->CreateBuffer(&tBuffer, &tData, &pContainer->tVB.pVB)))
		return false;

	return true;
}

bool CDx11Mesh::CreateIndexBuffer(void * pIndices, UINT iIndexCount, UINT iIndexStride,
	DXGI_FORMAT eIndexFmt)
{
	INDEXBUFFER	tIB = {};

	PMESHCONTAINER	pContainer = m_vecMeshContainer.back();
	tIB.iIndexCount = iIndexCount;
	tIB.iIndexStride = iIndexStride;
	tIB.eIndexFmt = eIndexFmt;
	tIB.iPrimitiveCount = iIndexCount / 3;

	D3D11_BUFFER_DESC	tBuffer = {};

	tBuffer.ByteWidth = tIB.iIndexCount * tIB.iIndexStride;
	tBuffer.Usage = D3D11_USAGE_DEFAULT;
	tBuffer.BindFlags = D3D11_BIND_INDEX_BUFFER;

	D3D11_SUBRESOURCE_DATA	tData = {};
	tData.pSysMem = pIndices;

	if (FAILED(DX11_DEVICE->CreateBuffer(&tBuffer, &tData, &tIB.pIB)))
		return false;

	pContainer->vecIB.push_back(tIB);

	return true;
}
void CDx11Mesh::Render()
{
	for (int i = 0; i < m_vecMeshContainer.size(); ++i)
	{
		PMESHCONTAINER	pContainer = m_vecMeshContainer[i];

		for (size_t j = 0; j < pContainer->vecIB.size(); ++j)
		{
			UINT	iOffset = 0;
			DX11_CONTEXT->IASetPrimitiveTopology(m_ePrimitive);
			DX11_CONTEXT->IASetVertexBuffers(0, 1, &pContainer->tVB.pVB,
				&pContainer->tVB.iVertexStride, &iOffset);
			DX11_CONTEXT->IASetIndexBuffer(pContainer->vecIB[j].pIB,
				pContainer->vecIB[j].eIndexFmt, 0);
			DX11_CONTEXT->DrawIndexed(pContainer->vecIB[j].iIndexCount, 0, 0);
		}
	}
}

void CDx11Mesh::Render(int iContainer, int iSubset)
{
	PMESHCONTAINER	pContainer = m_vecMeshContainer[iContainer];
	UINT	iOffset = 0;
	DX11_CONTEXT->IASetPrimitiveTopology(m_ePrimitive);
	DX11_CONTEXT->IASetVertexBuffers(0, 1, &pContainer->tVB.pVB,
		&pContainer->tVB.iVertexStride, &iOffset);
	DX11_CONTEXT->IASetIndexBuffer(pContainer->vecIB[iSubset].pIB,
		pContainer->vecIB[iSubset].eIndexFmt, 0);
	DX11_CONTEXT->DrawIndexed(pContainer->vecIB[iSubset].iIndexCount, 0, 0);
}

CDx11Animation3D * CDx11Mesh::CloneAnimationController() const
{
	if (!m_pAnimation)
		return NULL;

	return m_pAnimation->Clone();
}
