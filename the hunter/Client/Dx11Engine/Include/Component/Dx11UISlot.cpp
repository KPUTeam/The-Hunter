#include "Dx11UISlot.h"
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

CDx11UISlot::CDx11UISlot()	:
	m_pOnTexture(NULL),
	m_bIcon(false),
	m_bSlotFunc(false),
	m_bSlotDownFunc(false),
	m_SlotFunc(nullptr),
	m_SlotDownFunc(nullptr)
{
	SetTypeName<CDx11UISlot>();
}

CDx11UISlot::CDx11UISlot(const CDx11UISlot & slot)	:
	CDx11UI(slot)
{
	m_pOnTexture = slot.m_pOnTexture;
	m_pOnTexture->AddRef();

	m_bIcon = slot.m_bIcon;
}

CDx11UISlot::~CDx11UISlot()
{
	SAFE_RELEASE(m_pOnTexture);
}

void CDx11UISlot::AddIcon(CDx11GameObject * pIcon)
{
	m_bIcon = true;
}

void CDx11UISlot::DeleteIcon()
{
	m_bIcon = false;
}

bool CDx11UISlot::Init()
{
	if (!CDx11UI::Init())
		return false;

	m_bSlotFunc = false;

	CDx11Renderer*	pRenderer = m_pGameObject->AddComponent<CDx11Renderer>("SlotRenderer");

	pRenderer->SetRenderState(DX11_RS_DEPTH_DISABLE, RSF_DEPTHSTENCIL);
	pRenderer->SetCameraTag("UICamera");

	// 알파블렌딩 처리
	pRenderer->SetRenderState(DX11_RS_ALPHABLEND, RSF_BLEND);

	SAFE_RELEASE(pRenderer);

	SetMesh("UIMesh");
	SetShader("UIShader");
	SetTexture("Slot", L"Slot.png", DX11_TEXTURE_PATH);

	m_pOnTexture = DX11_GET_SINGLE(CDx11ResourcesManager)->LoadTexture(
		"SlotOn", L"SlotMouseOn.png", DX11_TEXTURE_PATH);

	CDx11ColliderRC*	pColl = m_pGameObject->AddComponent<CDx11ColliderRC>("SlotColl");

	pColl->SetRCInfo(0.f, 0.f, 52.f, 64.f);

	m_pTransform->Scaling(52.f, 64.f, 1.f);

	SAFE_RELEASE(pColl);

	return true;
}

int CDx11UISlot::Update(float fTime)
{
	return 0;
}

int CDx11UISlot::LateUpdate(float fTime)
{
	return 0;
}

void CDx11UISlot::Render(float fTime)
{
}

CDx11UISlot * CDx11UISlot::Clone()
{
	return new CDx11UISlot(*this);
}

void CDx11UISlot::OnCollisionEnter(CDx11Collider* pMy, CDx11Collider* pYour, float fTime)
{
}

void CDx11UISlot::OnCollision(CDx11Collider* pMy, CDx11Collider* pYour, float fTime)
{
	if (pYour->GetTag() == "Mouse")
	{
		if (DX11_GET_SINGLE(CDx11Input)->KeyUp("MouseLButton"))
		{
			if (m_bSlotFunc)
				m_SlotFunc(this, pYour, fTime);
		}

		else if (DX11_GET_SINGLE(CDx11Input)->KeyDown("MouseLButton"))
		{
			if (m_bSlotDownFunc)
				m_SlotDownFunc(this, pYour, fTime);

		}
	}
}

void CDx11UISlot::OnCollisionExit(CDx11Collider* pMy, CDx11Collider* pYour, float fTime)
{
}
