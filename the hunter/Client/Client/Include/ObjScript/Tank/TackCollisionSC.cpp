#include "TackCollisionSC.h"
#include "Resources/Dx11Mesh.h"
#include "Resources/Dx11ResourcesManager.h"
#include "GameObject/Dx11GameObject.h"
#include "Component/Dx11Renderer.h"
#include "Component/Dx11Transform.h"
#include "Scene/Dx11Scene.h"
#include "Scene/Dx11layer.h"
#include "Component/Dx11Terrain.h"
#include "Component/Dx11ColliderSphere.h"
#include "Component/Dx11ColliderOBB.h"
#include "Component/Dx11UIBar.h"
#include "Component/Dx11BillBoard.h"
#include "Component/DX11FSM.h"
#include "Component/Dx11ColliderFrustrum.h"
#include "Component/Dx11ColliderRay.h"
#include "Core/Dx11Input.h"
#include "Core/Dx11Debug.h"

#include "Component/Dx11Camera.h"

#include "TankCameraSC.h"
#include "../Bullet/BulletSC.h"

#include "../../Network/NetManager.h"

CTankCollisionSC::CTankCollisionSC():
	m_HP(1.f)
{
}

CTankCollisionSC::CTankCollisionSC(const CTankCollisionSC & script)
{
}

CTankCollisionSC::~CTankCollisionSC()
{
}

bool CTankCollisionSC::Init()
{
	return true;
}

int CTankCollisionSC::Update(float fTime)
{
	return 0;
}

int CTankCollisionSC::LateUpdate(float fTime)
{
	CDx11GameObject* pTank = m_pGameObject->GetParent();
	CDx11Transform* pTankTr = pTank->GetTransform();

	vPrePos = pTankTr->GetWorldPos();
	SAFE_RELEASE(pTankTr);
	SAFE_RELEASE(pTank);

	return 0;
}

void CTankCollisionSC::Render(float fTime)
{
}

CTankCollisionSC * CTankCollisionSC::Clone()
{
	return new CTankCollisionSC(*this);
}

void CTankCollisionSC::ShakeCamera()
{
	// 씬에서 메인 카메라 얻어오기
	CDx11GameObject*	pCameraObj = m_pScene->GetCamera();
	CDx11Camera*	pCamera = (CDx11Camera*)pCameraObj->FindComponentFromTypeName("CDx11Camera");

	if (pCamera->GetShakeTimes() == 0)
		pCamera->SetShakeTimes(80);

	SAFE_RELEASE(pCamera);
	SAFE_RELEASE(pCameraObj);
}

float CTankCollisionSC::GetHP()
{
	return m_HP;
}

void CTankCollisionSC::OnCollisionEnter(CDx11Collider * pMy, CDx11Collider * pYour, float fTime)
{
	if (pMy->GetTag() == PLAYER_BODY )
	{
		if (pYour->GetTag() == MONSTER_ATTACK_BOX) {
			ShakeCamera();
			m_HP -= 0.1;

			if (m_HP <= 0.f)
			{
				CNetManager::GetInst()->myDeath();
			}
		}

		if (pYour->GetTag() == BULLET_BODY) {
			ShakeCamera();
			m_HP -= 0.1;

			if (m_HP <= 0.f)
			{
				CNetManager::GetInst()->myDeath();
			}
		}
	}
}

void CTankCollisionSC::OnCollision(CDx11Collider * pMy, CDx11Collider * pYour, float fTime)
{
	if (pMy->GetTag() == PLAYER_BODY   &&
		pYour->GetTag() == STATIC_MESH)
	{
		ShakeCamera();

		//CDx11GameObject* pTank = m_pGameObject->GetParent();
		//CDx11Transform* pTankTr = pTank->GetTransform();

		//pTankTr->SetWorldPos(vPrePos);

		//SAFE_RELEASE(pTankTr);
		//SAFE_RELEASE(pTank);
	}
}

void CTankCollisionSC::OnCollisionExit(CDx11Collider * pMy, CDx11Collider * pYour, float fTime)
{
}
