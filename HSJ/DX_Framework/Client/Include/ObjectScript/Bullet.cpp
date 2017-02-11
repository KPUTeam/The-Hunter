#include "Bullet.h"
#include "GameObject/Dx11GameObject.h"
#include "Component/Dx11Transform.h"

CBullet::CBullet()
{
}

CBullet::CBullet(const CBullet & bullet) :
	CDx11Script(bullet)
{
	m_fSpeed = bullet.m_fSpeed;
	m_fDist = bullet.m_fDist;
}

CBullet::~CBullet()
{
}

void CBullet::SetSpeed(float fSpeed)
{
	m_fSpeed = fSpeed;
}

void CBullet::SetDistance(float fDist)
{
	m_fDist = fDist;
}

bool CBullet::Init()
{
	m_fSpeed = 3.f;
	m_fDist = 100.f;

	return true;
}

void CBullet::Input(float fTime)
{
}

int CBullet::Update(float fTime)
{
	m_pTransform->MoveWorld(m_fSpeed, fTime, AXIS_Y);

	m_fDist -= (m_fSpeed * fTime);

	if (m_fDist <= 0.f)
	{
		m_pGameObject->Death();
	}

	return 0;
}

int CBullet::LateUpdate(float fTime)
{
	return 0;
}

void CBullet::Render(float fTime)
{
}

CBullet * CBullet::Clone()
{
	return new CBullet(*this);
}
