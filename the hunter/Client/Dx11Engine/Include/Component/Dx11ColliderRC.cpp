#include "Dx11ColliderRC.h"
#include "Dx11ColliderPoint.h"
#include "Dx11Transform.h"
#include "../GameObject/Dx11GameObject.h"
#include "../Resources/Dx11ResourcesManager.h"
#include "../Scene/Dx11Scene.h"
#include "Dx11Camera.h"
#include "../Shader/Dx11Shader.h"
#include "../Rendering/Dx11RenderManager.h"
#include "../Rendering/Dx11RenderState.h"
#include "../Core/Dx11CollisionManager.h"

DX11_USING

CDx11ColliderRC::CDx11ColliderRC() :
	m_vLT(0.f, 0.f),
	m_vRB(0.f, 0.f)
{
	m_eCollType = CST_RC;
	SetTypeName<CDx11ColliderRC>();
}

CDx11ColliderRC::CDx11ColliderRC(const CDx11ColliderRC & rc)	:
	CDx11Collider(rc)
{
	m_vLT = rc.m_vLT;
	m_vRB = rc.m_vRB;

	m_pDepthState = rc.m_pDepthState;

	if(m_pDepthState)
		m_pDepthState->AddRef();
}

CDx11ColliderRC::~CDx11ColliderRC()
{
	SAFE_RELEASE(m_pDepthState);
}

void CDx11ColliderRC::SetRCInfo(float l, float t, float r, float b)
{
	m_vLT = Vec2(l, t);
	m_vRB = Vec2(r, b);
	m_vPrevPos.x = l;
	m_vPrevPos.y = t;
}

void CDx11ColliderRC::SetRCInfo()
{
	Vec3 Pos = m_pTransform->GetWorldPos();
	Vec3 Sc = m_pTransform->GetWorldScale();

	m_vLT = Vec2(Pos.x, Pos.y);
	m_vRB = Vec2(Pos.x + Sc.x, Pos.y + Sc.y);

	m_vPrevPos.x = m_vLT.x;
	m_vPrevPos.y = m_vLT.y;
}

Vec2 CDx11ColliderRC::GetLT()
{
	return m_vLT;
}

Vec2 CDx11ColliderRC::GetRB()
{
	return m_vRB;
}

bool CDx11ColliderRC::Init()
{
	m_pMesh = DX11_GET_SINGLE(CDx11ResourcesManager)->FindMesh("UIMesh");
	m_pDepthState = DX11_GET_SINGLE(CDx11RenderManager)->FindRS(DX11_RS_DEPTH_DISABLE);


	if (!CDx11Collider::Init())
		return false;

	m_vPrevPos = VEC3ZERO;

	return true;
}

int CDx11ColliderRC::Update(float fTime)
{
	return 0;
}

int CDx11ColliderRC::LateUpdate(float fTime)
{
	Vec3	vPos = m_pTransform->GetWorldPos();
	Vec3	vDist = vPos - m_vPrevPos;

	m_vPrevPos = vPos;

	m_vLT += Vec2(vDist.x, vDist.y);
	m_vRB += Vec2(vDist.x, vDist.y);

	Matrix	matScale, matTrans;

	matScale = XMMatrixScaling(m_vRB.x - m_vLT.x, m_vRB.y - m_vLT.y, 1.f);
	matTrans = XMMatrixTranslation(m_vLT.x, m_vLT.y, 0.f);
	m_matWorld = matScale * matTrans;

	return 0;
}

void CDx11ColliderRC::Render(float fTime)
{
	if (!DX11_GET_SINGLE(CDx11CollisionManager)->GetColliderRender())
		return;

	TRANSFORMCBUFFER	tBuffer = {};

	CDx11GameObject*	pCameraObj = m_pScene->FindCamera("UICamera");
	CDx11Camera*	pCamera = (CDx11Camera*)pCameraObj->FindComponentFromTypeName("CDx11Camera");

	tBuffer.matWorld = m_matWorld;
	tBuffer.matView = pCamera->GetViewMatrix();
	tBuffer.matProj = pCamera->GetProjMatrix();
	tBuffer.matWV = m_matWorld * tBuffer.matView;
	tBuffer.matWVP = tBuffer.matWV * tBuffer.matProj;

	tBuffer.matWorld = XMMatrixTranspose(tBuffer.matWorld);
	tBuffer.matView = XMMatrixTranspose(tBuffer.matView);
	tBuffer.matProj = XMMatrixTranspose(tBuffer.matProj);
	tBuffer.matWV = XMMatrixTranspose(tBuffer.matWV);
	tBuffer.matWVP = XMMatrixTranspose(tBuffer.matWVP);

	SAFE_RELEASE(pCamera);
	SAFE_RELEASE(pCameraObj);
	if (m_pShader)
	{
		m_pShader->UpdateConstantBuffer("Transform", &tBuffer,
			ST_VERTEX);
	}
	
	m_pDepthState->SetState();

	CDx11Collider::Render(fTime);

	m_pDepthState->ResetState();

}

CDx11ColliderRC * CDx11ColliderRC::Clone()
{
	return new CDx11ColliderRC(*this);
}

bool CDx11ColliderRC::Collision(CDx11Collider * pColl)
{
	switch (pColl->GetCollType())
	{
	case CST_RC:
		return CollisionRectToRect(m_vLT, m_vRB, 
			((CDx11ColliderRC*)pColl)->m_vLT, ((CDx11ColliderRC*)pColl)->m_vRB);
	case CST_POINT:
		return CollisionPointToRect(((CDx11ColliderPoint*)pColl)->GetPoint(),
			m_vLT, m_vRB);
	}

	return false;
}
