#include "Dx11State.h"

DX11_USING

CDx11State::CDx11State():
	m_dClipTime(0)
{
	m_eState = AI_IDLE;
}

CDx11State::CDx11State(const CDx11State& com)
{
	*this = com;
	m_iRef = 1;
}

CDx11State::~CDx11State()
{
}

void CDx11State::Enter()
{
}

void CDx11State::Execute(float fTime)
{
}

int CDx11State::Exit(float fTime)
{
	return 0;
}

CDx11State * CDx11State::Clone()
{
	return new CDx11State(*this);
}

void CDx11State::SetGameObject(CDx11GameObject * obj)
{
	m_pGameObject = obj;
}

AI_STATE CDx11State::GetStateType()
{
	return m_eState;
}
