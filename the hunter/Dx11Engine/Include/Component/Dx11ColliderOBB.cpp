#include "Dx11ColliderOBB.h"
#include "Dx11Transform.h"

//#if defined(_DEBUG) | defined(DEBUG)
#include "../Resources/Dx11Mesh.h"
#include "../Resources/Dx11ResourcesManager.h"
#include "../Rendering/Dx11Shader.h"
#include "../Scene/Dx11Scene.h"
#include "Dx11Camera.h"
#include "../GameObject/Dx11GameObject.h"
//#endif

DX11_USING

CDx11ColliderOBB::CDx11ColliderOBB() :
	m_vCenter(Vec3Zero),
	m_vRotation(Vec3Zero),
	m_vPrevPos(Vec3Zero),
	m_vPrevRot(Vec3One)
{
	m_eSubType = CST_OBB;
	SetTypeName<CDx11ColliderOBB>();

	for (int i = 0; i < 3; ++i)
	{
		m_vAxis[i] = Vec3Zero;
		m_fAxisLen[i] = 0.f;
	}
}


CDx11ColliderOBB::CDx11ColliderOBB(const CDx11ColliderOBB & obb)	:
	CDx11Collider(obb)
{
	m_vCenter = obb.m_vCenter;
	m_vRotation = obb.m_vRotation;
	m_vPrevPos = obb.m_vPrevPos;
	m_vPrevRot = obb.m_vPrevRot;
	for (int i = 0; i < 3; ++i)
	{
		m_vAxis[i] = obb.m_vAxis[i];
		m_fAxisLen[i] = obb.m_fAxisLen[i];
	}
}

CDx11ColliderOBB::~CDx11ColliderOBB()
{
}



void CDx11ColliderOBB::SetOBBInfo(const float fAxisLen[], 
	CDx11Transform * pTransform)
{
	m_vCenter =  pTransform->GetWorldPos();
	m_vRotation = pTransform->GetWorldRot();

	for (int i = 0; i < AXIS_MAX; ++i)
	{
		m_vAxis[i] = pTransform->GetWorldAxis((AXIS)i);
		m_fAxisLen[i] = fAxisLen[i];
	}

	m_vPrevPos = m_pTransform->GetWorldPos();
	m_vPrevRot = m_pTransform->GetWorldRot();
}

bool CDx11ColliderOBB::Init()
{
//#if defined(DEBUG) | defined(_DEBUG)
	m_pMesh = DX11_GET_SINGLE(CDx11ResourcesManager)->FindMesh("CubeMesh");
//#endif

	if (!CDx11Collider::Init())
		return false;


	return true;
}

void CDx11ColliderOBB::Input(float fTime)
{
}

int CDx11ColliderOBB::Update(float fTime)
{
	return 0;
}

int CDx11ColliderOBB::LateUpdate(float fTime)
{
	Vec3	vPos = m_pTransform->GetWorldPos();
	Vec3	vDist = vPos - m_vPrevPos;
	Vec3	vRot = m_pTransform->GetWorldRot();
	Vec3	vAngle = vRot - m_vPrevRot;
	
	m_vCenter += vDist;
	m_vRotation += vAngle;

	m_vPrevPos = vPos;
	m_vPrevRot = vRot;

//#if defined(DEBUG) | defined(_DEBUG)
	Matrix	matScale, matRot, matTranslate;
	XMVECTOR vQuarternion;
	matScale = XMMatrixScaling(m_fAxisLen[0] * 2, m_fAxisLen[1] * 2, m_fAxisLen[2] * 2);
	vQuarternion = XMQuaternionRotationRollPitchYaw(m_vRotation.x, m_vRotation.y, m_vRotation.z);
	matRot = XMMatrixRotationQuaternion(vQuarternion);
	matTranslate = XMMatrixTranslation(m_vCenter.x, m_vCenter.y, m_vCenter.z);
	
	m_matWorld = matScale * matRot * matTranslate;

	Vec3	vAxis[AXIS_MAX] = { Vec3X, Vec3Y, Vec3Z };
	for (int i = 0; i < AXIS_MAX; ++i)
	{
		Vec	v = vAxis[i].Convert();
		v = XMVector3TransformNormal(v, matRot);
		m_vAxis[i].Convert(v);
		m_vAxis[i] = m_vAxis[i].Normalize();
	}
//#endif

	return 0;
}

void CDx11ColliderOBB::Render(float fTime)
{
//#if defined(DEBUG) | defined(_DEBUG)
	CDx11GameObject*	pCameraObj = m_pScene->GetMainCamera();
	CDx11Camera*		pCamera = (CDx11Camera*)pCameraObj->FindComponentFromTypeName("CDx11Camera");

	TRANSFORMCBUFFER	tBuffer = {};

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

	m_pShader->UpdateConstantBuffer("Transform", &tBuffer, ST_VERTEX);
//#endif
	CDx11Collider::Render(fTime);
}

CDx11ColliderOBB * CDx11ColliderOBB::Clone()
{
	return new CDx11ColliderOBB(*this);
}

bool CDx11ColliderOBB::Collision(CDx11Collider * pColl)
{
	switch (pColl->GetColliderType())
	{
	case CST_OBB:
		return CollisionOBB(m_vCenter, m_vAxis, m_fAxisLen, 
			((CDx11ColliderOBB*)pColl)->m_vCenter,
			((CDx11ColliderOBB*)pColl)->m_vAxis, 
			((CDx11ColliderOBB*)pColl)->m_fAxisLen);
	}
	return false;
}
