#include "Skill2.h"
#include "GameObject/Dx11GameObject.h"
#include "Component/Dx11Transform.h"

CSkill2::CSkill2()
{
}

CSkill2::CSkill2(const CSkill2 & skill2)	:
	CDx11Script(skill2)
{

	m_fSpeed = skill2.m_fSpeed;
	m_fDist = skill2.m_fDist;
}

CSkill2::~CSkill2()
{
}

bool CSkill2::Init()
{
	m_fSpeed = 3.f;
	m_fDist = 10.f;

	return true;
}

void CSkill2::Input(float fTime)
{
}

int CSkill2::Update(float fTime)
{
	m_pTransform->MoveWorld(m_fSpeed, fTime, AXIS_Y);
	m_pTransform->RotationWorld(DX11_PI * 6.f, fTime, AXIS_Z);

	m_fDist -= (m_fSpeed * fTime);

	if (m_fDist <= 0.f)
	{
		m_pGameObject->Death();
	}

	return 0;
}

int CSkill2::LateUpdate(float fTime)
{
	return 0;
}

void CSkill2::Render(float fTime)
{
}

CSkill2 * CSkill2::Clone()
{
	return new CSkill2(*this);
}
