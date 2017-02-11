#include "Dx11Renderer.h"
#include "../Resources/Dx11Mesh.h"
#include "../Rendering/Dx11Shader.h"
#include "../Resources/Dx11ResourcesManager.h"
#include "../Rendering/Dx11ShaderManager.h"
#include "../Scene/Dx11Scene.h"
#include "Dx11Camera.h"
#include "Dx11Transform.h"
#include "../GameObject/Dx11GameObject.h"
#include "Dx11Material.h"
#include "Dx11Light.h"
#include "../Rendering/Dx11RenderState.h"
#include "../Rendering/Dx11RenderManager.h"

DX11_USING

CDx11Renderer::CDx11Renderer()	:
	m_pMesh(NULL),
	m_pShader(NULL),
	m_pMaterial(NULL)
{
	m_eComType = CT_RENDERER;
	memset(m_pRenderState, 0, sizeof(CDx11RenderState*) * RST_END);
}

CDx11Renderer::CDx11Renderer(const CDx11Renderer & renderer)	:
	CDx11Component(renderer)
{
	*this = renderer;
	m_iRefCount = 1;

	if (m_pShader)
		m_pShader->AddRef();

	if (m_pMesh)
		m_pMesh->AddRef();

	if (m_pMaterial)
		m_pMaterial->AddRef();

	for (int i = 0; i < RST_END; ++i)
	{
		if (m_pRenderState[i])
			m_pRenderState[i]->AddRef();
	}
}

CDx11Renderer::~CDx11Renderer()
{
	for (int i = 0; i < RST_END; ++i)
	{
		SAFE_RELEASE(m_pRenderState[i]);
	}

	SAFE_RELEASE(m_pMaterial);
	SAFE_RELEASE(m_pMesh);
	SAFE_RELEASE(m_pShader);
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
	SetTransformCBuffer();

	SetLightCBuffer();

	SetMaterialCBuffer();

	// 렌더상태 설정
	for (int i = 0; i < RST_END; ++i)
	{
		if (m_pRenderState[i])
			m_pRenderState[i]->SetState();
	}

	m_pShader->SetShader();
	m_pMesh->Render();

	for (int i = 0; i < RST_END; ++i)
	{
		if (m_pRenderState[i])
			m_pRenderState[i]->ResetState();
	}

	m_pShader->End();
}

CDx11Renderer * CDx11Renderer::Clone()
{
	return new CDx11Renderer(*this);
}

void CDx11Renderer::SetMesh(const string & strKey)
{
	SAFE_RELEASE(m_pMesh);
	m_pMesh = DX11_GET_SINGLE(CDx11ResourcesManager)->FindMesh(strKey);
}

void CDx11Renderer::SetShader(const string & strKey)
{
	SAFE_RELEASE(m_pShader);
	m_pShader = DX11_GET_SINGLE(CDx11ShaderManager)->FindShader(strKey);
}

void CDx11Renderer::CreateMaterial(const string & strDiffuseTexKey,
	const string & strDiffuseSampKey)
{
	SAFE_RELEASE(m_pMaterial);
	m_pMaterial = new CDx11Material;

	m_pMaterial->Init();

	m_pMaterial->SetDiffuseTexture(strDiffuseTexKey);
	m_pMaterial->SetDiffuseSampler(strDiffuseSampKey);
}

void CDx11Renderer::CreateMaterial(const string & strDiffuseTexKey, 
	TCHAR * pFileName, const string & strPathKey, 
	const string & strDiffuseSampKey)
{
	SAFE_RELEASE(m_pMaterial);
	m_pMaterial = new CDx11Material;

	m_pMaterial->Init();

	m_pMaterial->SetDiffuseTexture(strDiffuseTexKey, pFileName, strPathKey);
	m_pMaterial->SetDiffuseSampler(strDiffuseSampKey);
}

void CDx11Renderer::SetRenderState(const string & strKey)
{
	CDx11RenderState*	pRS = DX11_GET_SINGLE(CDx11RenderManager)->FindRenderState(strKey);

	if (!pRS)
		return;

	SAFE_RELEASE(m_pRenderState[pRS->GetStateType()]);
	m_pRenderState[pRS->GetStateType()] = pRS;
}

void CDx11Renderer::SetTransformCBuffer()
{
	TRANSFORMCBUFFER	tBuffer = {};

	// 카메라의 뷰와 투영 행렬을 얻어온다.
	CDx11GameObject*	pCameraObj = m_pScene->GetMainCamera();
	CDx11Camera*	pCamera = (CDx11Camera*)pCameraObj->FindComponentFromTypeName("CDx11Camera");
	tBuffer.matView = pCamera->GetViewMatrix();
	tBuffer.matProj = pCamera->GetProjMatrix();

	SAFE_RELEASE(pCamera);
	SAFE_RELEASE(pCameraObj);

	tBuffer.matWorld = m_pTransform->GetWorldMatrix();

	tBuffer.matWV = tBuffer.matWorld * tBuffer.matView;
	tBuffer.matWVP = tBuffer.matWV * tBuffer.matProj;

	tBuffer.matWorld = XMMatrixTranspose(tBuffer.matWorld);
	tBuffer.matView = XMMatrixTranspose(tBuffer.matView);
	tBuffer.matProj = XMMatrixTranspose(tBuffer.matProj);

	tBuffer.matWV = XMMatrixTranspose(tBuffer.matWV);
	tBuffer.matWVP = XMMatrixTranspose(tBuffer.matWVP);

	m_pShader->UpdateConstantBuffer("Transform", &tBuffer, ST_VERTEX);
	m_pShader->UpdateConstantBuffer("Transform", &tBuffer, ST_PIXEL);
}

void CDx11Renderer::SetLightCBuffer()
{
	list<CDx11GameObject*>*	pLightList = m_pScene->GetLightList();

	list<CDx11GameObject*>::iterator	iter;
	list<CDx11GameObject*>::iterator	iterEnd = pLightList->end();

	for (iter = pLightList->begin(); iter != iterEnd; ++iter)
	{
		CDx11Light*	pLight = (CDx11Light*)(*iter)->FindComponentFromType(CT_LIGHT);

		pLight->SetLight(m_pShader);

		SAFE_RELEASE(pLight);
	}
}

void CDx11Renderer::SetMaterialCBuffer()
{
	if (m_pMaterial)
		m_pMaterial->SetMaterial(m_pShader);
}
