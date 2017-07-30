#include "Dx11Collider.h"
#include "../Resources/Dx11Mesh.h"
#include "../Rendering/Dx11RenderState.h"
#include "../Shader/Dx11Shader.h"
#include "../Rendering/Dx11RenderManager.h"
#include "../Shader/Dx11ShaderManager.h"
#include "../Scene/Dx11Scene.h"
#include "Dx11Camera.h"
#include "../GameObject/Dx11GameObject.h"
#include "../Core/Dx11CollisionManager.h"


DX11_USING

CDx11Collider::CDx11Collider()
{
	m_eComType = CT_COLLIDER;

	m_pMesh = NULL;
	m_pState = NULL;
	m_pShader = NULL;
	m_vColor = Vec4(1.f, 0.f, 0.f, 1.f);
	m_bSetColor = false;
	m_bCallBack = false;

}

CDx11Collider::CDx11Collider(const CDx11Collider & collider)	:
	CDx11Component(collider)
{
	*this = collider;

	if (m_pMesh)
		m_pMesh->AddRef();

	if (m_pState)
		m_pState->AddRef();

	if (m_pShader)
		m_pShader->AddRef();

}

CDx11Collider::~CDx11Collider()
{
	list<CDx11Collider*>::iterator	iter;
	list<CDx11Collider*>::iterator	iterEnd = m_CollList.end();

	for (iter = m_CollList.begin(); iter != iterEnd; ++iter)
	{
		(*iter)->EraseCollision(this);
	}

	SAFE_RELEASE(m_pMesh);
	SAFE_RELEASE(m_pState);
	SAFE_RELEASE(m_pShader);

}

void CDx11Collider::SetColor(const Vec4 & vColor)
{
	m_vColor = vColor;
}

void CDx11Collider::SetColor(float r, float g, float b, float a)
{
	m_vColor.x = r;
	m_vColor.y = g;
	m_vColor.z = b;
	m_vColor.w = a;

}

void CDx11Collider::ColorEnable()
{
	m_bSetColor = true;
}

COLLIDER_SUB_TYPE CDx11Collider::GetCollType()
{
	return m_eCollType;
}

void CDx11Collider::AddCollision(CDx11Collider * pColl)
{
	// 아래 함수는 충돌체를 가지고 있는 오브젝트의 카운트를 추가하기 위해서
	// 호출했다.
	//pColl->GetGameObject();
	m_CollList.push_back(pColl);
}

bool CDx11Collider::CollsionInCollider(CDx11Collider * pColl)
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

void CDx11Collider::EraseCollision(CDx11Collider * pColl)
{
	list<CDx11Collider*>::iterator	iter;
	list<CDx11Collider*>::iterator	iterEnd = m_CollList.end();

	for (iter = m_CollList.begin(); iter != iterEnd; ++iter)
	{
		if (*iter == pColl)
		{
			m_CollList.erase(iter);
			break;
		}
	}
}

void CDx11Collider::DestroyObject()
{
	list<CDx11Collider*>::iterator	iter;
	list<CDx11Collider*>::iterator	iterEnd = m_CollList.end();

	for (iter = m_CollList.begin(); iter != iterEnd;)
	{
		CDx11GameObject*	pObj = (*iter)->GetGameObject();
		if (pObj->Release() == 1)
		{
			SAFE_RELEASE(pObj);
			iter = m_CollList.erase(iter);
		}

		else
			++iter;
	}
}

void CDx11Collider::SetTransform(CDx11Transform * pTransform)
{
	CDx11Component::SetTransform(pTransform);
}

bool CDx11Collider::Init()
{
	m_pState = DX11_GET_SINGLE(CDx11RenderManager)->FindRS(DX11_RS_WIREFRAME);
	m_pShader = DX11_GET_SINGLE(CDx11ShaderManager)->FindShader("ColliderShader");

	return true;
}

void CDx11Collider::Render(float fTime)
{
	if (!DX11_GET_SINGLE(CDx11CollisionManager)->GetColliderRender())
		return;

	if (m_pState)
		m_pState->SetState();

	if (m_pShader)
	{
		if (m_bSetColor)
		{
			m_pShader->UpdateConstantBuffer("Color", &m_vColor, ST_PIXEL);
			m_bSetColor = false;
		}

		else
		{
			Vec4	vColor(1.f, 1.f, 1.f, 1.f);
			m_pShader->UpdateConstantBuffer("Color", &vColor, ST_PIXEL);
		}
		m_pShader->SetShader();
	}

	if (m_pMesh)
	{
		m_pMesh->Render();
	}

	if (m_pShader)
		m_pShader->End();

	if (m_pState)
		m_pState->ResetState();
}

bool CDx11Collider::CollisionSphereToSphere(const Vec3 & vC1, float fR1,
	const Vec3 & vC2, float fR2)	const
{
	Vec3	vDist = vC2 - vC1;
	float	fLength = vDist.Length();

	return fR1 + fR2 >= fLength;
}

bool CDx11Collider::CollisionRectToRect(const Vec2 & vLT1, 
	const Vec2 & vRB1, const Vec2 & vLT2, const Vec2 & vRB2) const
{
	if (vLT1.x > vRB2.x)
		return false;

	else if (vRB1.x < vLT2.x)
		return false;

	else if (vLT1.y > vRB2.y)
		return false;

	else if (vRB1.y < vLT2.y)
		return false;

	return true;
}

bool CDx11Collider::CollisionPointToRect(const Vec2 & vPoint, 
	const Vec2 & vLT, const Vec2 & vRB)	const
{
	if (vPoint.x < vLT.x)
		return false;

	else if (vPoint.x > vRB.x)
		return false;

	else if (vPoint.y < vLT.y)
		return false;

	else if (vPoint.y > vRB.y)
		return false;

	return true;
}

bool CDx11Collider::CollisionRayToSphere(const RAY & tRay, const Vec3 & vCenter, float fRadius) const
{
	/*
	||p - c|| - r = 0
	||o + dt - c|| - r = 0

	// o - c : m
	(m + dt) * (m + dt) - r * r = 0
	dt^2 + 2mdt + m^2 - r^2 = 0
	*/
	float	b, c;
	Vec3	vM = tRay.vPos - vCenter;

	b = 2.f * vM.Dot(tRay.vDir);
	c = vM.Dot(vM) - fRadius * fRadius;

	float	fDet = sqrtf(b * b - 4.f * c);

	if (fDet < 0.f)
		return false;

	float t0, t1;
	t0 = (-b + fDet) / 2.f;
	t1 = (-b - fDet) / 2.f;

	if (t0 >= 0.f || t1 >= 0.f)
		return true;

	return false;
}

bool CDx11Collider::CollisionRayToTriangle(RAY & tRay, const Vec3 & vP0,
	const Vec3 & vP1, const Vec3 & vP2)	const
{
	float	u, v;

	Vec3	vEdge1 = vP1 - vP0;
	Vec3	vEdge2 = vP2 - vP0;

	Vec3	vCross;
	vCross = tRay.vDir.Cross(vEdge2);

	float	fDet = vEdge1.Dot(vCross);
	
	Vec3	vP0Dir = tRay.vPos - vP0;

	if (fDet <= 0.f)
	{
		vP0Dir = vP0 - tRay.vPos;
		fDet = -fDet;
	}

	if (fDet < 0.0001f)
		return false;

	u = vP0Dir.Dot(vCross);

	if (u < 0.f || u > fDet)
		return false;
	
	Vec3	vCross1 = vP0Dir.Cross(vEdge1);
	v = tRay.vDir.Dot(vCross1);

	if (v < 0.f || u + v > fDet)
		return false;

	tRay.fDist = vEdge2.Dot(vCross1);
	tRay.fDist *= (1.f / fDet);

	tRay.vIntersect = tRay.vPos + tRay.vDir * tRay.fDist;

	return true;
}

bool CDx11Collider::CollisionAABB(const Vec3 & vMin1, const Vec3 & vMax1, const Vec3 & vMin2, const Vec3 & vMax2) const
{
	if (vMin1.x > vMax2.x)
		return false;

	else if (vMin2.x > vMax1.x)
		return false;

	else if (vMin1.y > vMax2.y)
		return false;

	else if (vMin2.y > vMax1.y)
		return false;

	else if (vMin1.z > vMax2.z)
		return false;

	else if (vMin2.z > vMax1.z)
		return false;

	return true;
}

bool CDx11Collider::CollisionOBB(const Vec3 & vCenter1, const Vec3 vAxis1[], const float fAxisLen1[], const Vec3 & vCenter2, const Vec3 vAxis2[], const float fAxisLen2[]) const
{
	// 구 충돌을 통해 먼저 비충돌을 확인한다.
	float fRadius1 = 0.f;
	float fRadius2 = 0.f;
	for (int i = 0; i < 3; ++i)
	{
		if (fRadius1 < fAxisLen1[i])
			fRadius1 = fAxisLen1[i];

		if (fRadius2 < fAxisLen2[i])
			fRadius2 = fAxisLen2[i];
	}

	if (vCenter1.Distance(vCenter2) > abs(fRadius1 - fRadius2) * 2)
		return false;



	double c[3][3];
	double absC[3][3];
	double d[3];
	double r0, r1, r;					// 구간 반경과 구간 중심점 사이의 거리

	const double cutoff = 0.999999;
	bool existsParallelPair = false;
	Vec3 diff = vCenter1 - vCenter2;	// 바운딩박스 중심좌표의 차

										// 면과 수직인 분리축을 찾아본다.
										// 면의 법선벡터는 3개이므로 3번 for루프를 돌린다.
										// 1번 바운딩 박스 기준
	for (int i = 0; i < 3; ++i)
	{
		c[0][i] = vAxis1[0].Dot(vAxis2[i]);
		absC[0][i] = abs(c[0][i]);
		if (absC[0][i] > cutoff)
			existsParallelPair = true;
	}
	d[0] = diff.Dot(vAxis1[0]);
	r = abs(d[0]);
	r0 = fAxisLen1[0];
	r1 = fAxisLen2[0] * absC[0][0] +
		fAxisLen2[1] * absC[0][1] +
		fAxisLen2[2] * absC[0][2];
	if (r > r0 + r1)
		return false;

	for (int i = 0; i < 3; ++i)
	{
		c[1][i] = vAxis1[1].Dot(vAxis2[i]);
		absC[1][i] = abs(c[1][i]);
		if (absC[1][i] > cutoff)
			existsParallelPair = true;
	}
	d[1] = diff.Dot(vAxis1[1]);
	r = abs(d[1]);
	r0 = fAxisLen1[1];
	r1 = fAxisLen2[0] * absC[1][0] +
		fAxisLen2[1] * absC[1][1] +
		fAxisLen2[2] * absC[1][2];
	if (r > r0 + r1)
		return false;

	for (int i = 0; i < 3; ++i)
	{
		c[2][i] = vAxis1[2].Dot(vAxis2[i]);
		absC[2][i] = abs(c[2][i]);
		if (absC[2][i] > cutoff)
			existsParallelPair = true;
	}
	d[2] = diff.Dot(vAxis1[2]);
	r = abs(d[2]);
	r0 = fAxisLen1[2];
	r1 = fAxisLen2[0] * absC[2][0] +
		fAxisLen2[1] * absC[2][1] +
		fAxisLen2[2] * absC[2][2];
	if (r > r0 + r1)
		return false;


	// 2번 바운딩 박스 기준
	r = abs(diff.Dot(vAxis2[0]));
	r0 = fAxisLen1[0] * absC[0][0] +
		fAxisLen1[1] * absC[1][0] +
		fAxisLen1[2] * absC[2][0];
	r1 = fAxisLen2[0];
	if (r > r0 + r1)
		return false;

	r = abs(diff.Dot(vAxis2[1]));
	r0 = fAxisLen1[0] * absC[0][1] +
		fAxisLen1[1] * absC[1][1] +
		fAxisLen1[2] * absC[2][1];
	r1 = fAxisLen2[1];
	if (r > r0 + r1)
		return false;

	r = abs(diff.Dot(vAxis2[2]));
	r0 = fAxisLen1[0] * absC[0][2] +
		fAxisLen1[1] * absC[1][2] +
		fAxisLen1[2] * absC[2][2];
	r1 = fAxisLen2[2];
	if (r > r0 + r1)
		return false;



	if (existsParallelPair)
		return true;


	// 바운딩 박스의 모서리와 수직인 분리축을 찾는다. (3x3 = 9번)
	r = abs(d[2] * c[1][0] - d[1] * c[2][0]);
	r0 = fAxisLen1[1] * absC[2][0] + fAxisLen1[2] * absC[1][0];
	r1 = fAxisLen2[1] * absC[0][2] + fAxisLen2[2] * absC[0][1];
	if (r > r0 + r1)
		return false;

	r = abs(d[2] * c[1][1] - d[1] * c[2][1]);
	r0 = fAxisLen1[1] * absC[2][1] + fAxisLen1[2] * absC[1][1];
	r1 = fAxisLen2[0] * absC[0][2] + fAxisLen2[2] * absC[0][0];
	if (r > r0 + r1)
		return false;

	r = abs(d[2] * c[1][2] - d[1] * c[2][2]);
	r0 = fAxisLen1[1] * absC[2][2] + fAxisLen1[2] * absC[1][2];
	r1 = fAxisLen2[0] * absC[0][1] + fAxisLen2[1] * absC[0][0];
	if (r > r0 + r1)
		return false;

	r = abs(d[0] * c[2][0] - d[2] * c[0][0]);
	r0 = fAxisLen1[0] * absC[2][0] + fAxisLen1[2] * absC[0][0];
	r1 = fAxisLen2[1] * absC[1][2] + fAxisLen2[2] * absC[1][1];
	if (r > r0 + r1)
		return false;

	r = abs(d[0] * c[2][1] - d[2] * c[0][1]);
	r0 = fAxisLen1[0] * absC[2][1] + fAxisLen1[2] * absC[0][1];
	r1 = fAxisLen2[0] * absC[1][2] + fAxisLen2[2] * absC[1][0];
	if (r > r0 + r1)
		return false;

	r = abs(d[0] * c[2][2] - d[2] * c[0][2]);
	r0 = fAxisLen1[0] * absC[2][2] + fAxisLen1[2] * absC[0][2];
	r1 = fAxisLen2[0] * absC[1][1] + fAxisLen2[1] * absC[1][0];
	if (r > r0 + r1)
		return false;

	r = abs(d[1] * c[0][0] - d[0] * c[1][0]);
	r0 = fAxisLen1[0] * absC[1][0] + fAxisLen1[1] * absC[0][0];
	r1 = fAxisLen2[1] * absC[2][2] + fAxisLen2[2] * absC[2][1];
	if (r > r0 + r1)
		return false;

	r = abs(d[1] * c[0][1] - d[0] * c[1][1]);
	r0 = fAxisLen1[0] * absC[1][1] + fAxisLen1[1] * absC[0][1];
	r1 = fAxisLen2[0] * absC[2][2] + fAxisLen2[2] * absC[2][0];
	if (r > r0 + r1)
		return false;

	r = abs(d[1] * c[0][2] - d[0] * c[1][2]);
	r0 = fAxisLen1[0] * absC[1][2] + fAxisLen1[1] * absC[0][2];
	r1 = fAxisLen2[0] * absC[2][1] + fAxisLen2[1] * absC[2][0];
	if (r > r0 + r1)
		return false;

	return true;
}

bool CDx11Collider::CollisionFrustrumToSphere(Frustrum & f, const Vec3 & c, float r) const
{
	return f.IsInSphere(c, r);
}
