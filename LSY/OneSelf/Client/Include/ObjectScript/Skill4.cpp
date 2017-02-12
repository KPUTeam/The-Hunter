#include "Skill4.h"
#include "GameObject/Dx11GameObject.h"
#include "Component/Dx11Transform.h"

CSkill4::CSkill4()
{
}

CSkill4::CSkill4(const CSkill4 & skill4)
	: CDx11Script(skill4)
{
	m_fSpeed = skill4.m_fSpeed;
	m_fDist = skill4.m_fDist;
}


CSkill4::~CSkill4()
{
}

bool CSkill4::Init()
{
	m_fSpeed = 3.f;
	m_fDist = 10.f;
	
	return true;
}

void CSkill4::Input(float fTime)
{
}

int CSkill4::Update(float fTime)
{
	m_pTransform->MoveWorld(m_fSpeed, fTime, AXIS_Y);

	m_fDist -= (m_fSpeed*fTime);

	if (m_fDist <= 0.f)
	{
		m_pGameObject->Death();
	}

	return 0;
}

int CSkill4::LateUpdate(float fTime)
{
	return 0;
}

void CSkill4::Render(float fTime)
{
}

CSkill4 * CSkill4::Clone()
{
	return new CSkill4(*this);
}
