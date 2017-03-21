#include "LogoScene.h"
#include "MainScene.h"
#include "Core/Dx11Device.h"
#include "Resources/Dx11ResourcesManager.h"
#include "Resources/Dx11Texture.h"
#include "Scene/Dx11Layer.h"
#include "Scene/Dx11Scene.h"
#include "GameObject/Dx11GameObject.h"
#include "Component/Dx11Transform.h"
#include "Component/Dx11Renderer.h"
#include "Component/Dx11Material.h"
#include "Component/Dx11UIBack.h"

CLogoScene::CLogoScene()
{
}


CLogoScene::~CLogoScene()
{
}

bool CLogoScene::Init()
{
	CDx11Layer*	pLayer = m_pScene->FindLayer("UILayer");

	if (!pLayer)
		return false;

	CDx11GameObject*	pBack = CDx11GameObject::Create("BackObj");
	CDx11Transform*	pTransform = pBack->GetTransform();

	RESOLUTION_TYPE	eType = DX11_RESOLUTION;
	pTransform->SetWorldScale(g_tRS[eType].iWidth, g_tRS[eType].iHeight, 1.f);

	SAFE_RELEASE(pTransform);

	// 렌더러를 생성한다.
	CDx11Renderer*	pRenderer = pBack->AddComponent<CDx11Renderer>("BackRenderer");

	pRenderer->SetMesh(RECT_UI_MESH);
	pRenderer->SetShader(UI_SHADER);

	// 재질을 설정한다.
	pRenderer->CreateMaterial("LogoBack", L"Logo.png");

	SAFE_RELEASE(pRenderer);

	CDx11UIBack*	pUIBack = pBack->AddComponent<CDx11UIBack>("UIBack");

	SAFE_RELEASE(pUIBack);

	pLayer->AddObject(pBack);

	SAFE_RELEASE(pBack);

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
