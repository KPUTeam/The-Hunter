#include "Dx11UI.h"

DX11_USING

CDx11UI::CDx11UI()
{
	m_eComType = CT_UI;
	SetTypeName<CDx11UI>();
}

CDx11UI::CDx11UI(const CDx11UI & ui)
	: CDx11Component(ui)
{
}


CDx11UI::~CDx11UI()
{
}

bool CDx11UI::Init()
{
	return true;
}

void CDx11UI::Input(float fTime)
{
}

int CDx11UI::Update(float fTime)
{
	return 0;
}

int CDx11UI::LateUpdate(float fTime)
{
	return 0;
}

void CDx11UI::Render(float fTime)
{
}
