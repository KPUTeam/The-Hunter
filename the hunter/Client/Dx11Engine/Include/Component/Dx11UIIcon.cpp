#include "Dx11UIIcon.h"
#include "Dx11Transform.h"
#include "../Resources/Dx11ResourcesManager.h"
#include "../Resources/Dx11Texture.h"
#include "../Resources/Dx11Mesh.h"
#include "../Shader/Dx11Shader.h"
#include "../Shader/Dx11ShaderManager.h"
#include "Dx11Renderer.h"
#include "../GameObject/Dx11GameObject.h"
#include "Dx11ColliderRC.h"
#include "../Core/Dx11Input.h"

DX11_USING

CDx11UIIcon::CDx11UIIcon()
{
	m_bIconEnable = true;
	m_bDrag = false;
	SetTypeName<CDx11UIIcon>();
}

CDx11UIIcon::CDx11UIIcon(const CDx11UIIcon & icon)	:
	CDx11UI(icon)
{
}

CDx11UIIcon::~CDx11UIIcon()
{
}

void CDx11UIIcon::SetIconEnable(bool bEnable)
{
	m_bIconEnable = bEnable;
}

void CDx11UIIcon::StopDrag()
{
	m_bDrag = false;
}

bool CDx11UIIcon::GetDrag()
{
	return m_bDrag;
}

bool CDx11UIIcon::Init()
{
	if (!CDx11UI::Init())
		return false;

	m_bIconFunc = false;
	m_bIconMouseUpFunc = false;

	CDx11Renderer*	pRenderer = m_pGameObject->AddComponent<CDx11Renderer>("IconRenderer");

	pRenderer->SetRenderState(DX11_RS_DEPTH_DISABLE, RSF_DEPTHSTENCIL);
	pRenderer->SetCameraTag("UICamera");

	// 알파블렌딩 처리
	pRenderer->SetRenderState(DX11_RS_ALPHABLEND, RSF_BLEND);

	SAFE_RELEASE(pRenderer);

	SetMesh("UIMesh");
	SetShader("UIShader");
	SetTexture("Icon", L"Sword2.png", DX11_TEXTURE_PATH);

	CDx11ColliderRC*	pColl = m_pGameObject->AddComponent<CDx11ColliderRC>("IconColl");

	pColl->SetRCInfo(0.f, 0.f, 52.f, 64.f);

	m_pTransform->Scaling(52.f, 64.f, 1.f);

	SAFE_RELEASE(pColl);

	return true;
}

int CDx11UIIcon::Update(float fTime)
{
	return 0;
}

int CDx11UIIcon::LateUpdate(float fTime)
{
	if (m_bDrag)
	{
		if (DX11_GET_SINGLE(CDx11Input)->KeyUp("MouseLButton"))
		{
			m_bDrag = false;

			if (m_bIconMouseUpFunc)
				m_IconMouseUpFunc(this, fTime);

			CDx11GameObject*	pParent = m_pGameObject->GetParent();

			CDx11Transform*	pTr = pParent->GetTransform();

			m_pTransform->SetWorldPos(VEC3ZERO);

			SAFE_RELEASE(pTr);

			SAFE_RELEASE(pParent);
		}

		else
		{
			Vec2	vMove = DX11_GET_SINGLE(CDx11Input)->GetMouseGap();

			m_pTransform->MoveWorld(Vec3(vMove.x, vMove.y, 0.f));
		}
	}

	return 0;
}

void CDx11UIIcon::Render(float fTime)
{
}

CDx11UIIcon * CDx11UIIcon::Clone()
{
	return new CDx11UIIcon(*this);
}

void CDx11UIIcon::OnCollisionEnter(CDx11Collider* pMy, CDx11Collider* pYour, float fTime)
{

}

void CDx11UIIcon::OnCollision(CDx11Collider* pMy, CDx11Collider* pYour, float fTime)
{
	if (pYour->GetTag() == "Mouse")
	{
		if (DX11_GET_SINGLE(CDx11Input)->KeyDown("MouseLButton"))
		{
			m_bDrag = true;

			if (m_bIconFunc)
				m_IconFunc(this, fTime);
		}		
	}
}

void CDx11UIIcon::OnCollisionExit(CDx11Collider* pMy, CDx11Collider* pYour, float fTime)
{
}
