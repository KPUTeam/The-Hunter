#include "MainSceneSC.h"

#include "GameObject/Dx11GameObject.h"
#include "Scene/Dx11Scene.h"
#include "Scene/Dx11layer.h"
#include "Component/Dx11Renderer.h"
#include "Component/Dx11Transform.h"
#include "Component/Dx11Camera.h"
#include "Component/Dx11Camera3rd.h"
#include "Component/Dx11Terrain.h"
#include "Core/Dx11Scheduler.h"
#include "Resources/Dx11Mesh.h"
#include "Resources/Dx11ResourcesManager.h"
#include "Scene/Dx11SceneManager.h"
#include "Rendering/Dx11RenderManager.h"
#include "Component/DX11FSM.h"
#include "Core/Dx11Input.h"
#include "Component/Dx11Material.h"

#include "LoadingSceneSC.h"

#include "../Network/NetManager.h"
#include "../ClientCore.h"
#include "../Network/ZEROStream.h"

CMainSceneSC::CMainSceneSC()
{
	m_eState = FADE_IN_START;
	m_fFadeAmount = 0.f;
}

CMainSceneSC::CMainSceneSC(const CMainSceneSC & script)
{
	*this = script;
}


CMainSceneSC::~CMainSceneSC()
{
}

bool CMainSceneSC::Init()
{
	// 서버에 접속한다.
	// 네트워크 매니저 초기화
	if (!GET_SINGLE(CNetManager)->Init(const_cast<char*>(CClientCore::GetInst()->GetIP()),
		CClientCore::GetInst()->GetPort()))
		return false;

	// 접속 처리한다.
	if (!GET_SINGLE(CNetManager)->Login(GET_SINGLE(CClientCore)->GetID(),
		GET_SINGLE(CClientCore)->GetPass()))
		return false;

	// 레이어를 찾는다.
	CDx11Layer*	pLayer = m_pScene->FindLayer("DefaultLayer");

	if (!pLayer)
		return false;

	// 로고 오브젝트 추가
	CDx11GameObject*	pLogoObj = CDx11GameObject::Create("LogoObj", OCT_NORMAL);

	// 로고 오브젝트의 크기를 조정한다.
	CDx11Transform*	pTr = pLogoObj->GetTransform();
	RESOLUTION_TYPE	eRSType = DX11_GET_SINGLE(CDx11Device)->GetResolutionType();
	pTr->Scaling((float)g_tRS[eRSType].iWidth, (float)g_tRS[eRSType].iHeight, 1.f);
	SAFE_RELEASE(pTr);

	// 로고 오브젝트의 렌더러를 설정한다.
	CDx11Renderer*	pRenderer = pLogoObj->AddComponent<CDx11Renderer>("LogoRenderer");
	pRenderer->SetCameraTag("UICamera");
	pRenderer->SetMesh("UIMesh");
	pRenderer->SetShader("UIShader");
	pRenderer->SetRenderState(DX11_RS_DEPTH_DISABLE, RSF_DEPTHSTENCIL);
	pRenderer->CreateMaterial("LogoBack", L"logo.jpg");
	SAFE_RELEASE(pRenderer);

	// 레이어에 추가한다.
	pLayer->AddObject(pLogoObj);
	SAFE_RELEASE(pLogoObj);
	SAFE_RELEASE(pLayer);

	return true;
}

int CMainSceneSC::Update(float fTime)
{
	static bool		bReturn;

	if (GetAsyncKeyState(VK_RETURN) & 0x8000)
	{
		bReturn = true;
	}

	else if (bReturn)
	{
		bReturn = false;
		m_eState = FADE_OUT_START;
	}

	if (m_eState == FADE_OUT_START)
	{
		FadeOut(fTime);
	}
	else if (m_eState == FADE_IN_START)
	{
		FadeIn(fTime);
	}

	if (m_eState == FADE_OUT_END)
	{
		// 메인 씬을 생성한다.
		CDx11Scene*	pScene = DX11_GET_SINGLE(CDx11SceneManager)->CreateScene("LoadingScene");
		CLoadingSceneSc* pMainSceneSC = pScene->AddScript<CLoadingSceneSc>();
		DX11_GET_SINGLE(CDx11SceneManager)->ReplaceScene(pScene);
		SAFE_RELEASE(pMainSceneSC);
	}

	return 0;
}

int CMainSceneSC::LateUpdate(float fTime)
{
	return 0;
}

void CMainSceneSC::Render(float fTime)
{
}

CMainSceneSC * CMainSceneSC::Clone()
{
	return new CMainSceneSC(*this);
}

void CMainSceneSC::FadeOut(float fTime)
{

	if (m_fFadeAmount > 0.f)
	{
		m_fFadeAmount -= fTime;
	}
	else
	{
		m_eState = FADE_OUT_END;
	}

	CDx11Layer*	pLayer = m_pScene->FindLayer("DefaultLayer");

	CDx11GameObject* pRendererObj = pLayer->FindObjectFromTag("LogoObj");
	CDx11Renderer*	pRenderer = (CDx11Renderer*)pRendererObj->FindComponentFromType(CT_RENDERER);
	CDx11Material*	pMtrl = pRenderer->GetMaterial();
	pMtrl->SetFadeAmount(m_fFadeAmount);

	SAFE_RELEASE(pMtrl);
	SAFE_RELEASE(pRenderer);
	SAFE_RELEASE(pRendererObj);
	SAFE_RELEASE(pLayer);
}

void CMainSceneSC::FadeIn(float fTime)
{
	if (m_fFadeAmount < 1.f)
	{
		m_fFadeAmount += fTime;
	}

	else
	{
		m_eState = FADE_IN_END;
		m_fFadeAmount = 1.f;
	}

	CDx11Layer*	pLayer = m_pScene->FindLayer("DefaultLayer");

	CDx11GameObject* pRendererObj = pLayer->FindObjectFromTag("LogoObj");
	CDx11Renderer*	pRenderer = (CDx11Renderer*)pRendererObj->FindComponentFromType(CT_RENDERER);
	CDx11Material*	pMtrl = pRenderer->GetMaterial();
	pMtrl->SetFadeAmount(m_fFadeAmount);

	SAFE_RELEASE(pMtrl);
	SAFE_RELEASE(pRenderer);
	SAFE_RELEASE(pRendererObj);
	SAFE_RELEASE(pLayer);
}
