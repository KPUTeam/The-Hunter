#include "Dx11Script.h"

DX11_USING

CDx11Script::CDx11Script()
{
	m_eComType = CT_SCRIPT;
}

CDx11Script::CDx11Script(const CDx11Script & script)	:
	CDx11Component(script)
{
}

CDx11Script::~CDx11Script()
{
}

bool CDx11Script::Init()
{
	return true;
}

void CDx11Script::Input(float fTime)
{
}
