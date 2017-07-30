#include "User.h"

CUser::CUser()
{
}

CUser::CUser(const CUser & user) :
	CDx11Script(user)
{
}


CUser::~CUser()
{
}

bool CUser::Init()
{
	return true;
}

void CUser::Input(float fTime)
{
}

int CUser::Update(float fTime)
{
	return 0;
}

int CUser::LateUpdate(float fTime)
{
	return 0;
}

void CUser::Render(float fTime)
{
}

CUser * CUser::Clone()
{
	return new CUser(*this);
}

void CUser::OnCollisionEnter(CDx11Collider * pColl, float fTime)
{
}

void CUser::OnCollision(CDx11Collider * pColl, float fTime)
{
}

void CUser::OnCollisionExit(CDx11Collider * pColl, float fTime)
{
}
