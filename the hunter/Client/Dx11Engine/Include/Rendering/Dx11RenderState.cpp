#include "Dx11RenderState.h"

DX11_USING

CDx11RenderState::CDx11RenderState()	:
	m_pOldState(NULL),
	m_pState(NULL)
{
}

CDx11RenderState::~CDx11RenderState()
{
	SAFE_RELEASE(m_pOldState);
	SAFE_RELEASE(m_pState);
}
