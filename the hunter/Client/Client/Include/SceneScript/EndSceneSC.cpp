#include "EndSceneSC.h"

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
#include "Component/Dx11UIBar.h"
#include "Component/Dx11BillBoard.h"
#include "Resources/Dx11Texture.h"

#include "InGameSceneSC.h"

#include "../Network/NetManager.h"
#include "../ClientCore.h"
#include "../Network/ZEROStream.h"

CEndSceneSc::CEndSceneSc()
{
}

CEndSceneSc::CEndSceneSc(const CEndSceneSc & script)
{
	*this = script;
}


CEndSceneSc::~CEndSceneSc()
{
	SAFE_RELEASE(pEndLogo);
}

bool CEndSceneSc::Init()
{
	// 레이어를 찾는다.
	CDx11Layer*   pLayer = m_pScene->FindLayer("DefaultLayer");

	CDx11GameObject*   pFrameBar = CDx11GameObject::Create("EndLogo", OCT_NORMAL);

	// 렌더러를 설정한다.
	CDx11Renderer*   pRenderer = pFrameBar->AddComponent<CDx11Renderer>("EndLogo");
	pRenderer->SetCameraTag("UICamera");
	pRenderer->SetMesh("UIMesh");
	pRenderer->SetShader("UIShader");
	pRenderer->SetRenderState(DX11_RS_DEPTH_DISABLE, RSF_DEPTHSTENCIL);
	pRenderer->CreateMaterial("EndLogo", L"end.png");
	SAFE_RELEASE(pRenderer);

	// 크기를 조정한다.
	CDx11Transform*   pTr = pFrameBar->GetTransform();
	RESOLUTION_TYPE   eRSType = DX11_GET_SINGLE(CDx11Device)->GetResolutionType();
	pTr->Scaling((float)g_tRS[eRSType].iWidth, (float)g_tRS[eRSType].iHeight, 1.f);
	pTr->SetWorldPos(0, 0, 1.f);
	SAFE_RELEASE(pTr);

	// 레이어에 추가한다.
	pLayer->AddObject(pFrameBar);

	SAFE_RELEASE(pFrameBar);

	SAFE_RELEASE(pLayer);

//	DX11_GET_SINGLE(CDx11Scheduler)->CreateSchedule(this, &CEndSceneSc::Exit, 10.f, SO_ONCE);

	return true;
}

int CEndSceneSc::Update(float fTime)
{
	static bool		bReturn;

	if (GetAsyncKeyState(VK_RETURN) & 0x8000)
	{
		Exit(1);
	}

	return 0;
}

int CEndSceneSc::LateUpdate(float fTime)
{
	return 0;
}

void CEndSceneSc::Render(float fTime)
{
}

CEndSceneSc * CEndSceneSc::Clone()
{
	return new CEndSceneSc(*this);
}

int CEndSceneSc::Exit(float fTime)
{
	Exit(fTime);
	return 0;
}
