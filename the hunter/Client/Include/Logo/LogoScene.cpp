#include "LogoScene.h"

#include "Scene/Dx11Scene.h"
#include "Scene/Dx11Layer.h"

#include "GameObject/Dx11GameObject.h"
#include "Component/Dx11Transform.h"
#include "Core/Dx11Device.h"

#include "Component/Dx11Renderer.h"
#include "Component/Dx11UIBackground.h"

#include "Core/Dx11Scheduler.h"

#include "Scene/Dx11SceneManager.h"

#include "../InGame/InGameScene.h"

CLogoScene::CLogoScene()
{
}

CLogoScene::~CLogoScene()
{
}

bool CLogoScene::Init()
{
	/**/
	CDx11Layer*	pLayer = m_pScene->FindLayer("UILayer");

	if (!pLayer)
		return false;

	CDx11GameObject*	pBackOBJ = CDx11GameObject::Create("BackOBJ");

	// 레이어에 추가한다.
	pLayer->AddObject(pBackOBJ);

	// 화면 크기만크 확대한다.
	CDx11Transform*	pTransform = pBackOBJ->GetTransform();
	RESOLUTION_TYPE	eType = DX11_RESOLUTION;
	pTransform->Scaling(g_tRS[eType].iWidth, g_tRS[eType].iHeight, 1.f);
	SAFE_RELEASE(pTransform);

	// 렌더러를 생성한다.
	CDx11Renderer*	pRenderer = pBackOBJ->AddComponent<CDx11Renderer>("BackRenderer");
	pRenderer->SetMesh(RECT_UI_MESH);
	pRenderer->SetShader(UI_SHADER);
	pRenderer->CreateMaterial("Logo", L"Logo.png");
	pRenderer->SetRenderState(DX11_RS_DEPTH_DISABLE);
	SAFE_RELEASE(pRenderer);

	// UI 백그라운드
	CDx11UIBackground*	pUIBack = pBackOBJ->AddComponent<CDx11UIBackground>("UIBackground");
	SAFE_RELEASE(pUIBack);

	// 스케줄
	DX11_GET_SINGLE(CDx11Scheduler)->CreateSchedule(this, &CLogoScene::CreateMainScene, 2.f, SO_ONCE);

	SAFE_RELEASE(pBackOBJ);
	SAFE_RELEASE(pLayer);

	return true;
}

void CLogoScene::Input(float fTime)
{
}

int CLogoScene::Update(float fTime)
{
	return 0;
}

int CLogoScene::LateUpdate(float fTime)
{
	return 0;
}

void CLogoScene::Render(float fTime)
{
}

CLogoScene * CLogoScene::Clone()
{
	return new CLogoScene(*this);
}

int CLogoScene::CreateMainScene(float fTiem)
{
	CDx11Scene*	pScene = DX11_GET_SINGLE(CDx11SceneManager)->CreateScene("InGameScene");

	CInGameScene*	pScnenScript = pScene->CreateSceneScript<CInGameScene>("InGameScene");
	SAFE_RELEASE(pScnenScript);

	DX11_GET_SINGLE(CDx11SceneManager)->ChangeScene(pScene);

	return 0;
}
