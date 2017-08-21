#include "Dx11FSM.h"

DX11_USING

CDx11FSM::CDx11FSM():
	m_pState(NULL)
{
}

CDx11FSM::~CDx11FSM()
{
	SAFE_RELEASE(m_pState);
}

bool CDx11FSM::Init()
{
	return true;
}

int CDx11FSM::Update(float fTime)
{
	if (NULL != m_pState)
		m_pState->Execute(fTime);

	return 0;
}

int CDx11FSM::LateUpdate(float fTime)
{
	return 0;
}

void CDx11FSM::Render(float fTime)
{
}

CDx11FSM * CDx11FSM::Clone()
{
	return new CDx11FSM(*this);
}