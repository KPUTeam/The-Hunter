#include "InGameScene.h"

#include "Scene/Dx11Scene.h"
#include "GameObject/Dx11GameObject.h"
#include "Component/Dx11Terrain.h"
#include "Scene/Dx11Layer.h"
#include "Component/Dx11Camera.h"
#include "Component/Dx11Transform.h"
#include "Resources/Dx11Mesh.h"
#include "Resources/Dx11ResourcesManager.h"
#include "Component/Dx11Renderer.h"

#include "PlayerInitSC.h"

CInGameScene::CInGameScene()
{
}

CInGameScene::~CInGameScene()
{
}

bool CInGameScene::Init()
{
	CDx11Layer*	pLayer = m_pScene->FindLayer("DefaultLayer");

	if (!pLayer)
		return false;

	// 환경맵 설정
	m_pScene->CreateSky("SkyTex", L"sunsetcube1024.dds");

	// 높이맵 생성
	CDx11GameObject*	pTerrainObj = CDx11GameObject::Create("Terrain");
	CDx11Terrain*		pTerrain = pTerrainObj->AddComponent<CDx11Terrain>("Terrain");

	pTerrain->CreataTerrain("TerrainMesh", 129, 129, "TerrainDiffuse", L"Terrain\\ROCK_01+MOSS.bmp", "Terrain\\Height.bmp", TEXTURE_PATH);
	pLayer->AddObject(pTerrainObj);
	SAFE_RELEASE(pTerrainObj);

	// 조명 생성
	CDx11GameObject*	pLightObj = m_pScene->CreateLight(LT_DIR, "Light");
	CDx11Transform*		pLightTr = pLightObj->GetTransform();
	pLightTr->SetWorldPos(0, 100, 0);
	pLightTr->RotateWorld(DX11_PI / 3.f, 1.f, AXIS_X);
	SAFE_RELEASE(pLightTr);
	SAFE_RELEASE(pLightObj);

	// 플레이어 객체를 생성한다.
	CDx11GameObject*	pPlayer = CDx11GameObject::Create("PlayerObject");
	pLayer->AddObject(pPlayer);

	// 플레이어 스크립트를 추가한다.
	CPlayerInitSC*	pPlayerInitSC = pPlayer->AddComponent<CPlayerInitSC>("PlayerSC");
	SAFE_RELEASE(pPlayerInitSC);
	SAFE_RELEASE(pPlayer);

	SAFE_RELEASE(pLayer);
	return true;
}

void CInGameScene::Input(float fTime)
{
}

int CInGameScene::Update(float fTime)
{
	return 0;
}

int CInGameScene::LateUpdate(float fTime)
{
	return 0;
}

void CInGameScene::Render(float fTime)
{
}

CInGameScene * CInGameScene::Clone()
{
	return new CInGameScene(*this);
}
