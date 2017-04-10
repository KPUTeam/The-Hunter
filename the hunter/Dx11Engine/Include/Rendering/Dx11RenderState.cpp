#include "Dx11RenderState.h"

DX11_USING

CDx11RenderState::CDx11RenderState()	:
	m_pState(NULL),
	m_pPrevState(NULL)
{
}


CDx11RenderState::~CDx11RenderState()
{
	SAFE_RELEASE(m_pPrevState);
	SAFE_RELEASE(m_pState);
}

RENDER_STATE_TYPE CDx11RenderState::GetStateType()
{
	return m_eType;
}
