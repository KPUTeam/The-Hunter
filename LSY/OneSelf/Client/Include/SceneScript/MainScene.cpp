#include "MainScene.h"
#include "GameObject/Dx11GameObject.h"
#include "Component/Dx11Transform.h"
#include "Component/Dx11Renderer.h"
#include "Scene/Dx11Scene.h"
#include "Scene/Dx11Layer.h"
#include "../ObjectScript/Player.h"
#include "../ObjectScript/Bullet.h"
#include "../ObjectScript/Monster.h"
#include "../ObjectScript/Skill2.h"
#include "../ObjectScript/InvisibleObject.h"
#include "Core/Dx11Scheduler.h"
#include "Resources/Dx11ResourcesManager.h"
#include "Resources/Dx11Texture.h"
#include "Component/Dx11Material.h"


CMainScene::CMainScene()
{
}

CMainScene::~CMainScene()
{
}

bool CMainScene::Init()
{
	if (!CreatePlayer())
		return false;

	if (!CreateBullet())
		return false;

	if (!CreateChild())
		return false;

	if (!CreateCamera())
		return false;

	if (!CreateMonster())
		return false;

	if (!CreateSkill())
		return false;

	if (!CreateSky())
		return false;

	if (!CreateInvisible())
		return false;

	DX11_GET_SINGLE(CDx11Scheduler)->CreateSchedule(this, &CMainScene::CreateMonster,
		3.f, SO_LOOP);

	return true;
}

void CMainScene::Input(float fTime)
{
	
}

int CMainScene::Update(float fTime)
{
	/*CDx11GameObject*	pCamera = m_pScene->GetMainCamera();
	CDx11Transform*	pTr = pCamera->GetTransform();

	if (GetAsyncKeyState(VK_UP) & 0x8000)
	{
		pTr->MoveWorld(10.f, fTime, AXIS_Z);
	}

	if (GetAsyncKeyState(VK_DOWN) & 0x8000)
	{
		pTr->MoveWorld(-10.f, fTime, AXIS_Z);
	}

	if (GetAsyncKeyState(VK_LEFT) & 0x8000)
	{
		pTr->RotationWorld(-DX11_PI, fTime, AXIS_X);
	}

	if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
	{
		pTr->RotationWorld(DX11_PI, fTime, AXIS_X);
	}*/

	static string	str = "SubCamera1";
	static bool		bReturn;

	if (GetAsyncKeyState(VK_RETURN) & 0x8000)
	{
		bReturn = true;
	}

	else if (bReturn)
	{
		bReturn = false;
		m_pScene->ChangeCamera(str);

		if (str == "SubCamera1")
			str = "MainCamera";

		else
			str = "SubCamera1";
	}

//	SAFE_RELEASE(pTr);
//	SAFE_RELEASE(pCamera);

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

bool CMainScene::CreatePlayer()
{
	CDx11Layer*	pLayer = m_pScene->FindLayer("DefaultLayer");

	if (!pLayer)
		return false;

	CDx11GameObject*	pPlayer = CDx11GameObject::Create("PlayerObject");
	
	CPlayer*	pPlayerScript = pPlayer->AddComponent<CPlayer>("Player");
	SAFE_RELEASE(pPlayerScript);

	pLayer->AddObject(pPlayer);

	SAFE_RELEASE(pPlayer);
	SAFE_RELEASE(pLayer);

	return true;
}

bool CMainScene::CreateBullet()
{
	CDx11Layer*	pLayer = m_pScene->FindLayer("DefaultLayer");

	if (!pLayer)
		return false;

	// 총알 프로토타입을 만든다.
	CDx11GameObject*	pBullet = CDx11GameObject::Create("BulletPrototype", OT_PROTOTYPE);

	// 렌더러를 생성한다.
	CDx11Renderer*	pRenderer = pBullet->AddComponent<CDx11Renderer>("BulletRenderer");

	pRenderer->SetMesh(TRIANGLE_MESH);
	pRenderer->SetShader(DEFAULT_SHADER);

	SAFE_RELEASE(pRenderer);

	CBullet*	pBulletSc = (CBullet*)pBullet->AddComponent<CBullet>("BulletScript");

	SAFE_RELEASE(pBulletSc);
	SAFE_RELEASE(pLayer);

	return true;
}

bool CMainScene::CreateChild()
{
	CDx11Layer*	pLayer = m_pScene->FindLayer("DefaultLayer");// pLayer는 디폴트 레이어를 찾는다.
															 /////////////////
	CDx11GameObject* pPlayer = pLayer->FindObjectFromTag("PlayerObject");

//	CDx11GameObject* pPlayer = pLayer->FindObjectFromTag("Invisible");

	//	CDx11GameObject*	pPlayerChild = CDx11GameObject::Create("PlayerChildObject");

	CDx11GameObject*	pPlayerChild = CDx11GameObject::Create("LeftChild");

	CDx11Renderer*	pRenderer = pPlayerChild->AddComponent<CDx11Renderer>("PlayerChildRenderer");

	pRenderer->SetMesh(RECT_TEX_MESH);
	pRenderer->SetShader(DEFAULT_TEX_SHADER);
	pRenderer->CreateMaterial("ChildPlayer", L"Child.png");
	pRenderer->SetRenderState(DX11_RS_ALPHABLEND);

	SAFE_RELEASE(pRenderer);

	CDx11Transform*	pChildTr = pPlayerChild->GetTransform();
	pChildTr->MoveWorld(-1.f, 1.f, AXIS_X);
	pChildTr->SetWorldScale(0.5f, 0.5f, 1.f);

	SAFE_RELEASE(pChildTr);


	pPlayer->AddChild(pPlayerChild);
	SAFE_RELEASE(pPlayerChild);
	SAFE_RELEASE(pPlayer);
	///////////////////
	pPlayer = pLayer->FindObjectFromTag("PlayerObject");

	pPlayerChild = CDx11GameObject::Create("RightChild");

	pRenderer = pPlayerChild->AddComponent<CDx11Renderer>("PlayerChildRenderer");

	pRenderer->SetMesh(RECT_TEX_MESH);
	pRenderer->SetShader(DEFAULT_TEX_SHADER);
	pRenderer->CreateMaterial("ChildPlayer", L"Child.png");
	pRenderer->SetRenderState(DX11_RS_ALPHABLEND);

	SAFE_RELEASE(pRenderer);

	pChildTr = pPlayerChild->GetTransform();
	pChildTr->MoveWorld(1.f, 1.f, AXIS_X);
	pChildTr->SetWorldScale(0.5f, 0.5f, 1.f);

	SAFE_RELEASE(pChildTr);

	pPlayer->AddChild(pPlayerChild);
	SAFE_RELEASE(pPlayerChild);
	SAFE_RELEASE(pPlayer);
	SAFE_RELEASE(pLayer);

	return true;
}

bool CMainScene::CreateCamera()
{
	CDx11GameObject*	pCameraObj = m_pScene->CreateCamera("SubCamera1",
		DX11_PI / 3.f, 0.1f, 1000.f);

	CDx11Transform*	pCamTr = pCameraObj->GetTransform();

	pCamTr->SetWorldPos(5.f, 0.f, -10.f);
	pCamTr->SetWorldRot(0.f, -DX11_PI / 4.f, DX11_PI / 6.f);

	SAFE_RELEASE(pCamTr);
	SAFE_RELEASE(pCameraObj);

	return true;
}

bool CMainScene::CreateMonster()
{
	CDx11GameObject*	p_Monster = CDx11GameObject::Create("MonsterPrototype", OT_PROTOTYPE);

	CDx11Renderer*	pRenderer = p_Monster->AddComponent<CDx11Renderer>("MonsterRenderer");

	pRenderer->SetMesh(TRIANGLE_MESH);
	pRenderer->SetShader(DEFAULT_SHADER);

	SAFE_RELEASE(pRenderer);

	CDx11Transform*	pTransform = p_Monster->GetTransform();

	pTransform->MoveWorld(-5.f, 1.f, AXIS_X);

	SAFE_RELEASE(pTransform);

	CDx11Layer* pLayer = m_pScene->FindLayer("DefaultLayer");

	if (!pLayer)
		return false;

	pLayer->AddObject(p_Monster);

	CMonster*	pMonsterSc = (CMonster*)p_Monster->AddComponent<CMonster>("MonsterScript");

	SAFE_RELEASE(p_Monster);
	SAFE_RELEASE(pMonsterSc);
	SAFE_RELEASE(pLayer);

	return true;
}

bool CMainScene::CreateSkill()
{
	// 스킬2번 총알 프로토타입을 만든다.
	CDx11GameObject*	pSkill2 = CDx11GameObject::Create("Skill2Prototype", OT_PROTOTYPE);
	
	CSkill2*	pSkill2Sc = pSkill2->AddComponent<CSkill2>();
	SAFE_RELEASE(pSkill2Sc);

	CDx11GameObject*	pSkill2Child = CDx11GameObject::Create("Skill2Child", OT_NORMAL);

	CDx11Transform* pChildTr = pSkill2->GetTransform();
	SAFE_RELEASE(pChildTr);

	pChildTr = pSkill2Child->GetTransform();

	CDx11Renderer*	pRenderer = pSkill2Child->AddComponent<CDx11Renderer>("Skill2Rederer");

	pRenderer->SetMesh(TRIANGLE_MESH);
	pRenderer->SetShader(DEFAULT_SHADER);
	SAFE_RELEASE(pRenderer);

	pRenderer = pSkill2->AddComponent<CDx11Renderer>("Skill2");
	pRenderer->SetMesh(TRIANGLE_MESH);
	pRenderer->SetShader(DEFAULT_SHADER);

	pChildTr->SetWorldPos(-2.f, 0.f, 0.f);
	SAFE_RELEASE(pChildTr);

	pSkill2->AddChild(pSkill2Child);

	SAFE_RELEASE(pRenderer);

	SAFE_RELEASE(pSkill2Child);
	return true;
}

bool CMainScene::CreateSky()
{
	CDx11Layer*	pLayer = m_pScene->FindLayer("DefaultLayer");
	
	if (!pLayer)
		return false;

	// 환경맵 설정
//	m_pScene->CreateSky("SkyTex", L"DesertSky.dds");
//	m_pScene->CreateSky("SkyTex", L"GrassSky.dds");
	m_pScene->CreateSky("SkyTex", L"SnowSky.dds");
//	m_pScene->CreateSky("SkyTex", L"SunSetSky.dds");

	SAFE_RELEASE(pLayer);

	return true;
}

bool CMainScene::CreateInvisible()
{
	CDx11Layer*	pLayer = m_pScene->FindLayer("DefaultLayer");
	if (!pLayer)
		return false;

	CDx11GameObject*	m_pPlayer = pLayer->FindObjectFromTag("PlayerObject");

	// 공전하는 자식객체를 만들기 위한 렌더러만 안쓴 객체를 만든다.
	CDx11GameObject*	pInvisibleObj = CDx11GameObject::Create("Invisible", OT_PROTOTYPE);
	
	CDx11Transform*	pTransform = pInvisibleObj->GetTransform();
//	pTransform->SetWorldPos(-2.f, 0.f, 0.f);

//	CDx11Renderer*	pRenderer = pInvisibleObjChild->AddComponent<CDx11Renderer>("InvisibleChildRenderer");
	/*CDx11Renderer*	pRenderer = pInvisibleObj->AddComponent<CDx11Renderer>("InvisibleRenderer");

	pRenderer->SetMesh(TRIANGLE_MESH);
	pRenderer->SetShader(DEFAULT_SHADER);
	SAFE_RELEASE(pRenderer);*/

	CInvisibleObject* pInvisibleObjScript = pInvisibleObj->AddComponent<CInvisibleObject>("InvisibleObject");
	SAFE_RELEASE(pInvisibleObjScript);

	pLayer->AddObject(pInvisibleObj);
	
	SAFE_RELEASE(pTransform);
	SAFE_RELEASE(pInvisibleObj);
	SAFE_RELEASE(pLayer);

	return true;
}


int CMainScene::CreateMonster(float fTime)
{
	CDx11GameObject*	pMonster = CDx11GameObject::Create("MonsterPrototype", OT_CLONE);

	CDx11Transform*	pTransform = pMonster->GetTransform();

	pTransform->SetWorldPos((rand() % 1001) / 100.f - 5.f, (rand() % 1001) / 100.f - 5.f,
		(rand() % 501) / 100.f);

	SAFE_RELEASE(pTransform);

	CDx11Layer*	pLayer = m_pScene->FindLayer("DefaultLayer");

	if (!pLayer)
		return false;

	pLayer->AddObject(pMonster);
	SAFE_RELEASE(pMonster);
	SAFE_RELEASE(pLayer);

	return 0;
}
