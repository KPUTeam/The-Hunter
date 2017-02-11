#include "Player.h"
#include "Component/Dx11Transform.h"
#include "Component/Dx11Renderer.h"
#include "GameObject/Dx11GameObject.h"
#include "Scene/Dx11Layer.h"
#include "Core/Dx11Input.h"
#include "Resources/Dx11ResourcesManager.h"
#include "Resources/Dx11Texture.h"

CPlayer::CPlayer()
{
}

CPlayer::~CPlayer()
{
}

bool CPlayer::Init()
{
	DX11_GET_SINGLE(CDx11Input)->CreateKey("RotXFront", 'X');
	DX11_GET_SINGLE(CDx11Input)->CreateKey("RotXBack", 'C');
	DX11_GET_SINGLE(CDx11Input)->CreateKey("RotYFront", 'Y');
	DX11_GET_SINGLE(CDx11Input)->CreateKey("RotYBack", 'U');
	DX11_GET_SINGLE(CDx11Input)->CreateKey("RotZFront", 'D');
	DX11_GET_SINGLE(CDx11Input)->CreateKey("RotZBack", 'A');
	DX11_GET_SINGLE(CDx11Input)->CreateKey("MoveFront", 'W');
	DX11_GET_SINGLE(CDx11Input)->CreateKey("MoveBack", 'S');
	DX11_GET_SINGLE(CDx11Input)->CreateKey("Fire", VK_SPACE);
	DX11_GET_SINGLE(CDx11Input)->CreateKey("Skill1", VK_CONTROL, '1');
	DX11_GET_SINGLE(CDx11Input)->CreateKey("Skill2", VK_CONTROL, '2');
	DX11_GET_SINGLE(CDx11Input)->CreateKey("Skill3", VK_CONTROL, '3');
	DX11_GET_SINGLE(CDx11Input)->CreateKey("Skill4", VK_CONTROL, '4');

	m_fSolWSpeed = 2.f;
	m_fSolWLimit = 2.f;
	m_fSolWAcc = 0.f;
	m_bSolWEnable = false;

	if (!CreateMesh())
		return false;

	if (!CreateTex())
		return false;

	return true;
}

void CPlayer::Input(float fTime)
{
	if (DX11_GET_SINGLE(CDx11Input)->KeyDown("RotZBack") ||
		DX11_GET_SINGLE(CDx11Input)->KeyPush("RotZBack"))
	{
		m_pTransform->RotationWorld(DX11_PI, fTime, AXIS_Z);
	}

	if (DX11_GET_SINGLE(CDx11Input)->KeyDown("RotZFront") ||
		DX11_GET_SINGLE(CDx11Input)->KeyPush("RotZFront"))
	{
		m_pTransform->RotationWorld(-DX11_PI, fTime, AXIS_Z);
	}

	if (DX11_GET_SINGLE(CDx11Input)->KeyDown("RotXBack") ||
		DX11_GET_SINGLE(CDx11Input)->KeyPush("RotXBack"))
	{
		m_pTransform->RotationWorld(DX11_PI, fTime, AXIS_X);
	}

	if (DX11_GET_SINGLE(CDx11Input)->KeyDown("RotXFront") ||
		DX11_GET_SINGLE(CDx11Input)->KeyPush("RotXFront"))
	{
		m_pTransform->RotationWorld(-DX11_PI, fTime, AXIS_X);
	}

	if (DX11_GET_SINGLE(CDx11Input)->KeyDown("RotYBack") ||
		DX11_GET_SINGLE(CDx11Input)->KeyPush("RotYBack"))
	{
		m_pTransform->RotationWorld(DX11_PI, fTime, AXIS_Y);
	}

	if (DX11_GET_SINGLE(CDx11Input)->KeyDown("RotYFront") ||
		DX11_GET_SINGLE(CDx11Input)->KeyPush("RotYFront"))
	{
		m_pTransform->RotationWorld(-DX11_PI, fTime, AXIS_Y);
	}

	if (DX11_GET_SINGLE(CDx11Input)->KeyDown("MoveFront") ||
		DX11_GET_SINGLE(CDx11Input)->KeyPush("MoveFront"))
	{
		m_pTransform->MoveWorld(5.f, fTime, AXIS_Y);
	}

	if (DX11_GET_SINGLE(CDx11Input)->KeyDown("MoveBack") ||
		DX11_GET_SINGLE(CDx11Input)->KeyPush("MoveBack"))
	{
		m_pTransform->MoveWorld(-5.f, fTime, AXIS_Y);
	}

	if (DX11_GET_SINGLE(CDx11Input)->KeyDown("Fire") ||
		DX11_GET_SINGLE(CDx11Input)->KeyPush("Fire"))
	{
		Fire();
	}

	if (DX11_GET_SINGLE(CDx11Input)->KeyDown("Skill1"))
	{
		m_bSolWEnable = true;
		//SolW(fTime);
		//Fire();
	}

	if (DX11_GET_SINGLE(CDx11Input)->KeyDown("Skill2"))
	{
		//m_bSolWEnable = true;
		Skill2();
		//SolW(fTime);
		//Fire();
	}

	if (DX11_GET_SINGLE(CDx11Input)->KeyDown("Skill3"))
	{
		Skill3();
	}

	if (DX11_GET_SINGLE(CDx11Input)->KeyDown("Skill4"))
	{
		Skill4();
	}
}

int CPlayer::Update(float fTime)
{
	if (m_bSolWEnable)
	{
		SolW(fTime);
	}

	return 0;
}

int CPlayer::LateUpdate(float fTime)
{
	return 0;
}

void CPlayer::Render(float fTime)
{
}

CPlayer * CPlayer::Clone()
{
	return new CPlayer(*this);
}

void CPlayer::Fire()
{
	CDx11GameObject*	pBullet = CreateBullet(m_pTransform);
	SAFE_RELEASE(pBullet);

	// 자식 비행기 2명의 Transform을 얻어온다.
	CDx11GameObject*	pChild = m_pGameObject->FindChildFromTag("LeftChild");
	CDx11Transform*	pChildTr = pChild->GetTransform();
	pBullet = CreateBullet(pChildTr);
	SAFE_RELEASE(pBullet);
	SAFE_RELEASE(pChildTr);
	SAFE_RELEASE(pChild);
	
	pChild = m_pGameObject->FindChildFromTag("RightChild");
	pChildTr = pChild->GetTransform();
	pBullet = CreateBullet(pChildTr);
	SAFE_RELEASE(pBullet);
	SAFE_RELEASE(pChildTr);
	SAFE_RELEASE(pChild);
}

void CPlayer::SolW(float fTime)
{
	CDx11GameObject*	pChild = m_pGameObject->FindChildFromTag("LeftChild");
	CDx11Transform*	pChildTr = pChild->GetTransform();

	CDx11GameObject*	pRChild = m_pGameObject->FindChildFromTag("RightChild");
	CDx11Transform*	pRChildTr = pRChild->GetTransform();

	float	fSpeed = m_fSolWSpeed * fTime;
	m_fSolWAcc += fSpeed;

	//_cprintf("Acc : %.5f\n", m_fSolWAcc);

	Vec3	vMove(-fSpeed, 0.f, 0.f);
	Vec3	vMove1(fSpeed, 0.f, 0.f);

	if (m_fSolWAcc >= m_fSolWLimit)
	{
		m_bSolWEnable = false;
		vMove.x -= (m_fSolWAcc - m_fSolWLimit);
		vMove1.x += (m_fSolWAcc - m_fSolWLimit);
		m_fSolWAcc = 0.f;
	}
	
	pChildTr->MoveWorld(vMove);
	pRChildTr->MoveWorld(vMove1);

	SAFE_RELEASE(pChildTr);
	SAFE_RELEASE(pChild);

	SAFE_RELEASE(pRChildTr);
	SAFE_RELEASE(pRChild);
}


void CPlayer::Skill2()
{
	CDx11GameObject*	pSkill2 = CDx11GameObject::Create("Skill2Prototype",
		OT_CLONE);

	CDx11Transform*	pBulletTr = pSkill2->GetTransform();

	pBulletTr->Copy(m_pTransform);
	pBulletTr->MoveWorld(0.75f, 1.f, AXIS_Y);
	pBulletTr->SetWorldScale(0.5f, 0.5f, 0.5f);
//	pBulletTr->SetWorldView(m_pTransform->GetWorldAxis(AXIS_Y));
	//pBulletTr->MoveWorld(0.f, 0.5f, AXIS_Y);

	SAFE_RELEASE(pBulletTr);

	m_pLayer->AddObject(pSkill2);

	SAFE_RELEASE(pSkill2);
}

void CPlayer::Skill3()
{
	CDx11GameObject*	pBullet = CDx11GameObject::Create("BulletPrototype", OT_CLONE);
	CDx11Transform*	pBulletTr = pBullet->GetTransform();

	pBulletTr->Copy(m_pTransform);
	pBulletTr->MoveWorld(0.75f, 1.f, AXIS_Y);
	pBulletTr->SetWorldScale(0.5f, 0.5f, 0.5f);
	//pBulletTr->SetWorldView(m_pTransform->GetWorldAxis(AXIS_Y));

	SAFE_RELEASE(pBulletTr);
	m_pLayer->AddObject(pBullet);
	SAFE_RELEASE(pBullet);

	//
	CDx11GameObject*	pBullet1 = CDx11GameObject::Create("BulletPrototype", OT_CLONE);
	CDx11Transform*		pBulletTr1 = pBullet1->GetTransform();

	pBulletTr1->Copy(m_pTransform);
	pBulletTr1->MoveWorld(0.75f, 1.f, AXIS_Y);
	pBulletTr1->SetWorldRotZ(-DX11_PI / 4);
	pBulletTr1->SetWorldScale(0.5f, 0.5f, 0.5f);
	//pBulletTr1->SetWorldView(m_pTransform->GetWorldAxis(AXIS_X) + m_pTransform->GetWorldAxis(AXIS_Y));

	SAFE_RELEASE(pBulletTr1);
	m_pLayer->AddObject(pBullet1);
	SAFE_RELEASE(pBullet1);

	//
	CDx11GameObject*	pBullet2 = CDx11GameObject::Create("BulletPrototype", OT_CLONE);
	CDx11Transform*		pBulletTr2 = pBullet2->GetTransform();

	pBulletTr2->Copy(m_pTransform);
	pBulletTr2->MoveWorld(0.75f, 1.f, AXIS_Y);
	pBulletTr2->SetWorldRotZ(DX11_PI / 4);
	pBulletTr2->SetWorldScale(0.5f, 0.5f, 0.5f);
//	pBulletTr2->SetWorldView(m_pTransform->GetWorldAxis(AXIS_X) * -1 + m_pTransform->GetWorldAxis(AXIS_Y));

	SAFE_RELEASE(pBulletTr2);
	m_pLayer->AddObject(pBullet2);
	SAFE_RELEASE(pBullet2);
}

void CPlayer::Skill4()
{
}


CDx11GameObject * CPlayer::CreateBullet(CDx11Transform * pTransform)
{
	CDx11GameObject*	pBullet = CDx11GameObject::Create("BulletPrototype",
		OT_CLONE);

	CDx11Transform*	pBulletTr = pBullet->GetTransform();

	pBulletTr->Copy(pTransform);
	pBulletTr->MoveWorld(0.75f, 1.f, AXIS_Y);
	pBulletTr->SetWorldScale(0.5f, 0.5f, 0.5f);
//	pBulletTr->SetWorldView(pTransform->GetWorldAxis(AXIS_Y));
	//pBulletTr->MoveWorld(0.f, 0.5f, AXIS_Y);

	SAFE_RELEASE(pBulletTr);

	m_pLayer->AddObject(pBullet);
	
	return pBullet;
}

bool CPlayer::CreateMesh()
{
	// front face
	TEXVERTEX F_tTexVtx[4] =
	{
		{Vec3(-0.5f, 0.5f, -0.5f), Vec3(0.f, 0.f, -1.f), Vec2(0.f, 0.f)},
		{Vec3(0.5f, 0.5f, -0.5f), Vec3(0.f, 0.f, -1.f), Vec2(1.f, 0.f)},
		{Vec3(0.5f, -0.5f, -0.5f), Vec3(0.f, 0.f, -1.f), Vec2(0.f, 1.f)},
		{Vec3(-0.5f, -0.5f, -0.5f), Vec3(0.f, 0.f, -1.f), Vec2(1.f, 1.f)}
	};

	UINT F_iIndex[6] = {
		0, 1, 2,
		0, 2, 3
	};

	CDx11Mesh* pMesh = DX11_GET_SINGLE(CDx11ResourcesManager)->
		CreateMesh("Pyramid", sizeof(TEXVERTEX), 4, F_tTexVtx, sizeof(UINT), 6, F_iIndex, DXGI_FORMAT_R32_UINT);

	// Back face
	TEXVERTEX B_tTexVtx[4] =
	{
		{ Vec3(-0.5f, 0.5f, 0.5f), Vec3(0.f, 0.f, 1.f), Vec2(0.f, 0.f) },
		{ Vec3(0.5f, 0.5f, 0.5f), Vec3(0.f, 0.f, 1.f), Vec2(1.f, 0.f) },
		{ Vec3(0.5f, -0.5f, 0.5f), Vec3(0.f, 0.f, 1.f), Vec2(0.f, 1.f) },
		{ Vec3(-0.5f, -0.5f, 0.5f), Vec3(0.f, 0.f, 1.f), Vec2(1.f, 1.f) }
	};

	UINT B_iIndex[6] = {
		1, 0, 3,
		1, 3, 2
	};

	pMesh = DX11_GET_SINGLE(CDx11ResourcesManager)->
		CreateMesh("Texture1", sizeof(TEXVERTEX), 4, B_tTexVtx, sizeof(UINT), 6, B_iIndex, DXGI_FORMAT_R32_UINT);

	// Top face
	TEXVERTEX T_tTexVtx[4] =
	{
		{ Vec3(-0.5f, 0.5f, 0.5f), Vec3(0.f, 1.f, 0.f), Vec2(0.f, 0.f) },
		{ Vec3(0.5f, 0.5f, 0.5f), Vec3(0.f, 1.f, 0.f), Vec2(1.f, 0.f) },
		{ Vec3(0.5f, 0.5f, -0.5f), Vec3(0.f, 1.f, 0.f), Vec2(0.f, 1.f) },
		{ Vec3(-0.5f, 0.5f, -0.5f), Vec3(0.f, 1.f, 0.f), Vec2(1.f, 1.f) }
	};

	UINT T_iIndex[6] = {
		0, 1, 2,
		0, 2, 3
	};

	pMesh = DX11_GET_SINGLE(CDx11ResourcesManager)->
		CreateMesh("Texture2", sizeof(TEXVERTEX), 4, T_tTexVtx, sizeof(UINT), 6, T_iIndex, DXGI_FORMAT_R32_UINT);

	// Bottom face
	TEXVERTEX Bot_tTexVtx[4] =
	{
		{ Vec3(-0.5f, -0.5f, 0.5f), Vec3(0.f, -1.f, 0.f), Vec2(0.f, 0.f) },
		{ Vec3(0.5f, -0.5f, 0.5f), Vec3(0.f, -1.f, 0.f), Vec2(1.f, 0.f) },
		{ Vec3(0.5f, -0.5f, -0.5f), Vec3(0.f, -1.f, 0.f), Vec2(0.f, 1.f) },
		{ Vec3(-0.5f, -0.5f, -0.5f), Vec3(0.f, -1.f, 0.f), Vec2(1.f, 1.f) }
	};

	UINT Bot_iIndex[6] = {
		3, 2, 1,
		3, 1, 0
	};

	pMesh = DX11_GET_SINGLE(CDx11ResourcesManager)->
		CreateMesh("Texture3", sizeof(TEXVERTEX), 4, Bot_tTexVtx, sizeof(UINT), 6, Bot_iIndex, DXGI_FORMAT_R32_UINT);

	// Left face
	TEXVERTEX L_tTexVtx[4] =
	{
		{ Vec3(-0.5f, 0.5f, 0.5f), Vec3(1.f, 0.f, 0.f), Vec2(0.f, 0.f) },
		{ Vec3(-0.5f, 0.5f, -0.5f), Vec3(1.f, 0.f, 0.f), Vec2(1.f, 0.f) },
		{ Vec3(-0.5f, -0.5f, -0.5f), Vec3(1.f, 0.f, 0.f), Vec2(0.f, 1.f) },
		{ Vec3(-0.5f, -0.5f, 0.5f), Vec3(1.f, 0.f, 0.f), Vec2(1.f, 1.f) }
	};

	UINT L_iIndex[6] = {
		0, 1, 2,
		0, 2, 3
	};

	pMesh = DX11_GET_SINGLE(CDx11ResourcesManager)->
		CreateMesh("Texture4", sizeof(TEXVERTEX), 4, L_tTexVtx, sizeof(UINT), 6, L_iIndex, DXGI_FORMAT_R32_UINT);

	// Right face
	TEXVERTEX R_tTexVtx[4] =
	{
		{ Vec3(0.5f, 0.5f, -0.5f), Vec3(-1.f, 0.f, 0.f), Vec2(0.f, 0.f) },
		{ Vec3(0.5f, 0.5f, 0.5f), Vec3(-1.f, 0.f, 0.f), Vec2(1.f, 0.f) },
		{ Vec3(0.5f, -0.5f, 0.5f), Vec3(-1.f, 0.f, 0.f), Vec2(0.f, 1.f) },
		{ Vec3(0.5f, -0.5f, -0.5f), Vec3(-1.f, 0.f, 0.f), Vec2(1.f, 1.f) }
	};

	UINT R_iIndex[6] = {
		0, 1, 2,
		0, 2, 3
	};

	pMesh = DX11_GET_SINGLE(CDx11ResourcesManager)->
		CreateMesh("Texture5", sizeof(TEXVERTEX), 4, R_tTexVtx, sizeof(UINT), 6, R_iIndex, DXGI_FORMAT_R32_UINT);

	return true;
}

bool CPlayer::CreateTex()
{
	// Front
	// 샘플 텍스쳐 로딩
	CDx11Texture* pTex = DX11_GET_SINGLE(CDx11ResourcesManager)->LoadTexture("FrontTex", L"Pyramid.jpg");
	SAFE_RELEASE(pTex);

	// 렌더러를 생성한다.
	CDx11Renderer* pRenderer = m_pGameObject->AddComponent<CDx11Renderer>("FrontRenderer");
	pRenderer->SetMesh("Pyramid");
	pRenderer->SetShader(DEFAULT_TEX_SHADER);
	pRenderer->CreateMaterial("FrontTex", DEFAULT_SAMPLER);
	SAFE_RELEASE(pRenderer);

	// Back
	// 샘플 텍스쳐 로딩
	pTex = DX11_GET_SINGLE(CDx11ResourcesManager)->LoadTexture("BackTex", L"Texture1.jpg");
	SAFE_RELEASE(pTex);

	// 렌더러를 생성한다.
	pRenderer = m_pGameObject->AddComponent<CDx11Renderer>("BackRenderer");
	pRenderer->SetMesh("Texture1");
	pRenderer->SetShader(DEFAULT_TEX_SHADER);
	pRenderer->CreateMaterial("BackTex", DEFAULT_SAMPLER);
	SAFE_RELEASE(pRenderer);

	// Top
	// 샘플 텍스쳐 로딩
	pTex = DX11_GET_SINGLE(CDx11ResourcesManager)->LoadTexture("TopTex", L"Texture2.jpg");
	SAFE_RELEASE(pTex);

	// 렌더러를 생성한다.
	pRenderer = m_pGameObject->AddComponent<CDx11Renderer>("TopRenderer");
	pRenderer->SetMesh("Texture2");
	pRenderer->SetShader(DEFAULT_TEX_SHADER);
	pRenderer->CreateMaterial("TopTex", DEFAULT_SAMPLER);
	SAFE_RELEASE(pRenderer);

	// Bottom
	// 샘플 텍스쳐 로딩
	pTex = DX11_GET_SINGLE(CDx11ResourcesManager)->LoadTexture("BottomTex", L"Texture3.jpg");
	SAFE_RELEASE(pTex);

	// 렌더러를 생성한다.
	pRenderer = m_pGameObject->AddComponent<CDx11Renderer>("BottomRenderer");
	pRenderer->SetMesh("Texture3");
	pRenderer->SetShader(DEFAULT_TEX_SHADER);
	pRenderer->CreateMaterial("BottomTex", DEFAULT_SAMPLER);
	SAFE_RELEASE(pRenderer);

	// Left
	// 샘플 텍스쳐 로딩
	pTex = DX11_GET_SINGLE(CDx11ResourcesManager)->LoadTexture("LeftTex", L"Texture4.jpg");
	SAFE_RELEASE(pTex);

	// 렌더러를 생성한다.
	pRenderer = m_pGameObject->AddComponent<CDx11Renderer>("LeftRenderer");
	pRenderer->SetMesh("Texture4");
	pRenderer->SetShader(DEFAULT_TEX_SHADER);
	pRenderer->CreateMaterial("LeftTex", DEFAULT_SAMPLER);
	SAFE_RELEASE(pRenderer);

	// Right
	// 샘플 텍스쳐 로딩
	pTex = DX11_GET_SINGLE(CDx11ResourcesManager)->LoadTexture("RightTex", L"Texture5.jpg");
	SAFE_RELEASE(pTex);

	// 렌더러를 생성한다.
	pRenderer = m_pGameObject->AddComponent<CDx11Renderer>("RightRenderer");
	pRenderer->SetMesh("Texture5");
	pRenderer->SetShader(DEFAULT_TEX_SHADER);
	pRenderer->CreateMaterial("RightTex", DEFAULT_SAMPLER);
	SAFE_RELEASE(pRenderer);

	return true;
}