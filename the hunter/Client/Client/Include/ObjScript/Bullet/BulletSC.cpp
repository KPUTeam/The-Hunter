#include "BulletSC.h"
#include "GameObject/Dx11GameObject.h"
#include "Component/Dx11Transform.h"
#include "Component/Dx11ColliderSphere.h"
#include "Component/Dx11ColliderOBB.h"
#include "Resources/Dx11Mesh.h"
#include "Resources/Dx11ResourcesManager.h"
#include "Scene/Dx11Scene.h"
#include "Scene/Dx11layer.h"
#include "Component/Dx11Renderer.h"
#include "Component/Dx11Animation2D.h"
#include "Component/Dx11BillBoard.h"
#include "Component/Dx11Terrain.h"

DX11_USING

CBulletSC::CBulletSC() :
	m_fSpeed(0),
	m_fDist(0)
{
}

CBulletSC::CBulletSC(const CBulletSC & script)
{
	*this = script;
}


CBulletSC::~CBulletSC()
{
}

void CBulletSC::SetSpeed(float fSpeed)
{
	m_fSpeed = fSpeed;
}

void CBulletSC::SetDistance(float fDist)
{
	m_fDist = fDist;
}

bool CBulletSC::Init()
{
	// 충돌체를 설정한다.
	CDx11ColliderOBB* pOBB = m_pGameObject->AddComponent<CDx11ColliderOBB>(BULLET_BODY);
	float fAxisLen[3];
	fAxisLen[0] = m_fSize;
	fAxisLen[1] = m_fSize;
	fAxisLen[2] = m_fSize;
	pOBB->SetOBBInfo(fAxisLen, m_pTransform);
	SAFE_RELEASE(pOBB);

	//	총알 변수들의 초기값
	m_fSpeed = 40.f;
	m_fDist = 25.f;

	return true;
}

int CBulletSC::Update(float fTime)
{

	m_pTransform->MoveWorld(m_fSpeed, fTime);

	// 총알이 일정거리만큼 이동하면 삭제된다. 
	m_fDist -= (m_fSpeed * fTime);
	if (m_fDist <= 0.f)
	{
		m_pGameObject->Death();
	}

	// 바닥과 충돌 검사한다.
	CDx11GameObject* pTerrainObj = m_pScene->FindGameObject("TerrainObj");
	CDx11Terrain* pTerrain = (CDx11Terrain*)pTerrainObj->FindComponentFromTag("TerrainCP");

	// 바퀴 위치에서의 평면의 노말과 위치정보를 가져온다.
	TRIANGLE2RAY PosNormal = pTerrain->GetHeight(m_pGameObject);

	Vec3	vDist = m_pTransform->GetWorldPos() - PosNormal.Pos;
	float	fLength = vDist.Length();

	if (m_fSize >= fLength)
		DeathBullet();


	// 릴리즈
	SAFE_RELEASE(pTerrainObj);
	SAFE_RELEASE(pTerrain);


	return 0;
}

int CBulletSC::LateUpdate(float fTime)
{
	return 0;
}

void CBulletSC::Render(float fTime)
{
}

CBulletSC * CBulletSC::Clone()
{
	return new CBulletSC(*this);
}

void CBulletSC::DeathBullet()
{
	CDx11GameObject*	pSmallEffectObj = CDx11GameObject::Create("SmallEffectObject", OCT_NORMAL);
	m_pLayer->AddObject(pSmallEffectObj);

	pSmallEffectObj->SetCollision(false);

	CDx11Transform*	pEffectTr = pSmallEffectObj->GetTransform();
	pEffectTr->Scaling(2.3, 2.3, 2.3);

	Vec3 vEffectPos = m_pTransform->GetWorldPos();

	pEffectTr->SetWorldPos(vEffectPos);
	SAFE_RELEASE(pEffectTr);

	CDx11Renderer* pRenderer = pSmallEffectObj->AddComponent<CDx11Renderer>("SmallRenderer");
	pRenderer->CreateMaterial("SmallExplosion1");
	pRenderer->SetShader(EFFECTSHADER);
	pRenderer->SetMesh("EffectCenterMesh");
	pRenderer->SetRenderState(DX11_RS_ALPHABLEND, RSF_BLEND);
	SAFE_RELEASE(pRenderer);

	CDx11BillBoard* pBillBoard = pSmallEffectObj->AddComponent<CDx11BillBoard>("SmallBillBoard");
	SAFE_RELEASE(pBillBoard);


	CDx11Animation2D* pAnimation2D = pSmallEffectObj->AddComponent<CDx11Animation2D>("SmallAnim2D");
	pAnimation2D->Init();
	pAnimation2D->SetMaxFrame(75, 1);
	pAnimation2D->SetAnimation2DOption(A2DO_ONCE);
	pAnimation2D->SetAnimationLimitTime(0.75);

	for (int i = 1; i <= 75; ++i)
	{
		char	strKey[256] = {};
		sprintf_s(strKey, "explosion%d", i);
		wchar_t	strPath[MAX_PATH] = {};
		wsprintf(strPath, L"Explosion/%d.PNG", i);
		pAnimation2D->AddFrameTexture(strKey, strPath);
	}

	SAFE_RELEASE(pAnimation2D);
	SAFE_RELEASE(pSmallEffectObj);
}

void CBulletSC::OnCollisionEnter(CDx11Collider * pMy, CDx11Collider * pYour, float fTime)
{
	if (pMy->GetTag() == BULLET_BODY )
	{
		if(pYour->GetTag() == STATIC_MESH)
			DeathBullet();

		if (pYour->GetTag() == MONSTER_BODY)
			DeathBullet();

		if (pYour->GetTag() == PLAYER_BODY)
			DeathBullet();

		if (pYour->GetTag() == USER_BODY)
			DeathBullet();

		m_pGameObject->Death();
	}
}

void CBulletSC::OnCollision(CDx11Collider * pMy, CDx11Collider * pYour, float fTime)
{
}

void CBulletSC::OnCollisionExit(CDx11Collider * pMy, CDx11Collider * pYour, float fTime)
{
}

void CBulletSC::SetEmployer(CDx11GameObject * pTar)
{
	m_pEmployer = pTar;
}

CDx11GameObject * CBulletSC::GetEmployer()
{
	m_pEmployer->AddRef();
	return m_pEmployer;
}
