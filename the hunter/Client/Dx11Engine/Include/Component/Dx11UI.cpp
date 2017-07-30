#include "Dx11UI.h"
#include "Dx11Transform.h"
#include "../Resources/Dx11ResourcesManager.h"
#include "../Resources/Dx11Texture.h"
#include "../Resources/Dx11Mesh.h"
#include "../Shader/Dx11Shader.h"
#include "../Shader/Dx11ShaderManager.h"
#include "Dx11Renderer.h"
#include "../GameObject/Dx11GameObject.h"

DX11_USING

CDx11UI::CDx11UI()
{
	m_eComType = CT_UI;
	m_iZOrder = 0;
}

CDx11UI::CDx11UI(const CDx11UI & ui) :
	CDx11Component(ui)
{
	m_iZOrder = 0;
}

CDx11UI::~CDx11UI()
{
}

void CDx11UI::SetZOrder(int iZOrder)
{
	m_iZOrder = iZOrder;
}

int CDx11UI::GetZOrder() const
{
	return m_iZOrder;
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

void CDx11UI::SetTexture(const string & strKey)
{
	CDx11Renderer*	pRenderer = (CDx11Renderer*)m_pGameObject->FindComponentFromType(CT_RENDERER);

	if (!pRenderer)
		return;

	pRenderer->SetMaterialDiffuseTex(strKey);

	SAFE_RELEASE(pRenderer);
}

void CDx11UI::SetTexture(const string & strKey, TCHAR * pFileName, 
	const string & strPathKey)
{
	CDx11Renderer*	pRenderer = (CDx11Renderer*)m_pGameObject->FindComponentFromType(CT_RENDERER);

	if (!pRenderer)
		return;

	pRenderer->SetMaterialDiffuseTexFromLoad(strKey, pFileName, strPathKey);

	SAFE_RELEASE(pRenderer);
}

bool CDx11UI::Init()
{
	return true;
}

void CDx11UI::OnCollisionEnter(CDx11Collider* pMy, CDx11Collider* pYour, float fTime)
{
}

void CDx11UI::OnCollision(CDx11Collider* pMy, CDx11Collider* pYour, float fTime)
{
}

void CDx11UI::OnCollisionExit(CDx11Collider* pMy, CDx11Collider* pYour, float fTime)
{
}
