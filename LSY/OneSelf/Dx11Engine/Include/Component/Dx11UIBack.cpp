#include "Dx11UIBack.h"

DX11_USING

CDx11UIBack::CDx11UIBack()
{
}

CDx11UIBack::CDx11UIBack(const CDx11UIBack & ui)
	: CDx11UI(ui)
{
}


CDx11UIBack::~CDx11UIBack()
{
}

bool CDx11UIBack::Init()
{
	return true;
}

void CDx11UIBack::Input(float fTime)
{
}

int CDx11UIBack::Update(float fTime)
{
	return 0;
}

int CDx11UIBack::LateUpdate(float fTime)
{
	return 0;
}

void CDx11UIBack::Render(float fTime)
{
}

CDx11UIBack * CDx11UIBack::Clone()
{
	return new CDx11UIBack(*this);
}
