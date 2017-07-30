#include "Dx11UIPanel.h"
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

CDx11UIPanel::CDx11UIPanel()
{
	SetTypeName<CDx11UIPanel>();
	m_bDrag = false;
}

CDx11UIPanel::CDx11UIPanel(const CDx11UIPanel & panel)	:
	CDx11UI(panel)
{
	m_bDrag = false;
}

CDx11UIPanel::~CDx11UIPanel()
{
}

bool CDx11UIPanel::Init()
{
	if (!CDx11UI::Init())
		return false;

	CDx11Renderer*	pRenderer = m_pGameObject->AddComponent<CDx11Renderer>("PanelRenderer");

	pRenderer->SetRenderState(DX11_RS_DEPTH_DISABLE, RSF_DEPTHSTENCIL);
	pRenderer->SetCameraTag("UICamera");

	// 알파블렌딩 처리
	pRenderer->SetRenderState(DX11_RS_ALPHABLEND, RSF_BLEND);

	SAFE_RELEASE(pRenderer);

	SetMesh("UIMesh");
	SetShader("UIShader");
	SetTexture("Panel", L"Panel.png", DX11_TEXTURE_PATH);

	CDx11ColliderRC*	pColl = m_pGameObject->AddComponent<CDx11ColliderRC>("ButtonColl");

	pColl->SetRCInfo(0.f, 0.f, 300.f, 40.f);

	SAFE_RELEASE(pColl);

	return true;
}

int CDx11UIPanel::Update(float fTime)
{
	return 0;
}

int CDx11UIPanel::LateUpdate(float fTime)
{
	if (DX11_GET_SINGLE(CDx11Input)->KeyUp("MouseLButton"))
	{
		m_bDrag = false;
	}

	if (m_bDrag)
	{
		// 최상위 부모를 구한다.
		CDx11GameObject*	pParent = m_pGameObject->GetParent();
		Vec2	vMove = DX11_GET_SINGLE(CDx11Input)->GetMouseGap();

		if (!pParent)
		{
			m_pTransform->MoveWorld(Vec3(vMove.x, vMove.y, 0.f));
		}

		else
		{
			while (true)
			{
				CDx11GameObject*	pParent1 = pParent->GetParent();
				if (!pParent1)
					break;

				SAFE_RELEASE(pParent);
				pParent = pParent1;
			}
		}

		CDx11Transform*	pTr = pParent->GetTransform();

		pTr->MoveWorld(Vec3(vMove.x, vMove.y, 0.f));

		SAFE_RELEASE(pTr);

		SAFE_RELEASE(pParent);
	}

	return 0;
}

void CDx11UIPanel::Render(float fTime)
{
}

CDx11UIPanel * CDx11UIPanel::Clone()
{
	return new CDx11UIPanel(*this);
}

void CDx11UIPanel::OnCollisionEnter(CDx11Collider* pMy, CDx11Collider* pYour, float fTime)
{
}

void CDx11UIPanel::OnCollision(CDx11Collider* pMy, CDx11Collider* pYour, float fTime)
{
	if (pYour->GetTag() == "Mouse")
	{
		if (DX11_GET_SINGLE(CDx11Input)->KeyDown("MouseLButton"))
		{
			m_bDrag = true;
		}
	}
}

void CDx11UIPanel::OnCollisionExit(CDx11Collider* pMy, CDx11Collider* pYour, float fTime)
{
}
