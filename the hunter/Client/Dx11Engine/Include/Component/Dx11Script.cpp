#include "Dx11Script.h"

DX11_USING

CDx11Script::CDx11Script()
{
	SetTypeName<CDx11Script>();
	m_eComType = CT_SCRIPT;
}

CDx11Script::CDx11Script(const CDx11Script & script)	:
	CDx11Component(script)
{
	*this = script;
	m_iRef = 1;
}

CDx11Script::~CDx11Script()
{
}

bool CDx11Script::Init()
{
	return true;
}

int CDx11Script::Update(float fTime)
{
	return 0;
}

int CDx11Script::LateUpdate(float fTime)
{
	return 0;
}

void CDx11Script::Render(float fTime)
{
}

void CDx11Script::OnCollisionEnter(CDx11Collider* pMy, CDx11Collider* pYour, float fTime)
{
}

void CDx11Script::OnCollision(CDx11Collider* pMy, CDx11Collider* pYour, float fTime)
{
}

void CDx11Script::OnCollisionExit(CDx11Collider* pMy, CDx11Collider* pYour, float fTime)
{
}


// OnCollisionExit(CDx11Collider * pColl, float fTime)