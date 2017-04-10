#include "Dx11ColliderRect.h"
#include "Dx11Transform.h"
#include "../GameObject/Dx11GameObject.h"
#include "Dx11ColliderPoint.h"

//#if defined(_DEBUG) | defined(DEBUG)
#include "../Resources/Dx11Mesh.h"
#include "../Resources/Dx11ResourcesManager.h"
#include "../Rendering/Dx11Shader.h"
#include "../Scene/Dx11Scene.h"
#include "Dx11Camera.h"
//#endif

DX11_USING

CDx11ColliderRect::CDx11ColliderRect()	:
	m_vLT(0.f, 0.f),
	m_vRB(0.f, 0.f)
{
	m_eSubType = CST_RECT;
	SetTypeName<CDx11ColliderRect>();
}


CDx11ColliderRect::CDx11ColliderRect(const CDx11ColliderRect & rect)	:
	CDx11Collider(rect)
{
	m_vLT = rect.m_vLT;
	m_vRB = rect.m_vRB;
	m_vPrevPos = rect.m_vPrevPos;
}

CDx11ColliderRect::~CDx11ColliderRect()
{
}

void CDx11ColliderRect::SetRectInfo(float l, float t, float r, float b)
{
	m_vLT = Vec2(l, t);
	m_vRB = Vec2(r, b);
	m_vPrevPos.x = l;
	m_vPrevPos.y = t;
}

Vec2 CDx11ColliderRect::GetLT()
{
	return m_vLT;
}

Vec2 CDx11ColliderRect::GetRB()
{
	return m_vRB;
}

bool CDx11ColliderRect::Init()
{
//#if defined(DEBUG) | defined(_DEBUG)
	m_pMesh = DX11_GET_SINGLE(CDx11ResourcesManager)->FindMesh("RectUIMesh");
//#endif

	if (!CDx11Collider::Init())
		return false;

	m_vLT = Vec2(0.f, 0.f);
	m_vRB = Vec2(0.f, 0.f);
	m_vPrevPos = Vec3Zero;

	return true;
}

void CDx11ColliderRect::Input(float fTime)
{
}

int CDx11ColliderRect::Update(float fTime)
{
	return 0;
}

int CDx11ColliderRect::LateUpdate(float fTime)
{
	Vec3	vPos = m_pTransform->GetWorldPos();
	Vec3	vDist = vPos - m_vPrevPos;

	m_vLT += Vec2(vDist.x, vDist.y);
	m_vRB += Vec2(vDist.x, vDist.y);
	m_vPrevPos = vPos;

//#if defined(DEBUG) | defined(_DEBUG)
	Matrix	matScale, matTranslate;

	matScale = XMMatrixScaling(m_vRB.x - m_vLT.x, m_vRB.y - m_vLT.y ,1.f);
	matTranslate = XMMatrixTranslation(m_vLT.x, m_vLT.y, 0.f);
	m_matWorld = matScale * matTranslate;
//#endif

	return 0;
}

void CDx11ColliderRect::Render(float fTime)
{
//#if defined(_DEBUG) | defined(DEBUG)
	CDx11GameObject*	pObj = m_pScene->FindCamera("UICamera");

	CDx11Camera*	pCamera = (CDx11Camera*)pObj->FindComponentFromType(CT_CAMERA);

	TRANSFORMCBUFFER	tBuffer = {};
	tBuffer.matView = pCamera->GetViewMatrix();
	tBuffer.matProj = pCamera->GetProjMatrix();

	SAFE_RELEASE(pCamera);
	SAFE_RELEASE(pObj);

	tBuffer.matWorld = m_matWorld;

	tBuffer.matWV = tBuffer.matWorld * tBuffer.matView;
	tBuffer.matWVP = tBuffer.matWV * tBuffer.matProj;

	tBuffer.matWorld = XMMatrixTranspose(tBuffer.matWorld);
	tBuffer.matView = XMMatrixTranspose(tBuffer.matView);
	tBuffer.matProj = XMMatrixTranspose(tBuffer.matProj);

	tBuffer.matWV = XMMatrixTranspose(tBuffer.matWV);
	tBuffer.matWVP = XMMatrixTranspose(tBuffer.matWVP);

	m_pShader->UpdateConstantBuffer("Transform", &tBuffer, ST_VERTEX);
//#endif


	CDx11Collider::Render(fTime);
}

CDx11ColliderRect * CDx11ColliderRect::Clone()
{
	return new CDx11ColliderRect(*this);
}

bool CDx11ColliderRect::Collision(CDx11Collider * pColl)
{
	switch (pColl->GetColliderType())
	{
	case CST_RECT:
		return CollisionRectToRect(m_vLT, m_vRB,
			((CDx11ColliderRect*)pColl)->m_vLT, 
			((CDx11ColliderRect*)pColl)->m_vRB);
	case CST_POINT:
		return CollisionPointToRect(((CDx11ColliderPoint*)pColl)->GetPoint(),
			m_vLT, m_vRB);
	}
	return false;
}
