#include "Monster.h"
#include "Player.h"
#include "Bullet.h"
#include "GameObject/Dx11GameObject.h"
#include "Component/Dx11Transform.h"
#include "Component/Dx11Renderer.h"
#include "Scene/Dx11Scene.h"
#include "Scene/Dx11Layer.h"

CMonster::CMonster()
	:m_iCnt(0)
{
}

CMonster::CMonster(const CMonster & monster)	:
	CDx11Script(monster)
{
}

CMonster::~CMonster()
{
}

bool CMonster::Init()
{
	return true;
}

void CMonster::Input(float fTime)
{
}

int CMonster::Update(float fTime)
{
	CDx11GameObject*	pPlayerObj = m_pScene->FindObjectFromTag("PlayerObject");
	CPlayer*	pPlayer = (CPlayer*)pPlayerObj->FindComponentFromTag("PlayerScript");
	CDx11Transform*	pPlayerTr = pPlayerObj->GetTransform();

	// 플레이어의 위치를 얻어온다.
	Vec3 vPlayerPos = pPlayerTr->GetWorldPos();

	// m_vDir을 구한다.
	Vec3 m_vDir = vPlayerPos - (m_pTransform->GetWorldPos());

	// m_vDir과 몬스터의 Y축을 내적
	float fDot = m_vDir.Dot(m_pTransform->GetWorldAxis(AXIS_Y));
	float fAngle = acos(fDot / m_vDir.Length());

	// 몬스터의 Y축과 m_vDir을 외적해서 회전축을 구한다.
	Vec3 vRotation = m_pTransform->GetWorldAxis(AXIS_Y).Cross(m_vDir);

	m_pTransform->RotationWorld(fAngle, fTime, vRotation);
//	m_pTransform->RotateWorld(vRotation);
//	m_pTransform->MoveWorld(3.f, fTime, AXIS_Y);

	SAFE_RELEASE(pPlayerTr);
	SAFE_RELEASE(pPlayerObj);

	Fire();

	return 0;
}

int CMonster::LateUpdate(float fTime)
{
	return 0;
}

void CMonster::Render(float fTime)
{
}

CMonster * CMonster::Clone()
{
	return new CMonster(*this);
}



void CMonster::Fire()
{
	++m_iCnt;

	if (m_iCnt >= 250) {
		CDx11GameObject*	pBullet = CreateBullet(m_pTransform);
		SAFE_RELEASE(pBullet);

		CDx11GameObject*	pMonster = m_pGameObject->FindMonsterFromTag("MonsterPrototype");

		SAFE_RELEASE(pBullet);

		m_iCnt = 0;
	}
}

CDx11GameObject * CMonster::CreateBullet(CDx11Transform * pTransform)
{
	CDx11GameObject*	pBullet = CDx11GameObject::Create("BulletPrototype", OT_CLONE);
	CDx11Transform*	pBulletTr = pBullet->GetTransform();

	pBulletTr->Copy(pTransform);
	pBulletTr->MoveWorld(0.75, 1.f, AXIS_Y);
	pBulletTr->SetWorldScale(0.5f, 0.5f, 0.5f);
//	pBulletTr->SetWorldView(pTransform->GetWorldAxis(AXIS_Y));

	SAFE_RELEASE(pBulletTr);
	m_pLayer->AddObject(pBullet);

	return pBullet;
}