#include "Dx11UIBackground.h"

DX11_USING

CDx11UIBackground::CDx11UIBackground()
{
}

CDx11UIBackground::CDx11UIBackground(const CDx11UIBackground & ui)	:
	CDx11UI(ui)
{
}

CDx11UIBackground::~CDx11UIBackground()
{
}

bool CDx11UIBackground::Init()
{
	return true;
}

void CDx11UIBackground::Input(float fTime)
{
}

int CDx11UIBackground::Update(float fTime)
{
	return 0;
}

int CDx11UIBackground::LateUpdate(float fTime)
{
	return 0;
}

void CDx11UIBackground::Render(float fTime)
{
}

CDx11UIBackground * CDx11UIBackground::Clone()
{
	return new CDx11UIBackground(*this);
}

void CDx11UIBackground::OnCollisionEnter(CDx11Collider * pColl, float fTime)
{
}

void CDx11UIBackground::OnCollision(CDx11Collider * pColl, float fTime)
{
}

void CDx11UIBackground::OnCollisionExit(CDx11Collider * pColl, float fTime)
{
}
