#include "Dx11ColliderTerrain.h"
#include "../Resources/Dx11ResourcesManager.h"
#include "Dx11Transform.h"
#include "../Scene/Dx11Scene.h"
#include "Dx11Camera.h"
#include "../GameObject/Dx11GameObject.h"
#include "../Shader/Dx11Shader.h"
#include "Dx11ColliderRay.h"

DX11_USING

CDx11ColliderTerrain::CDx11ColliderTerrain()
{
	m_eCollType = CST_TERRAIN;
	SetTypeName<CDx11ColliderTerrain>();
}

CDx11ColliderTerrain::CDx11ColliderTerrain(const CDx11ColliderTerrain & terrain) :
	CDx11Collider(terrain)
{
	*this = terrain;
	m_iRef = 1;
}

CDx11ColliderTerrain::~CDx11ColliderTerrain()
{
}

void CDx11ColliderTerrain::SetTerrainPos(const vector<Vec3>& vecPos, 
	const vector<UINT>& vecIdx, UINT iW, UINT iH)
{
	m_vecPos = vecPos;
	m_iW = iW;
	m_iH = iH;

	for (int i = 0; i < vecIdx.size() / 3; ++i)
	{
		Vec3	vDir1 = m_vecPos[vecIdx[i * 3] + 1] - m_vecPos[vecIdx[i * 3]];
		Vec3	vDir2 = m_vecPos[vecIdx[i * 3] + 2] - m_vecPos[vecIdx[i * 3]];
		vDir1 = vDir1.Normalize();
		vDir2 = vDir2.Normalize();

		m_vecNormal.push_back(vDir1.Cross(vDir2));
	}
}

bool CDx11ColliderTerrain::Init()
{
#if defined(DEBUG) | defined(_DEBUG1)
	//m_pMesh = DX11_GET_SINGLE(CDx11ResourcesManager)->FindMesh("ColliderSphere");
#endif

	if (!CDx11Collider::Init())
		return false;

	return true;
}

int CDx11ColliderTerrain::Update(float fTime)
{
	return 0;
}

int CDx11ColliderTerrain::LateUpdate(float fTime)
{
#if defined(DEBUG) | defined(_DEBUG1)
#endif

	return 0;
}

void CDx11ColliderTerrain::Render(float fTime)
{
#if defined(DEBUG) | defined(_DEBUG1)
#endif

	CDx11Collider::Render(fTime);
}

CDx11ColliderTerrain * CDx11ColliderTerrain::Clone()
{
	return new CDx11ColliderTerrain(*this);
}

bool CDx11ColliderTerrain::Collision(CDx11Collider * pColl)
{
	CDx11GameObject*	pCollObj = pColl->GetGameObject();

	if (!pCollObj->GetGravity())
	{
		SAFE_RELEASE(pCollObj);
		return false;
	}

	CDx11Transform*	pTransform = pColl->GetTransform();

	Vec3	vPos = pTransform->GetWorldPos();

	Vec3	vTerrainScale = m_pTransform->GetWorldScale();
	Vec3	vTerrainPos = m_pTransform->GetWorldPos();

	// 인덱스를 구한다.
	int	iX = (int)(vPos.x / vTerrainScale.x - vTerrainPos.x);
	int	iY = (m_iH - 1) - ((int)(vPos.z / vTerrainScale.z - vTerrainPos.z) + 1);

	if (iX < 0 || iX >= m_iW || iY < 0 || iY >= m_iH)
		return false;

	int		idx = iY * m_iW + iX;

	// 광선을 만들어준다. 인자로 들어온 충돌체의 위치로부터 Y축방향 아래로 쏘는
	// 광선을 만든다.
	Vec3	vPrevPos = pTransform->GetWorldPos();
	RAY		tRay = {};
	tRay.vPos = Vec3(vPos.x, vPrevPos.y, vPos.z);//vPrevPos;
	tRay.vDir = Vec3(0.f, -1.f, 0.f);

	if (vPrevPos.y < 0.f)
	{
		int a = 10.f;
	}

	// 광선 거리를 구한다.
	float	fRayDist = vPrevPos.Distance(vPos);

	Vec3	vWorldPos[4];
	vWorldPos[0] = m_vecPos[idx] + vTerrainPos;
	vWorldPos[1] = m_vecPos[idx + 1] + vTerrainPos;
	vWorldPos[2] = m_vecPos[idx + m_iW] + vTerrainPos;
	vWorldPos[3] = m_vecPos[idx + m_iW + 1] + vTerrainPos;

	float	fHeight[4];

	fHeight[0] = m_vecPos[idx].y;
	fHeight[1] = m_vecPos[idx + 1].y;
	fHeight[2] = m_vecPos[idx + m_iW].y;
	fHeight[3] = m_vecPos[idx + m_iW + 1].y;

	float	fX = vPos.x / vTerrainScale.x - m_vecPos[idx].x;
	float	fY = m_vecPos[idx].z - vPos.z / vTerrainScale.z;

	// 우 상단 삼각형 체크
	if (fX >= fY)
	{
		if (CollisionRayToTriangle(tRay, vWorldPos[0], vWorldPos[1],
			vWorldPos[3]))
		{
			if (tRay.fDist <= fRayDist && tRay.fDist >= 0.f)
			{
				float fTerrainY = fHeight[0] + (fHeight[3] - fHeight[2]) * fX + (fHeight[2] - fHeight[0]) * fY;
				//pTransform->Move(Vec3(0.f, tRay.vIntersect.y - vPos.y, 0.f));
				//pTransform->Move(Vec3(0.f, fTerrainY - vPos.y, 0.f));

				if (fTerrainY < 0.f)
					int a = 10;
				pTransform->SetWorldPos(vPos.x, fTerrainY, vPos.z);
				// pCollObj->UpdateMatrix();
				pCollObj->ClearGravityTime();
				SAFE_RELEASE(pCollObj);
				SAFE_RELEASE(pTransform);
				return true;
			}

			SAFE_RELEASE(pCollObj);
			SAFE_RELEASE(pTransform);

			return false;
		}
	}

	else
	{
		// 좌 하단 삼각형 체크
		if (CollisionRayToTriangle(tRay, vWorldPos[0], vWorldPos[3],
			vWorldPos[2]))
		{
			if (tRay.fDist <= fRayDist && tRay.fDist >= 0.f)
			{
				pTransform->MoveWorld(Vec3(0.f, tRay.vIntersect.y - vPos.y, 0.f));
				//pTransform->Move(Vec3(0.f, fTerrainY - vPos.y, 0.f));
				//pTransform->SetWorldPos(vPos.x, tRay.vIntersect.y, vPos.z);
				// pCollObj->UpdateMatrix();
				pCollObj->ClearGravityTime();
				SAFE_RELEASE(pCollObj);
				SAFE_RELEASE(pTransform);
				return true;
			}

			SAFE_RELEASE(pCollObj);
			SAFE_RELEASE(pTransform);
			return false;
		}
	}

	SAFE_RELEASE(pCollObj);

	SAFE_RELEASE(pTransform);

	return false;
	/*
	float	fX = vPos.x / vTerrainScale.x - m_vecPos[iY * m_iW + iX].x;
	float	fY = m_vecPos[iY * m_iW + iX].z - vPos.z / vTerrainScale.z;

	float	fHeight[4];

	fHeight[0] = m_vecPos[idx].y;
	fHeight[1] = m_vecPos[idx + 1].y;
	fHeight[2] = m_vecPos[idx + m_iW].y;
	fHeight[3] = m_vecPos[idx + m_iW + 1].y;

	float	fTerrainY = 0.f;

	// 우상단 삼각형일 경우
	if (fX > fY)
		fTerrainY = fHeight[0] + (fHeight[1] - fHeight[0]) * fX + (fHeight[3] - fHeight[1]) * fY;

	else
		fTerrainY	= fHeight[0] + (fHeight[3] - fHeight[2]) * fX + (fHeight[2] - fHeight[0]) * fY;

	pTransform->Move(Vec3(0.f, fTerrainY - vPos.y, 0.f));
	pCollObj->UpdateMatrix();
	pCollObj->ClearGravityTime();

	switch (pColl->GetCollType())
	{
	}

	SAFE_RELEASE(pCollObj);

	SAFE_RELEASE(pTransform);
	*/

	return false;
}
