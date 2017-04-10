#include "Dx11Collider.h"
#include "../Resources/Dx11Mesh.h"
#include "../Rendering/Dx11RenderState.h"
#include "../Rendering/Dx11Shader.h"
#include "../Rendering/Dx11ShaderManager.h"
#include "../Rendering/Dx11RenderManager.h"
#include "../Component/Dx11Camera.h"
#include "../Core/Dx11CollisionManager.h"
#include "../Scene/Dx11Scene.h"
#include "../GameObject/Dx11GameObject.h"

DX11_USING

CDx11Collider::CDx11Collider()
{
	m_eComType = CT_COLLIDER;
//#if defined(DEBUG) | defined(_DEBUG)
	m_pMesh = NULL;
	m_pState = NULL;
	m_pShader = NULL;
	m_vColor = Vec4(1.f, 0.f, 0.f, 1.f);
	m_bSetColor = false;
//#endif
}

CDx11Collider::CDx11Collider(const CDx11Collider & collider)	:
	CDx11Component(collider)
{
	*this = collider;
	//m_eSubType = collider.m_eSubType;
	//m_vPrevPos = collider.m_vPrevPos;
	//m_CollList.clear();

//#if defined(DEBUG) | defined(_DEBUG)
	if (m_pMesh)
		m_pMesh->AddRef();

	if (m_pState)
		m_pState->AddRef();

	if (m_pShader)
		m_pShader->AddRef();
//#endif
}


CDx11Collider::~CDx11Collider()
{
	list<CDx11Collider*>::iterator	iter;
	list<CDx11Collider*>::iterator	iterEnd = m_CollList.end();

	for (iter = m_CollList.begin(); iter != iterEnd; ++iter)
	{
		(*iter)->EraseCollider(this);
	}

	m_CollList.clear();

//#if defined(DEBUG) | defined(_DEBUG)
	SAFE_RELEASE(m_pMesh);
	SAFE_RELEASE(m_pState);
	SAFE_RELEASE(m_pShader);
//#endif
}

void CDx11Collider::SetColor(const Vec4& vColor)
{
//#if defined(DEBUG) | defined(_DEBUG)
	m_vColor = vColor;
//#endif
}

void CDx11Collider::ColorEnable()
{
//#if defined(DEBUG) | defined(_DEBUG)
	m_bSetColor = true;
//#endif
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
//#if defined(DEBUG) | defined(_DEBUG)
	m_vColor = White;
	m_matWorld = XMMatrixIdentity();
	m_pState = DX11_GET_SINGLE(CDx11RenderManager)->FindRenderState(DX11_RS_WIREFRAME);
	m_pShader = DX11_GET_SINGLE(CDx11ShaderManager)->FindShader("ColliderShader");
//#endif	

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
//#if defined(DEBUG) | defined(_DEBUG)
	if (!DX11_GET_SINGLE(CDx11CollisionManager)->GetColliderRender())
		return;

	//if (m_bSetColor)
	//{
	//	m_pShader->UpdateConstantBuffer("Color", &m_vColor, ST_PIXEL);
	//	m_bSetColor = false;
	//}
	//else
	//{
	//	Vec4	vColor(1.f, 1.f, 1.f, 1.f);
	//	m_pShader->UpdateConstantBuffer("Color", &vColor, ST_PIXEL);
	//}

	m_pShader->UpdateConstantBuffer("Color", &m_vColor, ST_PIXEL);
	
	if(m_pShader)
		m_pShader->SetShader();
	
	if (m_pState)
		m_pState->SetState();

	if (m_pMesh)
		m_pMesh->Render();

	if (m_pShader)
		m_pShader->End();

	if (m_pState)
		m_pState->ResetState();

//#endif
}

bool CDx11Collider::CollisionSphereToSphere(
	const Vec3 & vC1, float fR1, 
	const Vec3 & vC2, float fR2)
{
	float fDist = vC1.Distance(vC2);

	return fDist <= fR1 + fR2;
}

bool CDx11Collider::CollisionRectToRect(
	const Vec2 & vLT1, const Vec2 & vRB1, 
	const Vec2 & vLT2, const Vec2 & vRB2)
{
	if (vLT1.x > vRB2.x)
		return false;

	else if (vLT2.x > vRB1.x)
		return false;

	else if (vLT1.y > vRB2.y)
		return false;

	else if (vLT2.y > vRB1.y)
		return false;

	return true;
}

bool CDx11Collider::CollisionPointToRect(
	const Vec2 & vPoint, 
	const Vec2 & vLT, const Vec2 & vRB)
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

bool CDx11Collider::CollisionAABB(
	const Vec3 & vMin1, const Vec3 & vMax1, 
	const Vec3 & vMin2, const Vec3 & vMax2)
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

bool CDx11Collider::CollisionOBB(
	const Vec3 & vCenter1, const Vec3 vAxis1[], const float fAxisLen1[], 
	const Vec3 & vCenter2, const Vec3 vAxis2[], const float fAxisLen2[])
{
	// 인자로 두 바운딩 박스의 월드 중심 좌표, 3개의 축, 각 축의 길이(스케일값)이 들어온다.


	double c[3][3];			
	double absC[3][3];		
	double d[3];
	double r0, r1, r;					// 구간 반경과 구간 중심점 사이의 거리
	
	const double cutoff = 0.999999;
	bool existsParallelPair = false;
	Vec3 diff = vCenter2 - vCenter1;	// 바운딩박스 중심좌표의 차

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
