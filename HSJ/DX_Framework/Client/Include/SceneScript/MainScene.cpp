#include "MainScene.h"

// Core
#include "Core/Dx11Scheduler.h"
#include "Core/Dx11Input.h"

// Scene
#include "Scene/Dx11Scene.h"
#include "Scene/Dx11Layer.h"

// GameObject
#include "GameObject/Dx11GameObject.h"
#include "Component/Dx11Transform.h"
#include "Component/Dx11Renderer.h"
#include "Component/Dx11Camera.h"

// Resources
#include "Resources/Dx11ResourcesManager.h"
#include "Resources/Dx11Texture.h"
#include "Resources/Dx11Mesh.h"

// Client
#include "../ObjectScript/Player.h"
#include "../ObjectScript/Monster.h"
#include "../ObjectScript/Bullet.h"

CMainScene::CMainScene()
{
}

CMainScene::~CMainScene()
{
}

bool CMainScene::Init()
{
	// 환경맵 설정
	m_pScene->CreateSky("SkyTex", L"DesertSky.dds");

	CreateBottom();
	CreateMonster();
	CreatePlayer();
	CreateBullet();

	DX11_GET_SINGLE(CDx11Scheduler)->CreateSchedule(this, &CMainScene::CreateMonster_Clone, 3.f, SO_LOOP);

	return true;
}


void CMainScene::Input(float fTime)
{
}

int CMainScene::Update(float fTime)
{
	return 0;
}

int CMainScene::LateUpdate(float fTime)
{
	return 0;
}

void CMainScene::Render(float fTime)
{
}

CMainScene * CMainScene::Clone()
{
	return new CMainScene(*this);
}

bool CMainScene::CreateBottom()
{
	float x = 20.f;

	CDx11Layer*	pLayer = m_pScene->FindLayer("DefaultLayer");

	if (!pLayer)
		return false;
	 
	// 매쉬 생성
	TEXVERTEX	tTexVtx[4] =
	{
		{ Vec3(-x, 0, x), Vec3(0.f, 1.f, 0.f), Vec2(0.f, 0.f) },
		{ Vec3(x, 0, x), Vec3(0.f, 1.f, 0.f), Vec2(1.f, 0.f) },
		{ Vec3(-x, 0,-x), Vec3(0.f, 1.f, 0.f), Vec2(0.f, 1.f) },
		{ Vec3(x, 0, -x), Vec3(0.f, 1.f, 0.f), Vec2(1.f, 1.f) }
	};

	UINT	iIndex[6] = { 0, 1, 3, 0, 3, 2 };

	CDx11Mesh* pMesh = DX11_GET_SINGLE(CDx11ResourcesManager)->
		CreateMesh("GrassMesh", sizeof(TEXVERTEX), 4, tTexVtx, sizeof(UINT), 6, iIndex, DXGI_FORMAT_R32_UINT);
	SAFE_RELEASE(pMesh);

	CDx11GameObject* pBottom = CDx11GameObject::Create("GrassObject");

	// 샘플 텍스쳐 로딩
	CDx11Texture* pTex = DX11_GET_SINGLE(CDx11ResourcesManager)->LoadTexture("GrassTex", L"grass.png");
	SAFE_RELEASE(pTex);

	// 렌더러를 생성한다.
	CDx11Renderer* pRenderer = pBottom->AddComponent<CDx11Renderer>("GrassRenderer");
	pRenderer->SetMesh("GrassMesh");
	pRenderer->SetShader(DEFAULT_TEX_SHADER);
	pRenderer->CreateMaterial("GrassTex", DEFAULT_SAMPLER);

	pLayer->AddObject(pBottom);
	SAFE_RELEASE(pBottom);
	SAFE_RELEASE(pRenderer);
	SAFE_RELEASE(pLayer);
	return true;
}

bool CMainScene::CreatePlayer()
{

	CDx11Layer*	pLayer = m_pScene->FindLayer("DefaultLayer");

	if (!pLayer)
		return false;

	CDx11GameObject*	pPlayer = CDx11GameObject::Create("PlayerObject");
	CPlayer*	pPlayerScript = pPlayer->AddComponent<CPlayer>("PlayerScript");

	CDx11GameObject* pChild = CDx11GameObject::Create("ChildObjecrt");
	pPlayer->AddChild(pChild);

	CDx11Renderer* pRenderer = pChild->AddComponent<CDx11Renderer>("ChildRenderer");
	pRenderer->SetMesh("PyramidMesh");
	pRenderer->SetShader(DEFAULT_SHADER);

	CDx11Transform* tr = pChild->GetTransform();
	tr->DeleteParentFlag(TPF_ROT);
	tr->MoveWorld(3, 1.f, AXIS_X);

	SAFE_RELEASE(tr);
	SAFE_RELEASE(pRenderer);
	SAFE_RELEASE(pChild);

	pLayer->AddObject(pPlayer);

	SAFE_RELEASE(pPlayerScript);
	SAFE_RELEASE(pPlayer);
	SAFE_RELEASE(pLayer);

	return true;
}

bool CMainScene::CreateMonster()
{
	CDx11GameObject*	pMonster = CDx11GameObject::Create("MonsterObject", OT_PROTOTYPE);
	CMonster*	pMonsterScript = pMonster->AddComponent<CMonster>("MonsterScript");

	SAFE_RELEASE(pMonsterScript);
	SAFE_RELEASE(pMonster);
	return true;
}

bool CMainScene::CreateBullet()
{
	 CDx11Layer*	pLayer = m_pScene->FindLayer("DefaultLayer");

	if (!pLayer)
		return false;

	CDx11GameObject* pBullet = CDx11GameObject::Create("BulletObject",OT_PROTOTYPE);
	CDx11Renderer* pRenderer = pBullet->AddComponent<CDx11Renderer>("BulletRenderer");
	pRenderer->SetMesh("PyramidMesh");
	pRenderer->SetShader(DEFAULT_SHADER);

	CBullet* pBulletScript = pBullet->AddComponent<CBullet>("BulletScript");

	SAFE_RELEASE(pBullet);
	SAFE_RELEASE(pBulletScript);
	SAFE_RELEASE(pRenderer);
	SAFE_RELEASE(pLayer);

	return true;
}

int CMainScene::CreateMonster_Clone(float fTime)
{
	CDx11Layer*	pLayer = m_pScene->FindLayer("DefaultLayer");

	if (!pLayer)
		return false;

	CDx11GameObject*	pMonster = CDx11GameObject::Create("MonsterObject", OT_CLONE);
	CDx11Transform*	pTransform = pMonster->GetTransform();

	pTransform->SetWorldPos((rand() % 1001) / 100.f - 5.f, (rand() % 1001) / 100.f + 5.f, (rand() % 501) / 100.f);

	SAFE_RELEASE(pTransform);

	pLayer->AddObject(pMonster);
	SAFE_RELEASE(pMonster);
	SAFE_RELEASE(pLayer);

	return 0;
}
