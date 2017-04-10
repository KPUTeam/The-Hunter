#include "Dx11UI.h"
#include "Dx11Renderer.h"
#include "../GameObject/Dx11GameObject.h"

DX11_USING

CDx11UI::CDx11UI()
{
	m_eComType = CT_UI;
	SetTypeName<CDx11UI>();
	m_iZOrder = 0;
}


CDx11UI::CDx11UI(const CDx11UI & ui)	:
	CDx11Component(ui)
{
}

CDx11UI::~CDx11UI()
{
}

int CDx11UI::GetZOrder() const
{
	return m_iZOrder;
}

void CDx11UI::SetZOrder(int iZOrder)
{
	m_iZOrder = iZOrder;
}

void CDx11UI::SetMesh(const string & strKey)
{
	CDx11Renderer*	pRenderer = (CDx11Renderer*)m_pGameObject->FindComponentFromType(CT_RENDERER);
	if (!pRenderer)
		return;

	pRenderer->SetMesh(strKey);
	SAFE_RELEASE(pRenderer);
}

void CDx11UI::SetShader(const string & strKey)
{
	CDx11Renderer*	pRenderer = (CDx11Renderer*)m_pGameObject->FindComponentFromType(CT_RENDERER);
	if (!pRenderer)
		return;

	pRenderer->SetShader(strKey);
	SAFE_RELEASE(pRenderer);
}

void CDx11UI::SetRenderState(const string & strKey)
{
	CDx11Renderer*	pRenderer = (CDx11Renderer*)m_pGameObject->FindComponentFromType(CT_RENDERER);
	if (!pRenderer)
		return;

	pRenderer->SetRenderState(strKey);
	SAFE_RELEASE(pRenderer);
}

void CDx11UI::SetRenderTypeFlag(RENDER_TYPE_FLAG eType)
{
	CDx11Renderer*	pRenderer = (CDx11Renderer*)m_pGameObject->FindComponentFromType(CT_RENDERER);
	if (!pRenderer)
		return;

	pRenderer->SetRenderType(eType);
	SAFE_RELEASE(pRenderer);
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

void CDx11UI::OnCollisionEnter(CDx11Collider * pColl, float fTime)
{
}

void CDx11UI::OnCollision(CDx11Collider * pColl, float fTime)
{
}

void CDx11UI::OnCollisionExit(CDx11Collider * pColl, float fTime)
{
}
