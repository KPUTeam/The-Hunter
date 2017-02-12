#include "Player.h"

#include "Core/Dx11Input.h"

#include "Resources/Dx11ResourcesManager.h"
#include "Resources/Dx11Texture.h"
#include "Resources/Dx11Mesh.h"

#include "GameObject/Dx11GameObject.h"
#include "Component/Dx11Renderer.h"
#include "Component/Dx11Transform.h"
#include "Scene/Dx11Layer.h"
#include "Scene/Dx11Scene.h"

CPlayer::CPlayer():
	x(0),
	y(0),
	z(0)
{
}

CPlayer::CPlayer(const CPlayer & player) :
	CDx11Script(player)
{
}

CPlayer::~CPlayer()
{
}

bool CPlayer::Init()
{
	x = 180.f / 100 / 2;
	y = 256.f / 100 / 2;
	z = 108.f / 100 / 2;

	CreateKey();
	CreateMesh();
	CreateTex();

	// 초기 위치 설정
	CDx11Transform* tr = m_pGameObject->GetTransform();
	tr->SetLocalRot(0.f, 180.f, 0.f);
	tr->MoveLocal(y, 1.f, AXIS_Y);
	SAFE_RELEASE(tr);
	
	return true;
}

void CPlayer::Input(float fTime)
{
	CDx11Transform* tr = m_pGameObject->GetTransform();

	if (DX11_GET_SINGLE(CDx11Input)->KeyDown("RotLeft") ||
		DX11_GET_SINGLE(CDx11Input)->KeyPush("RotLeft"))
	{
		tr->RotateWorld(-90, fTime, AXIS_Y);
	}

	if (DX11_GET_SINGLE(CDx11Input)->KeyDown("RotRight") ||
		DX11_GET_SINGLE(CDx11Input)->KeyPush("RotRight"))
	{
		tr->RotateWorld(90, fTime, AXIS_Y);
	}

	if (DX11_GET_SINGLE(CDx11Input)->KeyDown("Go") ||
		DX11_GET_SINGLE(CDx11Input)->KeyPush("Go"))
	{
		tr->MoveWorld(-3.f, fTime, AXIS_Z);
	}

	if (DX11_GET_SINGLE(CDx11Input)->KeyDown("Back") ||
		DX11_GET_SINGLE(CDx11Input)->KeyPush("Back"))
	{
		tr->MoveWorld(3.f, fTime, AXIS_Z);
	}

	if (DX11_GET_SINGLE(CDx11Input)->KeyDown("Skill")|| 
		DX11_GET_SINGLE(CDx11Input)->KeyPush("Skill"))
	{
		CreateBullet_Clone();
	}

	SAFE_RELEASE(tr);
}

int CPlayer::Update(float fTime)
{
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

bool CPlayer::CreateKey()
{
	DX11_GET_SINGLE(CDx11Input)->CreateKey("RotLeft", VK_LEFT);
	DX11_GET_SINGLE(CDx11Input)->CreateKey("RotRight", VK_RIGHT);
	DX11_GET_SINGLE(CDx11Input)->CreateKey("Go", VK_UP);
	DX11_GET_SINGLE(CDx11Input)->CreateKey("Back", VK_DOWN);
	DX11_GET_SINGLE(CDx11Input)->CreateKey("Skill", VK_SPACE);
	return true;
}

bool CPlayer::CreateMesh()
{
	// 앞면
	TEXVERTEX	tTexVtx[4] =
	{
		{ Vec3(-x, y, -z), Vec3(0.f, 0.f, -1.f), Vec2(0.f, 0.f) },
		{ Vec3(x, y, -z), Vec3(0.f, 0.f, -1.f), Vec2(1.f, 0.f) },
		{ Vec3(-x, -y,-z), Vec3(0.f, 0.f, -1.f), Vec2(0.f, 1.f) },
		{ Vec3(x, -y, -z), Vec3(0.f, 0.f, -1.f), Vec2(1.f, 1.f) }
	};

	UINT	iIndex[6] = { 0, 1, 3, 0, 3, 2 };

	CDx11Mesh* pMesh = DX11_GET_SINGLE(CDx11ResourcesManager)->
		CreateMesh("FrontMesh", sizeof(TEXVERTEX), 4, tTexVtx, sizeof(UINT), 6, iIndex, DXGI_FORMAT_R32_UINT);
	SAFE_RELEASE(pMesh);

	// 뒷면
	TEXVERTEX	tTexVtx1[4] =
	{
		{ Vec3(-x, y, z), Vec3(0.f, 0.f, 1.f), Vec2(0.f, 0.f) },
		{ Vec3(x, y, z), Vec3(0.f, 0.f, 1.f), Vec2(1.f, 0.f) },
		{ Vec3(-x, -y,z), Vec3(0.f, 0.f, 1.f), Vec2(0.f, 1.f) },
		{ Vec3(x, -y, z), Vec3(0.f, 0.f, 1.f), Vec2(1.f, 1.f) }
	};

	UINT	iIndex1[6] = {2,3,0,3,1,0};

	pMesh = DX11_GET_SINGLE(CDx11ResourcesManager)->
		CreateMesh("BackMesh", sizeof(TEXVERTEX), 4, tTexVtx1, sizeof(UINT), 6, iIndex1, DXGI_FORMAT_R32_UINT);
	SAFE_RELEASE(pMesh);

	// 왼쪽
	TEXVERTEX	tTexVtx2[4] =
	{
		{ Vec3(x, y, z), Vec3(1.f, 0.f, 0.f), Vec2(0.f, 0.f) },
		{ Vec3(x, y, -z), Vec3(1.f, 0.f, 0.f), Vec2(1.f, 0.f) },
		{ Vec3(x, -y, z), Vec3(1.f, 0.f, 0.f), Vec2(0.f, 1.f) },
		{ Vec3(x, -y, -z), Vec3(1.f, 0.f, 0.f), Vec2(1.f, 1.f) }
	};

	UINT	iIndex2[6] = { 2,3,0,3,1,0 };

	pMesh =
		DX11_GET_SINGLE(CDx11ResourcesManager)->
		CreateMesh("LeftMesh", sizeof(TEXVERTEX), 4, tTexVtx2, sizeof(UINT), 6, iIndex2, DXGI_FORMAT_R32_UINT);
	SAFE_RELEASE(pMesh);

	// 오른쪽
	TEXVERTEX	tTexVtx3[4] =
	{
		{ Vec3(-x, y, z), Vec3(-1.f, 0.f, 0.f), Vec2(0.f, 0.f) },
		{ Vec3(-x, y, -z), Vec3(-1.f, 0.f, 0.f), Vec2(1.f, 0.f) },
		{ Vec3(-x, -y, z), Vec3(-1.f, 0.f, 0.f), Vec2(0.f, 1.f) },
		{ Vec3(-x, -y, -z), Vec3(-1.f, 0.f, 0.f), Vec2(1.f, 1.f) }
	};

	UINT	iIndex3[6] = { 0, 1, 3, 0, 3, 2 };

	pMesh =
		DX11_GET_SINGLE(CDx11ResourcesManager)->
		CreateMesh("RightMesh", sizeof(TEXVERTEX), 4, tTexVtx3, sizeof(UINT), 6, iIndex3, DXGI_FORMAT_R32_UINT);
	SAFE_RELEASE(pMesh);

	// 윗면
	TEXVERTEX	tTexVtx4[4] =
	{
		{ Vec3(-x, y, z), Vec3(0.f, 1.f, 0.f), Vec2(0.f, 0.f) },
		{ Vec3(x, y, z), Vec3(0.f, 1.f, 0.f), Vec2(1.f, 0.f) },
		{ Vec3(-x, y,-z), Vec3(0.f, 1.f, 0.f), Vec2(0.f, 1.f) },
		{ Vec3(x, y, -z), Vec3(0.f, 1.f, 0.f), Vec2(1.f, 1.f) }
	};

	UINT	iIndex4[6] = { 0, 1, 3, 0, 3, 2 };

	pMesh =
		DX11_GET_SINGLE(CDx11ResourcesManager)->
		CreateMesh("TopMesh", sizeof(TEXVERTEX), 4, tTexVtx4, sizeof(UINT), 6, iIndex4, DXGI_FORMAT_R32_UINT);
	SAFE_RELEASE(pMesh);

	// 밑면
	TEXVERTEX	tTexVtx5[4] =
	{
		{ Vec3(-x, -y, z), Vec3(0.f, -1.f, 0.f), Vec2(0.f, 0.f) },
		{ Vec3(x, -y, z), Vec3(0.f, -1.f, 0.f), Vec2(1.f, 0.f) },
		{ Vec3(-x, -y,-z), Vec3(0.f,-1.f, 0.f), Vec2(0.f, 1.f) },
		{ Vec3(x, -y, -z), Vec3(0.f, -1.f, 0.f), Vec2(1.f, 1.f) }
	};

	UINT	iIndex5[6] = { 2,3,0,3,1,0 };

	pMesh =
		DX11_GET_SINGLE(CDx11ResourcesManager)->
		CreateMesh("BottomMesh", sizeof(TEXVERTEX), 4, tTexVtx5, sizeof(UINT), 6, iIndex5, DXGI_FORMAT_R32_UINT);
	SAFE_RELEASE(pMesh);

	return true;
}

bool CPlayer::CreateTex()
{
	// ================================== Front ====================================
	// 샘플 텍스쳐 로딩
	CDx11Texture* pTex = DX11_GET_SINGLE(CDx11ResourcesManager)->LoadTexture("FrontTex", L"front.png");
	SAFE_RELEASE(pTex);

	// 렌더러를 생성한다.
	CDx11Renderer* pRenderer = m_pGameObject->AddComponent<CDx11Renderer>("FrontRenderer");
	pRenderer->SetMesh("FrontMesh");
	pRenderer->SetShader(DEFAULT_TEX_SHADER);
	pRenderer->CreateMaterial("FrontTex", DEFAULT_SAMPLER);
	SAFE_RELEASE(pRenderer);

	// ================================== Back ====================================
	// 샘플 텍스쳐 로딩
	pTex = DX11_GET_SINGLE(CDx11ResourcesManager)->LoadTexture("BackTex", L"back.png");
	SAFE_RELEASE(pTex);

	// 렌더러를 생성한다.
	pRenderer = m_pGameObject->AddComponent<CDx11Renderer>("BackRenderer");
	pRenderer->SetMesh("BackMesh");
	pRenderer->SetShader(DEFAULT_TEX_SHADER);
	pRenderer->CreateMaterial("BackTex", DEFAULT_SAMPLER);
	SAFE_RELEASE(pRenderer);

	// ================================== Left ====================================
	// 샘플 텍스쳐 로딩
	pTex = DX11_GET_SINGLE(CDx11ResourcesManager)->LoadTexture("LeftTex", L"left.png");
	SAFE_RELEASE(pTex);

	// 렌더러를 생성한다.
	pRenderer = m_pGameObject->AddComponent<CDx11Renderer>("LeftRenderer");
	pRenderer->SetMesh("LeftMesh");
	pRenderer->SetShader(DEFAULT_TEX_SHADER);
	pRenderer->CreateMaterial("LeftTex", DEFAULT_SAMPLER);
	SAFE_RELEASE(pRenderer);

	// ================================== Right ====================================
	// 샘플 텍스쳐 로딩
	pTex = DX11_GET_SINGLE(CDx11ResourcesManager)->LoadTexture("RightTex", L"right.png");
	SAFE_RELEASE(pTex);

	// 렌더러를 생성한다.
	pRenderer = m_pGameObject->AddComponent<CDx11Renderer>("RightRenderer");
	pRenderer->SetMesh("RightMesh");
	pRenderer->SetShader(DEFAULT_TEX_SHADER);
	pRenderer->CreateMaterial("RightTex", DEFAULT_SAMPLER);
	SAFE_RELEASE(pRenderer);

	// ================================== Top ====================================
	// 샘플 텍스쳐 로딩
	pTex = DX11_GET_SINGLE(CDx11ResourcesManager)->LoadTexture("TopTex", L"top.png");
	SAFE_RELEASE(pTex);

	// 렌더러를 생성한다.
	pRenderer = m_pGameObject->AddComponent<CDx11Renderer>("TopRenderer");
	pRenderer->SetMesh("TopMesh");
	pRenderer->SetShader(DEFAULT_TEX_SHADER);
	pRenderer->CreateMaterial("TopTex", DEFAULT_SAMPLER);
	SAFE_RELEASE(pRenderer);

	// ================================== Bottom ====================================
	// 샘플 텍스쳐 로딩
	pTex = DX11_GET_SINGLE(CDx11ResourcesManager)->LoadTexture("BottomTex", L"bottom.png");
	SAFE_RELEASE(pTex);

	// 렌더러를 생성한다.
	pRenderer = m_pGameObject->AddComponent<CDx11Renderer>("BottomRenderer");
	pRenderer->SetMesh("BottomMesh");
	pRenderer->SetShader(DEFAULT_TEX_SHADER);
	pRenderer->CreateMaterial("BottomTex", DEFAULT_SAMPLER);
	SAFE_RELEASE(pRenderer);

	return true;
}

int CPlayer::CreateBullet_Clone()
{
	CDx11GameObject*	pBullet = CDx11GameObject::Create("BulletObject", OT_CLONE);
	CDx11Transform* pTransform = pBullet->GetTransform();
	CDx11Transform* pTr = m_pGameObject->GetTransform();
	pTransform->Copy(pTr);
	SAFE_RELEASE(pTr);

	pTransform->SetLocalRotX(-90);
	pTransform->MoveLocal(3.f, 1.f, AXIS_Y);

	m_pLayer->AddObject(pBullet);
	SAFE_RELEASE(pBullet);
	SAFE_RELEASE(pTransform);


	return 0;
}

