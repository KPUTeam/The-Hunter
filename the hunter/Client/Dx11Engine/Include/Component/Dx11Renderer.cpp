#include "Dx11Renderer.h"
#include "../Resources/Dx11Mesh.h"
#include "../Shader/Dx11Shader.h"
#include "../Resources/Dx11ResourcesManager.h"
#include "../Shader/Dx11ShaderManager.h"
#include "../Scene/Dx11Scene.h"
#include "../GameObject/Dx11GameObject.h"
#include "Dx11Light.h"
#include "Dx11Camera.h"
#include "../Core/Dx11Device.h"
#include "Dx11Transform.h"
#include "Dx11Material.h"
#include "../Rendering/Dx11RenderState.h"
#include "../Rendering/Dx11RenderManager.h"
#include "Dx11Animation2D.h"
#include "Dx11Animation3D.h"
#include "../Resources/Dx11Texture.h"

DX11_USING

CDx11Renderer::CDx11Renderer()	:
	m_pMesh(NULL),
	m_pShader(NULL),
	m_pBoneTexture(NULL)
{
	m_strCameraTag = "MainCamera";
	SetTypeName<CDx11Renderer>();
	m_eComType = CT_RENDERER;
	memset(m_pRenderState, 0, sizeof(CDx11RenderState*) * RSF_MAX);
}

CDx11Renderer::CDx11Renderer(const CDx11Renderer & renderer) :
	CDx11Component(renderer)
{
	*this = renderer;
	m_iRef = 1;

	if (m_pMesh) 
		m_pMesh->AddRef();
	

	if (m_pShader)
		m_pShader->AddRef();
	
	for (int i = 0; i < RSF_MAX; ++i)
	{
		if (m_pRenderState[i])
			m_pRenderState[i]->AddRef();
	}

	m_vecMaterial.clear();

	for (int i = 0; i < renderer.m_vecMaterial.size(); ++i)
	{
		vector<CDx11Material*>	vecMtrl;
		m_vecMaterial.push_back(vecMtrl);
		for (int j = 0; j < renderer.m_vecMaterial[i].size(); ++j)
		{
			CDx11Material*	pMtrl = renderer.m_vecMaterial[i][j]->Clone();

			m_vecMaterial[i].push_back(pMtrl);
		}
	}

	m_mapCBuffer.clear();

	unordered_map<string, PCBUFFER>::const_iterator iter;
	unordered_map<string, PCBUFFER>::const_iterator iterEnd = m_mapCBuffer.end();

	for (iter = m_mapCBuffer.begin(); iter != iterEnd; ++iter)
	{
		PCBUFFER	pBuffer = new CBUFFER;
		pBuffer->strKey = iter->second->strKey;
		pBuffer->iSize = iter->second->iSize;
		pBuffer->pData = new char[pBuffer->iSize];
		pBuffer->iShaderType = iter->second->iShaderType;
		memcpy(pBuffer->pData, iter->second->pData, pBuffer->iSize);

		m_mapCBuffer.insert(make_pair(iter->first, pBuffer));
	}
}

CDx11Renderer::~CDx11Renderer()
{
	for (int i = 0; i < RSF_MAX; ++i)
	{
		SAFE_RELEASE(m_pRenderState[i]);
	}

	for (size_t i = 0; i < m_vecMaterial.size(); ++i)
	{
		Safe_Release_VecList(m_vecMaterial[i]);
	}

	unordered_map<string, PCBUFFER>::iterator iter;
	unordered_map<string, PCBUFFER>::iterator iterEnd = m_mapCBuffer.end();

	for (iter = m_mapCBuffer.begin(); iter != iterEnd; ++iter)
	{
		SAFE_DELETE_ARRAY(iter->second->pData);
	}

	Safe_Delete_Map(m_mapCBuffer);
	SAFE_RELEASE(m_pMesh);
	SAFE_RELEASE(m_pShader);

}

CDx11Material * CDx11Renderer::GetMaterial(UINT iContainer, UINT iSubset)
{
	if (iContainer >= m_vecMaterial.size())
		return NULL;

	else if (iSubset >= m_vecMaterial[iContainer].size())
		return NULL;

	m_vecMaterial[iContainer][iSubset]->AddRef();

	return m_vecMaterial[iContainer][iSubset];
}

bool CDx11Renderer::IsRender2D() const
{
	return m_strCameraTag == "UICamera";
}

bool CDx11Renderer::AddCBuffer(const string & strKey, 
	const string & strCBufferKey, int iSize, int iShaderType)
{
	if (FindCBuffer(strKey))
		return false;

	PCBUFFER	pBuffer = new CBUFFER;

	pBuffer->strKey = strCBufferKey;
	pBuffer->iSize = iSize;
	pBuffer->pData = new char[iSize];
	pBuffer->iShaderType = iShaderType;

	m_mapCBuffer.insert(make_pair(strKey, pBuffer));

	return false;
}

PCBUFFER CDx11Renderer::FindCBuffer(const string & strKey)
{
	unordered_map<string, PCBUFFER>::const_iterator iter = m_mapCBuffer.find(strKey);

	if (iter == m_mapCBuffer.end())
		return NULL;

	return iter->second;
}

bool CDx11Renderer::UpdateCBuffer(const string & strKey, 
	void * pData, int iSize)
{
	PCBUFFER	pBuffer = FindCBuffer(strKey);

	if (!pBuffer)
		return false;

	memcpy(pBuffer->pData, pData, iSize);

	return true;
}

void CDx11Renderer::UpdateCBuffer()
{
	unordered_map<string, PCBUFFER>::iterator iter;
	unordered_map<string, PCBUFFER>::iterator iterEnd = m_mapCBuffer.end();

	for (iter = m_mapCBuffer.begin(); iter != iterEnd; ++iter)
	{
		m_pShader->UpdateConstantBuffer(iter->second->strKey,
			iter->second->pData, (SHADER_TYPE)iter->second->iShaderType);
	}
}

void CDx11Renderer::SetBoneTexture(CDx11Texture * pBoneTex)
{
	m_pBoneTexture = pBoneTex;
}

bool CDx11Renderer::Init()
{
	return true;
}

int CDx11Renderer::Update(float fTime)
{
	return 0;
}

int CDx11Renderer::LateUpdate(float fTime)
{
	return 0;
}

void CDx11Renderer::Render(float fTime)
{
	UpdateTransform();

	UpdateLight();

	UpdateMaterial();

	UpdateCBuffer();

	UpdateAnimation();

	UpdateRenderState();

	m_pShader->SetShader();

	if (m_pBoneTexture)
		m_pBoneTexture->SetTexture(3, 1);

	for (size_t i = 0; i < m_pMesh->GetContainerCount(); ++i)
	{
		for (size_t j = 0; j < m_pMesh->GetSubsetCount(i); ++j)
		{
			m_vecMaterial[i][j]->SetMaterial(m_pShader);
			m_pMesh->Render(i, j);
		}
	}

	m_pShader->End();

	ResetRenderState();
}

CDx11Renderer * CDx11Renderer::Clone()
{
	return new CDx11Renderer(*this);
}

CDx11Mesh * CDx11Renderer::GetMesh() const
{
	m_pMesh->AddRef();
	return m_pMesh;
}

void CDx11Renderer::SetCameraTag(const string & strTag)
{
	m_strCameraTag = strTag;
}

void CDx11Renderer::SetMesh(CDx11Mesh * pMesh)
{
	SAFE_RELEASE(m_pMesh);
	pMesh->AddRef();
	m_pMesh = pMesh;

	if (m_pMesh->IsValidMaterial())
	{
		for (size_t i = 0; i < m_vecMaterial.size(); ++i)
		{
			Safe_Release_VecList(m_vecMaterial[i]);
		}

		for (size_t i = 0; i < m_pMesh->GetContainerCount(); ++i)
		{
			vector<CDx11Material*>	vecMtrl;
			m_vecMaterial.push_back(vecMtrl);

			for (size_t j = 0; j < m_pMesh->GetSubsetCount(i); ++j)
			{
				m_vecMaterial[i].push_back(m_pMesh->CloneMaterial(i, j));
			}
		}
	}
	
	if (m_pMesh)
	{
		// 애니메이션이 있을 경우 복사해준다.
		CDx11Animation3D*	pAnimation = m_pMesh->CloneAnimationController();

		if (pAnimation)
		{
			m_pGameObject->AddComponent(pAnimation);
			SAFE_RELEASE(pAnimation);

			AddCBuffer("BoneArray", "BoneArray", sizeof(BONECBUFFER), ST_VERTEX);
		}
	}
}

void CDx11Renderer::SetMesh(const string & strKey)
{
	SAFE_RELEASE(m_pMesh);
	m_pMesh = DX11_GET_SINGLE(CDx11ResourcesManager)->FindMesh(strKey);

	if (m_pMesh && m_pMesh->IsValidMaterial())
	{
		for (size_t i = 0; i < m_vecMaterial.size(); ++i)
		{
			Safe_Release_VecList(m_vecMaterial[i]);
		}

		for (size_t i = 0; i < m_pMesh->GetContainerCount(); ++i)
		{
			vector<CDx11Material*>	vecMtrl;
			m_vecMaterial.push_back(vecMtrl);

			for (size_t j = 0; j < m_pMesh->GetSubsetCount(i); ++j)
			{
				m_vecMaterial[i].push_back(m_pMesh->CloneMaterial(i, j));
			}
		}
	}

	if (m_pMesh)
	{
		// 애니메이션이 있을 경우 복사해준다.
		CDx11Animation3D*	pAnimation = m_pMesh->CloneAnimationController();

		if (pAnimation)
		{
			m_pGameObject->AddComponent(pAnimation);
			SAFE_RELEASE(pAnimation);

			AddCBuffer("BoneArray", "BoneArray", sizeof(BONECBUFFER), ST_VERTEX);
		}
	}
}

void CDx11Renderer::SetMesh(const string & strKey, TCHAR * pFileName, const string & strPathKey)
{
	SAFE_RELEASE(m_pMesh);

	m_pMesh = DX11_GET_SINGLE(CDx11ResourcesManager)->LoadMesh(strKey, pFileName, strPathKey);

	if (m_pMesh && m_pMesh->IsValidMaterial())
	{
		for (size_t i = 0; i < m_vecMaterial.size(); ++i)
		{
			Safe_Release_VecList(m_vecMaterial[i]);
		}

		for (size_t i = 0; i < m_pMesh->GetContainerCount(); ++i)
		{
			vector<CDx11Material*>	vecMtrl;
			m_vecMaterial.push_back(vecMtrl);

			for (size_t j = 0; j < m_pMesh->GetSubsetCount(i); ++j)
			{
				m_vecMaterial[i].push_back(m_pMesh->CloneMaterial(i, j));
			}
		}
	}

	if (m_pMesh)
	{
		// 애니메이션이 있을 경우 복사해준다.
		CDx11Animation3D*	pAnimation = m_pMesh->CloneAnimationController();

		if (pAnimation)
		{
			m_pGameObject->AddComponent(pAnimation);
			SAFE_RELEASE(pAnimation);

			AddCBuffer("BoneArray", "BoneArray", sizeof(BONECBUFFER), ST_VERTEX);
		}
	}
}

void CDx11Renderer::SetMeshFromFullPath(const string & strKey, const wchar_t * pFullPath)
{
	SAFE_RELEASE(m_pMesh);
	m_pMesh = DX11_GET_SINGLE(CDx11ResourcesManager)->LoadMesh(strKey, pFullPath);

	if (m_pMesh && m_pMesh->IsValidMaterial())
	{
		for (size_t i = 0; i < m_vecMaterial.size(); ++i)
		{
			Safe_Release_VecList(m_vecMaterial[i]);
		}

		for (size_t i = 0; i < m_pMesh->GetContainerCount(); ++i)
		{
			vector<CDx11Material*>	vecMtrl;
			m_vecMaterial.push_back(vecMtrl);

			for (size_t j = 0; j < m_pMesh->GetSubsetCount(i); ++j)
			{
				m_vecMaterial[i].push_back(m_pMesh->CloneMaterial(i, j));
			}
		}
	}

	if (m_pMesh)
	{
		// 애니메이션이 있을 경우 복사해준다.
		CDx11Animation3D*	pAnimation = m_pMesh->CloneAnimationController();

		if (pAnimation)
		{
			m_pGameObject->AddComponent(pAnimation);
			SAFE_RELEASE(pAnimation);

			AddCBuffer("BoneArray", "BoneArray", sizeof(BONECBUFFER),
				ST_VERTEX);
		}
	}
}

void CDx11Renderer::SetShader(CDx11Shader * pShader)
{
	pShader->AddRef();
	m_pShader = pShader;
}

void CDx11Renderer::SetShader(const string & strKey)
{
	m_pShader = DX11_GET_SINGLE(CDx11ShaderManager)->FindShader(strKey);
}

void CDx11Renderer::AddMaterial(CDx11Material * pMtrl, int iContainer)
{
	if (m_vecMaterial.empty())
	{
		vector<CDx11Material*>	vecMtrl;
		m_vecMaterial.push_back(vecMtrl);
	}

	if (m_vecMaterial.size() <= iContainer)
		return;

	pMtrl->AddRef();
	m_vecMaterial[iContainer].push_back(pMtrl);
}

void CDx11Renderer::DeleteMaterial(UINT iContainer, UINT iSubset)
{
	if (m_vecMaterial.size() <= iContainer)
		return;

	else if(m_vecMaterial[iContainer].size() <= iSubset)
		return;

	m_vecMaterial[iContainer][iSubset]->Release();

	vector<CDx11Material*>::iterator	iter = m_vecMaterial[iContainer].begin() + iSubset;
	m_vecMaterial[iContainer].erase(iter);
}

void CDx11Renderer::CreateMaterial(const string & strDiffTexKey, UINT iContainer)
{
	if (m_vecMaterial.empty())
	{
		vector<CDx11Material*>	vecMtrl;
		m_vecMaterial.push_back(vecMtrl);
	}

	if (m_vecMaterial.size() <= iContainer)
		return;

	CDx11Material*	pMtrl = new CDx11Material;

	if (!pMtrl->Init())
	{
		SAFE_RELEASE(pMtrl);
		return;
	}

	pMtrl->SetDiffuseTexture(strDiffTexKey);
	
	m_vecMaterial[iContainer].push_back(pMtrl);
}

void CDx11Renderer::CreateMaterial(const string & strDiffTexKey,
	TCHAR * pFileName, const string & strPathKey, UINT iContainer)
{
	if (m_vecMaterial.empty())
	{
		vector<CDx11Material*>	vecMtrl;
		m_vecMaterial.push_back(vecMtrl);
	}

	if (m_vecMaterial.size() <= iContainer)
		return;

	CDx11Material*	pMtrl = new CDx11Material;

	if (!pMtrl->Init())
	{
		SAFE_RELEASE(pMtrl);
		return;
	}

	pMtrl->SetDiffuseTexture(strDiffTexKey, pFileName, strPathKey);

	m_vecMaterial[iContainer].push_back(pMtrl);
}

void CDx11Renderer::SetMaterialDiffuseTex(const string & strKey, 
	const UINT& iContainer, UINT iSubset)
{
	if (m_vecMaterial.empty())
	{
		CreateMaterial(strKey);
	}

	else
	{
		if (m_vecMaterial.size() <= iContainer)
			return;

		else if (m_vecMaterial[iContainer].empty())
		{
			CreateMaterial(strKey, iContainer);
		}

		else if (m_vecMaterial[iContainer].size() <= iSubset)
			return;

		m_vecMaterial[iContainer][iSubset]->SetDiffuseTexture(strKey);
	}
}

void CDx11Renderer::SetMaterialDiffuseTexFromLoad(const string & strKey,
	TCHAR * pFileName, const string & strPathKey, const UINT& iContainer,
	UINT iSubset)
{
	if (m_vecMaterial.empty())
	{
		CreateMaterial(strKey, pFileName, strPathKey);
	}

	else
	{
		if (m_vecMaterial.size() <= iContainer)
			return;

		else if (m_vecMaterial[iContainer].empty())
		{
			CreateMaterial(strKey, iContainer);
		}

		else if (m_vecMaterial[iContainer].size() <= iSubset)
			return;

		m_vecMaterial[iContainer][iSubset]->SetDiffuseTexture(strKey, pFileName, strPathKey);
	}
}

void CDx11Renderer::AddMultiTex(const string & strKey, const UINT& iContainer,
	UINT iSubset)
{
	if (m_vecMaterial.size() <= iContainer)
		return;

	else if (m_vecMaterial[iContainer].size() <= iSubset)
		return;

	m_vecMaterial[iContainer][iSubset]->AddMultiTex(strKey);
}

void CDx11Renderer::AddMultiTex(const string & strKey, TCHAR * pFileName, 
	const string & strPathKey, const UINT& iContainer,
	UINT iSubset)
{
	if (m_vecMaterial.size() <= iContainer)
		return;

	else if (m_vecMaterial[iContainer].size() <= iSubset)
		return;

	m_vecMaterial[iContainer][iSubset]->AddMultiTex(strKey, pFileName, strPathKey);
}

void CDx11Renderer::DeleteMultiTex(int idx, const UINT& iContainer,
	UINT iSubset)
{
	if (m_vecMaterial.size() <= iContainer)
		return;

	else if (m_vecMaterial[iContainer].size() <= iSubset)
		return;

	m_vecMaterial[iContainer][iSubset]->DeleteMultiTex(idx);
}

void CDx11Renderer::DeleteMultiTex(const string & strTag, const UINT& iContainer,
	UINT iSubset)
{
	if (m_vecMaterial.size() <= iContainer)
		return;

	else if (m_vecMaterial[iContainer].size() <= iSubset)
		return;

	m_vecMaterial[iContainer][iSubset]->DeleteMultiTex(strTag);
}

void CDx11Renderer::DeleteMultiTex(TCHAR * pFileName, const UINT& iContainer,
	UINT iSubset)
{
	if (m_vecMaterial.size() <= iContainer)
		return;

	else if (m_vecMaterial[iContainer].size() <= iSubset)
		return;

	m_vecMaterial[iContainer][iSubset]->DeleteMultiTex(pFileName);
}

void CDx11Renderer::AddMultiTex(CDx11Texture * pTex, const UINT& iContainer,
	UINT iSubset)
{
	if (m_vecMaterial.size() <= iContainer)
		return;

	else if (m_vecMaterial[iContainer].size() <= iSubset)
		return;

	m_vecMaterial[iContainer][iSubset]->AddMultiTex(pTex);
}

void CDx11Renderer::SetRenderState(const string & strKey, 
	RENDERSTATE_FLAG eFlag)
{
	SAFE_RELEASE(m_pRenderState[eFlag]);
	m_pRenderState[eFlag] = DX11_GET_SINGLE(CDx11RenderManager)->FindRS(strKey);
}

bool CDx11Renderer::IsBlendRender()
{
	return m_pRenderState[RSF_BLEND] != NULL;
}

void CDx11Renderer::UpdateTransform()
{
	TRANSFORMCBUFFER	tTransformCBuffer = {};

	CDx11GameObject*	pCameraObj = m_pScene->FindCamera(m_strCameraTag);
	CDx11Camera*	pCamera = (CDx11Camera*)pCameraObj->FindComponentFromTypeName("CDx11Camera");

	tTransformCBuffer.matView = pCamera->GetViewMatrix();
	tTransformCBuffer.matProj = pCamera->GetProjMatrix();

	SAFE_RELEASE(pCamera);
	SAFE_RELEASE(pCameraObj);

	tTransformCBuffer.matWorld = m_pTransform->GetWorldMatrix();
	tTransformCBuffer.matWV = tTransformCBuffer.matWorld *
		tTransformCBuffer.matView;
	tTransformCBuffer.matWVP = tTransformCBuffer.matWV *
		tTransformCBuffer.matProj;

	tTransformCBuffer.matWV = XMMatrixTranspose(tTransformCBuffer.matWV);
	tTransformCBuffer.matWVP = XMMatrixTranspose(tTransformCBuffer.matWVP);
	tTransformCBuffer.matWorld = XMMatrixTranspose(tTransformCBuffer.matWorld);
	tTransformCBuffer.matView = XMMatrixTranspose(tTransformCBuffer.matView);
	tTransformCBuffer.matProj = XMMatrixTranspose(tTransformCBuffer.matProj);

	m_pShader->UpdateConstantBuffer("Transform", &tTransformCBuffer,
		ST_VERTEX);
	m_pShader->UpdateConstantBuffer("Transform", &tTransformCBuffer,
		ST_PIXEL);
}

void CDx11Renderer::UpdateLight()
{
	// 조명정보를 얻어온다.
	const list<CDx11GameObject*>*	pLightList =
		m_pScene->GetLightList();

	list<CDx11GameObject*>::const_iterator	iter;
	list<CDx11GameObject*>::const_iterator	iterEnd = pLightList->end();

	for (iter = pLightList->begin(); iter != iterEnd; ++iter)
	{
		CDx11Light*	pLight = (CDx11Light*)(*iter)->FindComponentFromType(CT_LIGHT);

		pLight->SetConstant(m_pShader);

		SAFE_RELEASE(pLight);
	}
}

void CDx11Renderer::UpdateMaterial()
{
	/*for (size_t i = 0; i < m_vecMaterial.size(); ++i)
	{
		m_vecMaterial[i]->SetMaterial(m_pShader);
	}*/
}

void CDx11Renderer::UpdateRenderState()
{
	for (int i = 0; i < RSF_MAX; ++i)
	{
		if (m_pRenderState[i])
			m_pRenderState[i]->SetState();
	}
}

void CDx11Renderer::UpdateAnimation()
{
	CDx11Animation2D*	pAnimation = (CDx11Animation2D*)m_pGameObject->FindComponentFromType(CT_ANIMATION2D);

	if (pAnimation)
	{
		UVFRAME		tFrame = {};

		tFrame.iFrameX = pAnimation->GetFrameX();
		tFrame.iFrameY = pAnimation->GetFrameY();
		tFrame.iFrameMaxX = pAnimation->GetMaxFrameX();
		tFrame.iFrameMaxY = pAnimation->GetMaxFrameY();
		tFrame.iAniType = pAnimation->GetAnimationType();

		m_pShader->UpdateConstantBuffer("UVFrame", &tFrame, ST_VERTEX);

		SAFE_RELEASE(pAnimation);
	}
}

void CDx11Renderer::ResetRenderState()
{
	for (int i = 0; i < RSF_MAX; ++i)
	{
		if (m_pRenderState[i])
			m_pRenderState[i]->ResetState();
	}
}
