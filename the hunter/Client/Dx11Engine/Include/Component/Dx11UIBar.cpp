#include "Dx11UIBar.h"
#include "Dx11Transform.h"
#include "../Resources/Dx11ResourcesManager.h"
#include "../Resources/Dx11Texture.h"
#include "../Resources/Dx11Mesh.h"
#include "../Shader/Dx11Shader.h"
#include "../Shader/Dx11ShaderManager.h"
#include "Dx11Renderer.h"
#include "../GameObject/Dx11GameObject.h"

DX11_USING

CDx11UIBar::CDx11UIBar()
{
	SetTypeName<CDx11UIBar>();
}

CDx11UIBar::CDx11UIBar(const CDx11UIBar & ui)	:
	CDx11UI(ui)
{
	m_fValue = ui.m_fValue;
	m_fMin = ui.m_fMin;
	m_fMax = ui.m_fMax;
}

CDx11UIBar::~CDx11UIBar()
{
}

void CDx11UIBar::SetBarDir(UIBAR_DIR eDir)
{
	m_eBarDir = eDir;
}

void CDx11UIBar::SetMin(float fMin)
{
	m_fMin = fMin;
}

void CDx11UIBar::SetMax(float fMax)
{
	m_fMax = fMax;
}

void CDx11UIBar::SetValue(float fValue)
{
	m_fEmpty = m_fValue - fValue;
	m_fValue = fValue;

	ComputeScale();
}

void CDx11UIBar::SetInfo(float fMin, float fMax, float fValue)
{
	m_fMin = fMin;
	m_fMax = fMax;
	m_fValue = fValue;
	m_vMaxScale = m_pTransform->GetWorldScale();
}

void CDx11UIBar::AddValue(float fValue)
{
	m_fEmpty = -fValue;
	float	fValue1 = m_fValue + fValue;

	if (fValue1 > m_fMax)
	{
		m_fEmpty = m_fValue - m_fMax;
		m_fValue = m_fMax;
	}

	else if (fValue1 < m_fMin)
	{
		m_fEmpty = m_fMin - m_fValue;
		m_fValue = m_fMin;
	}

	else
		m_fValue = fValue1;
	
	ComputeScale();
}

bool CDx11UIBar::SetBarTexture(TCHAR * pFileName, const string & strKey,
	const string & strPathKey)
{
	CDx11Renderer*	pRenderer = (CDx11Renderer*)m_pGameObject->FindComponentFromType(CT_RENDERER);

	// 기존에 기본재질이 지정되어 있다면 지우고 생성한다.
	pRenderer->DeleteMaterial();
	pRenderer->CreateMaterial(strKey, pFileName, strPathKey);

	SAFE_RELEASE(pRenderer);

	return true;
}

bool CDx11UIBar::SetBarTexture(const string & strKey)
{
	CDx11Renderer*	pRenderer = (CDx11Renderer*)m_pGameObject->FindComponentFromType(CT_RENDERER);

	// 기존에 기본재질이 지정되어 있다면 지우고 생성한다.
	pRenderer->DeleteMaterial();
	pRenderer->CreateMaterial(strKey);

	SAFE_RELEASE(pRenderer);

	return true;
}

bool CDx11UIBar::SetMesh(const string & strKey, void * pVertices, 
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

void CDx11UIBar::ComputeScale()
{
	Vec3	vScale = m_pTransform->GetWorldScale();

	m_vMaxScale.Max(vScale);
	//m_vMaxScale = vScale;

	Vec3	vWorldPos = m_pTransform->GetWorldPos();

	// 현재 값과 구간값의 비율을 구해준다.
	float	fValue = m_fValue - m_fMin;
	float	fLength = m_fMax - m_fMin;
	Vec3	vMove;

	switch (m_eBarDir)
	{
	case UBD_LEFT:
		m_pTransform->Scaling(fValue / fLength * m_vMaxScale.x,
			m_vMaxScale.y, m_vMaxScale.z);
		break;
	case UBD_RIGHT:
		m_pTransform->Scaling(fValue / fLength * m_vMaxScale.x,
			m_vMaxScale.y, m_vMaxScale.z);
		vMove = VEC3ZERO;
		vMove.x = fValue / fLength * m_vMaxScale.x;
		m_pTransform->MoveWorld(vMove);
		break;
	case UBD_UP:
		m_pTransform->Scaling(m_vMaxScale.x,
			fValue / fLength * m_vMaxScale.y, m_vMaxScale.z);
		break;
	case UBD_DOWN:
		if (m_fValue >= m_fMin && m_fValue <= m_fMax)
		{
			m_pTransform->Scaling(m_vMaxScale.x,
				fValue / fLength * m_vMaxScale.y, m_vMaxScale.z);
			vMove = VEC3ZERO;
			vMove.y = m_fEmpty / fLength * m_vMaxScale.y;
			m_pTransform->MoveWorld(vMove);
		}
		break;
	}

	m_fEmpty = 0.f;
}

bool CDx11UIBar::Init()
{
	if (!CDx11UI::Init())
		return false;

	m_eBarDir = UBD_LEFT;

	m_fValue = 1.f;
	m_fMin = 0.f;
	m_fMax = 1.f;
	
	m_vMaxScale = Vec3(100.f, 20.f, 1.f);
	m_pTransform->Scaling(m_vMaxScale);

	CDx11Renderer*	pRenderer = m_pGameObject->AddComponent<CDx11Renderer>("BarRenderer");
	
	pRenderer->SetRenderState(DX11_RS_DEPTH_DISABLE, RSF_DEPTHSTENCIL);
	pRenderer->SetCameraTag("UICamera");
	pRenderer->SetShader("UIShader");

	SAFE_RELEASE(pRenderer);

	CDx11UI::SetMesh("UIMesh");
	SetBarTexture(L"Bar.png", "DefaultBarTex", DX11_TEXTURE_PATH);

	return true;
}

int CDx11UIBar::Update(float fTime)
{
	return 0;
}

int CDx11UIBar::LateUpdate(float fTime)
{
	return 0;
}

void CDx11UIBar::Render(float fTime)
{
}

CDx11UIBar * CDx11UIBar::Clone()
{
	return new CDx11UIBar(*this);
}
