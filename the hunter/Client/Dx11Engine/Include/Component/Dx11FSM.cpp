#include "Dx11FSM.h"

DX11_USING

CDx11FSM::CDx11FSM():
	m_pState(NULL),
	m_pNextState(NULL)
{
	m_eComType = CT_FSM;
}

CDx11FSM::~CDx11FSM()
{
	SAFE_RELEASE(m_pState);
	SAFE_RELEASE(m_pNextState);
}

bool CDx11FSM::Init()
{
	return true;
}

int CDx11FSM::Update(float fTime)
{
	if (m_pState != NULL)
		m_pState->Execute(fTime);

	return 0;
}

int CDx11FSM::LateUpdate(float fTime)
{
	if (m_pNextState != NULL)
	{
		DX11_GET_SINGLE(CDx11Scheduler)->DeleteSchedule(m_pState);

		SAFE_RELEASE(m_pState);
		
		m_pState = m_pNextState;
		m_pState->Enter();

		m_pNextState = NULL;

	}
	return 0;
}

void CDx11FSM::Render(float fTime)
{
}

CDx11FSM * CDx11FSM::Clone()
{
	return new CDx11FSM(*this);
}

AI_STATE CDx11FSM::GetStateType()
{
	return m_pState->GetStateType();
}