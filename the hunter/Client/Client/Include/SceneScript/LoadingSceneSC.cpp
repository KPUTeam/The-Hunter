#include "LoadingSceneSC.h"

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

CLoadingSceneSc::CLoadingSceneSc():
	pLoadingBar(NULL),
	pLoadngBarTr(NULL)
{
}

CLoadingSceneSc::CLoadingSceneSc(const CLoadingSceneSc & script)
{
	*this = script;
}


CLoadingSceneSc::~CLoadingSceneSc()
{
	SAFE_RELEASE(pLoadingBar);
	SAFE_RELEASE(pLoadngBarTr);
}

void CLoadingSceneSc::CreateFrameBar()
{
	// 레이어를 찾는다.
	CDx11Layer*	pLayer = m_pScene->FindLayer("DefaultLayer");

	CDx11GameObject*	pFrameBar = CDx11GameObject::Create("pLoadingBar2", OCT_NORMAL);

	// 렌더러를 설정한다.
	CDx11Renderer*	pRenderer = pFrameBar->AddComponent<CDx11Renderer>("pLoadingBarRenderer2");
	pRenderer->SetCameraTag("UICamera");
	pRenderer->SetMesh("UIMesh");
	pRenderer->SetShader("UIShader");
	pRenderer->SetRenderState(DX11_RS_DEPTH_DISABLE, RSF_DEPTHSTENCIL);
	pRenderer->CreateMaterial("pLoadingBar2", L"Loading/LoadingFrame.png");
	SAFE_RELEASE(pRenderer);

	// 크기를 조정한다.
	CDx11Transform*	pTr = pFrameBar->GetTransform();
	RESOLUTION_TYPE	eRSType = DX11_GET_SINGLE(CDx11Device)->GetResolutionType();
	pTr->Scaling((float)g_tRS[eRSType].iWidth/3, (float)g_tRS[eRSType].iHeight/3, 1.f);
	pTr->SetWorldPos((float)g_tRS[eRSType].iWidth/3, (float)g_tRS[eRSType].iHeight/3, 1.f);
	SAFE_RELEASE(pTr);

	// 레이어에 추가한다.
	pLayer->AddObject(pFrameBar);

	SAFE_RELEASE(pFrameBar);
	SAFE_RELEASE(pLayer);
}

void CLoadingSceneSc::CreateLoadingBar()
{
	// 레이어를 찾는다.
	CDx11Layer*	pLayer = m_pScene->FindLayer("DefaultLayer");

	pLoadingBar = CDx11GameObject::Create("pLoadingBar", OCT_NORMAL);

	// 렌더러를 설정한다.
	CDx11Renderer*	pRenderer = pLoadingBar->AddComponent<CDx11Renderer>("pLoadingBarRenderer");
	pRenderer->SetCameraTag("UICamera");
	pRenderer->SetMesh("UIMesh");
	pRenderer->SetShader("UIShader");
	pRenderer->SetRenderState(DX11_RS_ALPHABLEND, RSF_BLEND);
	pRenderer->SetRenderState(DX11_RS_DEPTH_DISABLE, RSF_DEPTHSTENCIL);
	pRenderer->CreateMaterial("LoadingBar", L"Loading/LoadingBar.png");
	SAFE_RELEASE(pRenderer);


	// 레이어에 추가한다.
	pLayer->AddObject(pLoadingBar);

	SAFE_RELEASE(pLayer);
}

bool CLoadingSceneSc::Init()
{

	// 레이어를 찾는다.
	CDx11Layer*	pLayer = m_pScene->FindLayer("DefaultLayer");

	if (!pLayer)
		return false;

	CreateFrameBar();
	CreateLoadingBar();

	SAFE_RELEASE(pLayer);

	// 스프라이트 로드
	for (int i = 1; i <= 75; ++i)
	{
		char	strKey[256] = {};
		sprintf_s(strKey, "explosion%d", i);
		wchar_t	strPath[MAX_PATH] = {};
		wsprintf(strPath, L"Explosion/%d.PNG", i);
		CDx11Texture*	pTex = DX11_GET_SINGLE(CDx11ResourcesManager)->LoadTexture(strKey, strPath);
		SAFE_RELEASE(pTex);
	}

	// 에프비엑스 추가
	strKey.push_back("Skeleton"); cFileName.push_back(L"Skeleton.FBX");
	strKey.push_back("bBody"); cFileName.push_back(L"bBody.FBX");
	strKey.push_back("bHead"); cFileName.push_back(L"bHead.FBX");
	strKey.push_back("rBody"); cFileName.push_back(L"rBody.FBX");
	strKey.push_back("rHead"); cFileName.push_back(L"rHead.FBX");
	strKey.push_back("gBody"); cFileName.push_back(L"gBody.FBX");
	strKey.push_back("gHead"); cFileName.push_back(L"gHead.FBX");
	strKey.push_back("yBody"); cFileName.push_back(L"yBody.FBX");
	strKey.push_back("yHead"); cFileName.push_back(L"yHead.FBX");
	strKey.push_back("Wheel"); cFileName.push_back(L"Wheel.FBX");
	strKey.push_back("Bush1"); cFileName.push_back(L"Bush1.FBX");
	strKey.push_back("Root1"); cFileName.push_back(L"Root1.FBX");
	strKey.push_back("Grass1"); cFileName.push_back(L"Grass1.FBX");
	strKey.push_back("Leaves"); cFileName.push_back(L"Leaves.FBX");
	strKey.push_back("Grass2"); cFileName.push_back(L"Grass2.FBX");
	strKey.push_back("CampFire"); cFileName.push_back(L"CampFire.FBX");

	// 선인장
	strKey.push_back("Cactus2"); cFileName.push_back(L"Cactus2.FBX");
	strKey.push_back("Cactus3"); cFileName.push_back(L"Cactus3.FBX");
	strKey.push_back("Cactus_Short1"); cFileName.push_back(L"Cactus_Short1.FBX");
	strKey.push_back("Cactus_Short2"); cFileName.push_back(L"Cactus_Short2.FBX");
	strKey.push_back("Cactus_Short3"); cFileName.push_back(L"Cactus_Short3.FBX");
	strKey.push_back("Leafy1"); cFileName.push_back(L"Leafy1.FBX");
	strKey.push_back("Leafy2"); cFileName.push_back(L"Leafy2.FBX");
	strKey.push_back("Leafy4"); cFileName.push_back(L"Leafy4.FBX");

	// 바위
	strKey.push_back("Rock_Cone"); cFileName.push_back(L"Rock_Cone.FBX");
	strKey.push_back("Rock_Heavy"); cFileName.push_back(L"Rock_Heavy.FBX");
	strKey.push_back("Rock_Large"); cFileName.push_back(L"Rock_Large.FBX");
	strKey.push_back("Rock_Small"); cFileName.push_back(L"Rock_Small.FBX");

	// 이터레이터
	iterKey = strKey.begin();
	iterFileName = cFileName.begin();

	// 트랜스폼을 저장한다.
	pLoadngBarTr = pLoadingBar->GetTransform();

	// 초기 크기를 설정한다. 
	RESOLUTION_TYPE	eRSType = DX11_GET_SINGLE(CDx11Device)->GetResolutionType();
	pLoadngBarTr->SetWorldPos((float)g_tRS[eRSType].iWidth / 3, (float)g_tRS[eRSType].iHeight / 3, 1.f);
	pLoadngBarTr->Scaling(0, (float)g_tRS[eRSType].iHeight / 3, 1.f);

	return true;
}

int CLoadingSceneSc::Update(float fTime)
{
	// 에프비엑스 로드
	if (iterKey != strKey.end()) 
	{
		CDx11Mesh* pMesh =
			DX11_GET_SINGLE(CDx11ResourcesManager)->LoadMesh(*iterKey, *iterFileName, DX11_MESH_PATH);

		SAFE_RELEASE(pMesh);

		RESOLUTION_TYPE	eRSType = DX11_GET_SINGLE(CDx11Device)->GetResolutionType();

		float fWidthSize = (float)g_tRS[eRSType].iWidth / 3 * (++fBarScale / strKey. size());
		pLoadngBarTr->Scaling(fWidthSize, (float)g_tRS[eRSType].iHeight / 3, 1.f);
	
		iterKey++;
		iterFileName++;
	}

	else 
	{
		// 인게임 씬을 생성한다.
		CDx11Scene*	pScene = DX11_GET_SINGLE(CDx11SceneManager)->CreateScene("InGameScene");
		CInGameSceneSC* pInGameSceneSC = pScene->AddScript<CInGameSceneSC>();
		CNetManager::GetInst()->GetInGameNetwork()->SetInGameScene(pInGameSceneSC);
		DX11_GET_SINGLE(CDx11SceneManager)->ReplaceScene(pScene);
		SAFE_RELEASE(pInGameSceneSC);
	}

	return 0;
}

int CLoadingSceneSc::LateUpdate(float fTime)
{
	return 0;
}

void CLoadingSceneSc::Render(float fTime)
{
}

CLoadingSceneSc * CLoadingSceneSc::Clone()
{
	return new CLoadingSceneSc(*this);
}