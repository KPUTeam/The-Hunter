#include "Dx11Collider.h"

#if defined(_DEBUG) | defined(DEBUG)
#include "../Resources/Dx11Mesh.h"
#include "../Rendering/Dx11Shader.h"
#include "../Rendering/Dx11ShaderManager.h"
#include "../Rendering/Dx11RenderState.h"
#include "../Rendering/Dx11RenderManager.h"
#endif

DX11_USING

CDx11Collider::CDx11Collider()
{
	m_eComType = CT_COLLIDER;
#if defined(_DEBUG) | defined(DEBUG)
	m_pMesh = NULL;
	m_pShader = NULL;
	m_pWireFrame = NULL;
#endif
}


CDx11Collider::CDx11Collider(const CDx11Collider & collider)
	:CDx11Component(collider)
{
	m_eSubType = collider.m_eSubType;
	m_vPrevPos = collider.m_vPrevPos;
	m_CollList.clear();
#if defined(_DEBUG) | defined(DEBUG)
	m_pMesh = collider.m_pMesh;
	m_pMesh->AddRef();

	m_pShader = collider.m_pShader;
	m_pShader->AddRef();

	m_pWireFrame = collider.m_pWireFrame;
	m_pWireFrame->AddRef();

	m_vColor = collider.m_vColor;
#endif
}

CDx11Collider::~CDx11Collider()
{
#if defined(_DEBUG) | defined(DEBUG)
	SAFE_RELEASE(m_pMesh);
	SAFE_RELEASE(m_pShader);
	SAFE_RELEASE(m_pWireFrame);
#endif

	list<CDx11Collider*>::iterator	iter;
	list<CDx11Collider*>::iterator	iterEnd = m_CollList.end();

	for (iter = m_CollList.begin(); iter != iterEnd; ++iter)
	{
		(*iter)->EraseCollider(this);
	}
	
	m_CollList.clear();
}

COLLIDER_SUB_TYPE CDx11Collider::GetColliderType()
{
	return m_eSubType;
}

void CDx11Collider::AddCollider(CDx11Collider * pColl)
{
	m_CollList.push_back(pColl);
}

void CDx11Collider::EraseCollider(CDx11Collider * pColl)
{
	list<CDx11Collider*>::iterator	iter;
	list<CDx11Collider*>::iterator	iterEnd = m_CollList.end();

	for (iter = m_CollList.begin(); iter != iterEnd; ++iter)
	{
		if (*iter == pColl)
		{
			m_CollList.erase(iter);
			return;
		}
	}
}

bool CDx11Collider::CheckCollider(CDx11Collider * pColl)
{
	list<CDx11Collider*>::iterator	iter;
	list<CDx11Collider*>::iterator	iterEnd = m_CollList.end();

	for (iter = m_CollList.begin(); iter != iterEnd; ++iter)
	{
		if (*iter == pColl)
			return true;
	}

	return false;
}

bool CDx11Collider::Init()
{
#if defined(_DEBUG) | defined(DEBUG)
	m_matWorld = XMMatrixIdentity();
	m_pShader = DX11_GET_SINGLE(CDx11ShaderManager)->FindShader(COLLIDER_SHADER);
	m_vColor = White;

	m_pWireFrame = DX11_GET_SINGLE(CDx11RenderManager)->FindRenderState(DX11_RS_WIREFRAME);
#endif

	return true;
}

void CDx11Collider::Input(float fTime)
{
}

int CDx11Collider::Update(float fTime)
{
	return 0;
}

int CDx11Collider::LateUpdate(float fTime)
{
	return 0;
}

void CDx11Collider::Render(float fTime)
{
#if defined(_DEBUG) | defined(DEBUG)
	m_pShader->UpdateConstantBuffer("Color", &m_vColor, ST_PIXEL);

	m_pWireFrame->SetState();

	m_pShader->SetShader();
	m_pMesh->Render();

	m_pWireFrame->ResetState();
#endif
}

bool CDx11Collider::CollisionSphereToSphere(const Vec3 & vC1, float fR1, const Vec3 & vC2, float fR2)
{
	float fDist = vC1.Distance(vC2);

	return fDist <= fR1 + fR2;
}
