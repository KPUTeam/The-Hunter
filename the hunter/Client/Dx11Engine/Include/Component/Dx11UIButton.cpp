#include "Dx11UIButton.h"
#include "Dx11Transform.h"
#include "../Resources/Dx11ResourcesManager.h"
#include "../Resources/Dx11Texture.h"
#include "../Resources/Dx11Mesh.h"
#include "../Shader/Dx11Shader.h"
#include "../Shader/Dx11ShaderManager.h"
#include "Dx11Renderer.h"
#include "../GameObject/Dx11GameObject.h"
#include "Dx11ColliderRC.h"
#include "Dx11Material.h"
#include "../Core/Dx11Input.h"

DX11_USING

CDx11UIButton::CDx11UIButton()	:
	m_bAction(false),
	m_eState(BS_NONE)
{
	memset(m_pMaterial, 0, sizeof(CDx11Material*) * 3);
}

CDx11UIButton::CDx11UIButton(const CDx11UIButton & ui)	:
	CDx11UI(ui)
{
	m_bAction = false;
	m_eState = BS_NONE;
}

CDx11UIButton::~CDx11UIButton()
{
	for (int i = 0; i < 3; ++i)
	{
		SAFE_RELEASE(m_pMaterial[i]);
	}
}

bool CDx11UIButton::Init()
{
	if (!CDx11UI::Init())
		return false;

	CDx11Renderer*	pRenderer = m_pGameObject->AddComponent<CDx11Renderer>("ButtonRenderer");

	pRenderer->SetRenderState(DX11_RS_DEPTH_DISABLE, RSF_DEPTHSTENCIL);
	pRenderer->SetCameraTag("UICamera");
	pRenderer->SetShader("UIButtonShader");

	// 알파블렌딩 처리
	pRenderer->SetRenderState(DX11_RS_ALPHABLEND, RSF_BLEND);

	SAFE_RELEASE(pRenderer);

	CDx11UI::SetMesh("UIMesh");

	SetStateTexture(BS_NONE, L"Mouse\\None.png", "DefaultButtonNormalTex", 
		DX11_TEXTURE_PATH);

	SetStateTexture(BS_MOUSEON, L"Mouse\\On.png", "DefaultButtonMouseOnTex",
		DX11_TEXTURE_PATH);

	pRenderer = (CDx11Renderer*)m_pGameObject->FindComponentFromType(CT_RENDERER);

	pRenderer->DeleteMultiTex(m_strBSTag[BS_MOUSEON]);

	SAFE_RELEASE(pRenderer);

	CDx11ColliderRC*	pColl = m_pGameObject->AddComponent<CDx11ColliderRC>("ButtonColl");

	pColl->SetRCInfo(0.f, 0.f, 100.f, 50.f);

	SAFE_RELEASE(pColl);

	return true;
}

int CDx11UIButton::Update(float fTime)
{
	if (m_eState == BS_MOUSEOUT)
		m_eState = BS_NONE;

	return 0;
}

int CDx11UIButton::LateUpdate(float fTime)
{
	return 0;
}

void CDx11UIButton::Render(float fTime)
{
}

CDx11UIButton * CDx11UIButton::Clone()
{
	return new CDx11UIButton(*this);
}

void CDx11UIButton::OnCollisionEnter(CDx11Collider* pMy, CDx11Collider* pYour, float fTime)
{
	if (pYour->GetTag() == "Mouse")
	{
		if (DX11_GET_SINGLE(CDx11Input)->KeyDown("MouseLButton"))
		{
			if (m_bAction)
			{
				CDx11GameObject*	pObj = pYour->GetGameObject();
				m_Action(pObj);
				SAFE_RELEASE(pObj);
			}
		}

		else
		{
		}
	}
}

void CDx11UIButton::OnCollision(CDx11Collider* pMy, CDx11Collider* pYour, float fTime)
{
	if (pYour->GetTag() == "Mouse")
	{
		if (DX11_GET_SINGLE(CDx11Input)->KeyDown("MouseLButton"))
		{
			CDx11Renderer*	pRenderer = (CDx11Renderer*)m_pGameObject->FindComponentFromType(CT_RENDERER);

			if (m_eState == BS_MOUSEON)
			{
				//pRenderer->AddMultiTex(m_strBSTag[BS_MOUSEON]);
				//pRenderer->DeleteMultiTex(m_strBSTag[BS_MOUSEON]);
			}

			else if (m_eState != BS_CLICK)
			{
				pRenderer->AddMultiTex(m_strBSTag[BS_CLICK]);
			}

			SAFE_RELEASE(pRenderer);

			m_eState = BS_CLICK;

			if (m_bAction)
			{
				CDx11GameObject*	pObj = pYour->GetGameObject();
				m_Action(pObj);
				SAFE_RELEASE(pObj);
			}
		}

		else
		{
			if (m_eState != BS_MOUSEON)
			{
				CDx11Renderer*	pRenderer = (CDx11Renderer*)m_pGameObject->FindComponentFromType(CT_RENDERER);

				pRenderer->DeleteMultiTex(m_strBSTag[BS_CLICK]);
				pRenderer->AddMultiTex(m_strBSTag[BS_MOUSEON]);

				SAFE_RELEASE(pRenderer);
			}
			m_eState = BS_MOUSEON;
		}
	}
}

void CDx11UIButton::OnCollisionExit(CDx11Collider* pMy, CDx11Collider* pYour, float fTime)
{
	if (pYour->GetTag() == "Mouse")
	{
		if (m_eState == BS_MOUSEON || m_eState == BS_CLICK)
		{
			m_eState = BS_MOUSEOUT;

			CDx11Renderer*	pRenderer = (CDx11Renderer*)m_pGameObject->FindComponentFromType(CT_RENDERER);

			pRenderer->DeleteMultiTex(m_strBSTag[BS_CLICK]);
			pRenderer->DeleteMultiTex(m_strBSTag[BS_MOUSEON]);

			SAFE_RELEASE(pRenderer);
		}
	}
}

bool CDx11UIButton::SetStateTexture(BUTTON_STATE eState, 
	TCHAR * pFileName, const string & strKey, const string & strPathKey)
{
	if (eState == BS_NONE)
	{
		CDx11Renderer*	pRenderer = (CDx11Renderer*)m_pGameObject->FindComponentFromType(CT_RENDERER);

		// 기존에 기본재질이 지정되어 있다면 지우고 생성한다.
		//pRenderer->DeleteMaterial();
		//pRenderer->CreateMaterial(strKey, pFileName, strPathKey);
		pRenderer->SetMaterialDiffuseTexFromLoad(strKey, pFileName, strPathKey);

		//m_pMaterial[eState] = pRenderer->GetMaterial();

		SAFE_RELEASE(pRenderer);
	}

	else
	{
		CDx11Renderer*	pRenderer = (CDx11Renderer*)m_pGameObject->FindComponentFromType(CT_RENDERER);

		string	strTag = "ButtonMouseON";

		if (eState == BS_CLICK)
			strTag = "ButtonClick";

		pRenderer->AddMultiTex(strKey, pFileName, strPathKey);

		SAFE_RELEASE(pRenderer);
		/*m_pMaterial[eState] = new CDx11Material;

		if (!m_pMaterial[eState]->Init())
		{
			SAFE_RELEASE(m_pMaterial[eState]);
			return false;
		}

		m_pMaterial[eState]->SetDiffuseTexture(strKey, pFileName, strPathKey);*/
	}

	m_strBSTag[eState] = strKey;

	return true;
}

bool CDx11UIButton::SetStateTexture(BUTTON_STATE eState, 
	const string & strKey)
{
	if (eState == BS_NONE)
	{
		CDx11Renderer*	pRenderer = (CDx11Renderer*)m_pGameObject->FindComponentFromType(CT_RENDERER);

		// 기존에 기본재질이 지정되어 있다면 지우고 생성한다.
		//pRenderer->DeleteMaterial();
		//pRenderer->CreateMaterial(strKey);
		pRenderer->SetMaterialDiffuseTex(strKey);

		//m_pMaterial[eState] = pRenderer->GetMaterial();

		SAFE_RELEASE(pRenderer);
	}

	else
	{
		CDx11Renderer*	pRenderer = (CDx11Renderer*)m_pGameObject->FindComponentFromType(CT_RENDERER);

		pRenderer->AddMultiTex(strKey);

		SAFE_RELEASE(pRenderer);
		/*m_pMaterial[eState] = new CDx11Material;

		if (!m_pMaterial[eState]->Init())
		{
			SAFE_RELEASE(m_pMaterial[eState]);
			return false;
		}

		m_pMaterial[eState]->SetDiffuseTexture(strKey);*/
	}

	m_strBSTag[eState] = strKey;
	
	return true;
}

bool CDx11UIButton::SetMesh(const string & strKey, void * pVertices,
	UINT iVertexCount, UINT iVertexStride, void * pIndices, UINT iIndexCount,
	UINT iIndexStride, DXGI_FORMAT eIndexFmt,
	D3D11_PRIMITIVE_TOPOLOGY ePrimitive)
{
	CDx11Renderer*	pRenderer = (CDx11Renderer*)m_pGameObject->FindComponentFromType(CT_RENDERER);

	CDx11Mesh*	pMesh = DX11_GET_SINGLE(CDx11ResourcesManager)->CreateMesh(
		strKey, pVertices, iVertexCount, iVertexStride, pIndices, iIndexCount,
		iIndexStride, eIndexFmt, ePrimitive);

	pRenderer->SetMesh(pMesh);

	SAFE_RELEASE(pMesh);

	SAFE_RELEASE(pRenderer);

	return true;
}
