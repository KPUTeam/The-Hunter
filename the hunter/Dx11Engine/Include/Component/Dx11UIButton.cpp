#include "Dx11UIButton.h"
#include "Dx11Renderer.h"
#include "../GameObject/Dx11GameObject.h"
#include "Dx11ColliderRect.h"
#include "Dx11Material.h"
#include "../Core/Dx11Input.h"
#include "../Resources/Dx11Texture.h"
#include "../Resources/Dx11ResourcesManager.h"
#include "Dx11Transform.h"

DX11_USING

CDx11UIButton::CDx11UIButton()
{
	for (int i = 0; i < BS_END; ++i)
	{
		m_pBtnTexture[i] = NULL;
	}
	m_bMouseDown = false;
	m_bCallBack = false;
	m_eBtnState = BS_NORMAL;
	SetTypeName<CDx11UIButton>();
}


CDx11UIButton::CDx11UIButton(const CDx11UIButton & button)	:
	CDx11UI(button)
{
	m_vBtnSize = button.m_vBtnSize;
	m_eBtnState = button.m_eBtnState;
	memcpy(m_vStateColor, button.m_vStateColor, sizeof(Vec4) * BS_END);

	for (int i = 0; i < BS_END; ++i)
	{
		m_pBtnTexture[i] = button.m_pBtnTexture[i];
		if (m_pBtnTexture[i])
			m_pBtnTexture[i]->AddRef();
	}

}

CDx11UIButton::~CDx11UIButton()
{
	for (int i = 0; i < BS_END; ++i)
	{
		SAFE_RELEASE(m_pBtnTexture[i]);
	}
}

void CDx11UIButton::SetButtonSize(const Vec2 & vSize)
{
	m_vBtnSize = vSize;
}

void CDx11UIButton::SetStateColor(BUTTON_STATE bs, const Vec4 & vColor)
{
	m_vStateColor[bs] = vColor;
}

void CDx11UIButton::AddButtonTexture(BUTTON_STATE bs, const string & strKey, 
	TCHAR * pFileName, const string & strPathKey)
{
	if (!pFileName)
	{
		SAFE_RELEASE(m_pBtnTexture[bs]);
		m_pBtnTexture[bs] = DX11_GET_SINGLE(CDx11ResourcesManager)->FindTexture(strKey);
	}

	else
	{
		SAFE_RELEASE(m_pBtnTexture[bs]);
		m_pBtnTexture[bs] = DX11_GET_SINGLE(CDx11ResourcesManager)->LoadTexture(strKey,
			pFileName, strPathKey);
	}
}

bool CDx11UIButton::Init()
{
	CDx11Renderer*	pRenderer = m_pGameObject->AddComponent<CDx11Renderer>("ButtonRenderer");
	pRenderer->SetMesh(RECT_UI_MESH);
	pRenderer->SetShader(UI_SHADER);
	pRenderer->CreateMaterial("StartButton", L"Button.png", 0, TEXTURE_PATH);
	AddButtonTexture(BS_NORMAL, "StartButton");
	pRenderer->SetRenderState(DX11_RS_ALPHABLEND);
	pRenderer->SetRenderState(DX11_RS_DEPTH_DISABLE);
	
	SAFE_RELEASE(pRenderer);

	CDx11ColliderRect*	pRectColl = m_pGameObject->AddComponent<CDx11ColliderRect>("RectColl");
	pRectColl->SetColor(Green);
	pRectColl->ColorEnable();
	pRectColl->SetRectInfo(0.f, 0.f, 100.f, 50.f);
	SAFE_RELEASE(pRectColl);

	m_vStateColor[BS_NORMAL] = White;
	m_vStateColor[BS_MOUSEON] = Yellow;
	m_vStateColor[BS_CLICK] = Red;

	return true;
}

void CDx11UIButton::Input(float fTime)
{
}

int CDx11UIButton::Update(float fTime)
{
	return 0;
}

int CDx11UIButton::LateUpdate(float fTime)
{
	m_pTransform->Scaling(m_vBtnSize.x, m_vBtnSize.y, 1.f);
	return 0;
}

void CDx11UIButton::Render(float fTime)
{
}

CDx11UIButton * CDx11UIButton::Clone()
{
	return new CDx11UIButton(*this);
}

void CDx11UIButton::OnCollisionEnter(CDx11Collider * pColl, float fTime)
{
	if (pColl->GetTag() == "MouseCollider")
	{
		m_pTransform->SetWorldPos(540.f, 475.f, 0.f);
		SetButtonSize(Vec2(200.f, 100.f));
		CDx11ColliderRect* pCollRect = (CDx11ColliderRect*)m_pGameObject->FindComponentFromType(CT_COLLIDER);
		pCollRect->SetRectInfo(0.f, 0.f, 200.f, 100.f);
		SAFE_RELEASE(pCollRect);

		if (!DX11_GET_SINGLE(CDx11Input)->KeyDown("MouseLButton") &&
			!DX11_GET_SINGLE(CDx11Input)->KeyPush("MouseLButton"))
			m_bMouseDown = false;
	}
}

void CDx11UIButton::OnCollision(CDx11Collider * pColl, float fTime)
{
	if (pColl->GetTag() == "MouseCollider")
	{
		CDx11Renderer*	pRenderer = (CDx11Renderer*)m_pGameObject->FindComponentFromType(CT_RENDERER);
		CDx11Material*	pMtrl = pRenderer->GetMaterial();
		
		

		// 마우스 클릭 확인
		if (!m_bMouseDown)
		{
			// 버튼 다운
			if (DX11_GET_SINGLE(CDx11Input)->KeyDown("MouseLButton"))
			{
				m_bMouseDown = true;
				m_eBtnState = BS_CLICK;
			}
			else
				m_eBtnState = BS_MOUSEON;
		}
		
		else
		{
			// 버튼 업
			if (DX11_GET_SINGLE(CDx11Input)->KeyUp("MouseLButton"))
			{
				m_eBtnState = BS_CLICK;
				if (m_bCallBack)
					m_CallBack(this, fTime);
			}

			//버튼 업이 완료되었고 다운, 푸쉬상태가 아닐 때
			else if (!DX11_GET_SINGLE(CDx11Input)->KeyDown("MouseLButton") &&
					!DX11_GET_SINGLE(CDx11Input)->KeyPush("MouseLButton"))
			{
				m_eBtnState = BS_MOUSEON;
				m_bMouseDown = false;
			}
		}

		pMtrl->SetDiffuse(m_vStateColor[m_eBtnState]);
		if (m_pBtnTexture[m_eBtnState])
			pMtrl->SetDiffuseTexture(m_pBtnTexture[m_eBtnState]);

		SAFE_RELEASE(pMtrl);
		SAFE_RELEASE(pRenderer);
	}
	
}

void CDx11UIButton::OnCollisionExit(CDx11Collider * pColl, float fTime)
{
	m_eBtnState = BS_NORMAL;
	m_pTransform->SetWorldPos(590.f, 500.f, 0.f);
	SetButtonSize(Vec2(100.f, 50.f));
	CDx11ColliderRect* pCollRect = (CDx11ColliderRect*)m_pGameObject->FindComponentFromType(CT_COLLIDER);
	pCollRect->SetRectInfo(0.f, 0.f, 100.f, 50.f);
	SAFE_RELEASE(pCollRect);

	CDx11Renderer*	pRenderer = (CDx11Renderer*)m_pGameObject->FindComponentFromType(CT_RENDERER);
	CDx11Material*	pMtrl = pRenderer->GetMaterial();

	pMtrl->SetDiffuse(m_vStateColor[m_eBtnState]);
	pMtrl->SetDiffuseTexture(m_pBtnTexture[m_eBtnState]);

	SAFE_RELEASE(pMtrl);
	SAFE_RELEASE(pRenderer);
}
