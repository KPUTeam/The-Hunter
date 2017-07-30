#include "MonsterSC.h"

#include "Component/Dx11Renderer.h"
#include "GameObject/Dx11GameObject.h"
#include "Scene/Dx11Scene.h"
#include "Scene/Dx11layer.h"
#include "Component/Dx11Transform.h"
#include "Core/Dx11Input.h"
#include "Component/Dx11Animation3D.h"
#include "Component/DX11FSM.h"
#include "Component/Dx11Terrain.h"
#include "Component/Dx11Material.h"
#include "Component/Dx11ColliderSphere.h"
#include "Component/Dx11ColliderOBB.h"
#include "Component/Dx11ColliderFrustrum.h"
#include "Component/Dx11UIBar.h"
#include "Component/Dx11BillBoard.h"
#include "Core/Dx11Input.h"
#include "Core/Dx11Debug.h"
#include "../../AIState/IdleState.h"
#include "../../AIState/WalkState.h"
#include "../../AiState/JumpState.h"
#include "../../AIState/DamageState.h"
#include "../../AIState/DeathState.h"

#include "../Bullet/BulletSC.h"

//#include "../../AIState/IdleState.h"
//#include "../../AIState/RunState.h"
//#include "../../AIState/DamageState.h"


CMonsterSC::CMonsterSC():
	m_fHP(1),
	m_pHPBar(NULL),
	m_pFrameBar(NULL)
{
}

CMonsterSC::CMonsterSC(const CMonsterSC & script)
{
	*this = script;
}


CMonsterSC::~CMonsterSC()
{
	SAFE_RELEASE(m_pHPBar);
	SAFE_RELEASE(m_pFrameBar);
}

bool CMonsterSC::Init()
{
	// �ݸ������Ǿ�
	CDx11ColliderSphere* pSphere = m_pGameObject->AddComponent<CDx11ColliderSphere>(MONSTER_VIEW);
	pSphere->SetSphereInfo(5, m_pTransform->GetWorldPos());
	SAFE_RELEASE(pSphere);

	// �浹ü�� �����Ѵ�.
	CDx11ColliderOBB* pOBB = m_pGameObject->AddComponent<CDx11ColliderOBB>(MONSTER_BODY);
	float fAxisLen[3];
	fAxisLen[0] = 0.5;
	fAxisLen[1] = 1.7f;
	fAxisLen[2] = 0.5;
	pOBB->SetOBBInfo(fAxisLen, m_pTransform);
	SAFE_RELEASE(pOBB);

	// ����Ʈ Ʈ�������� �����Ѵ�.
	CDx11Transform* pDefaultTr = m_pGameObject->GetTransform();
	pDefaultTr->Scaling(0, 0, 0);
	SAFE_RELEASE(pDefaultTr);

	// ����Ʈ �������� �߰��Ѵ�.
	CDx11Renderer*	pDefaultRenderer = m_pGameObject->AddComponent<CDx11Renderer>("Renderer");
	pDefaultRenderer->SetMesh("CubeMesh");
	pDefaultRenderer->SetShader("DefaultShader");
	pDefaultRenderer->SetMaterialDiffuseTex("Default");
	SAFE_RELEASE(pDefaultRenderer);

	// �޽�������Ʈ�� ���δ�.
	CDx11GameObject* pMeshObj = CDx11GameObject::Create("pMeshObj");
	m_pGameObject->AddChild(pMeshObj);

	// �޽��� �����Ѵ�.
	CDx11Renderer*	pRenderer = pMeshObj->AddComponent<CDx11Renderer>("Renderer");
	pRenderer->SetMesh("Skeleton", L"Skeleton.FBX");
	pRenderer->SetShader("AnimationTexShader");
	SAFE_RELEASE(pRenderer);

	// �ʱ� Ʈ�������� �����Ѵ�.
	CDx11Transform* pTr = pMeshObj->GetTransform();
	pTr->Scaling(0.012, 0.012, 0.012);	// ũ�⸦ �����Ѵ�.
	pTr->SetWorldRotX(-DX11_PI / 2);	// ȸ������ �����Ѵ�.
	pTr->SetWorldRotY(DX11_PI);			// ȸ������ �����Ѵ�.
	SAFE_RELEASE(pTr);

	// �ִϸ��̼� Ŭ���� �ʱ�ȭ �Ѵ�.
	CDx11Animation3D* pAnimation3D = (CDx11Animation3D*)pMeshObj->FindComponentFromType(CT_ANIMATION3D);

	pAnimation3D->CreateClip("Idle", 0, 2.335);
	pAnimation3D->CreateClip("Idle_Shake", 2.3708187134, 6.4);
	pAnimation3D->CreateClip("Walk", 6.4112280701, 7.4);
	pAnimation3D->CreateClip("Run", 7.4463742689, 8.2);
	pAnimation3D->CreateClip("Attack", 8.3, 9.9);
	pAnimation3D->CreateClip("Damage_Left", 10.017543859, 10.7);
	pAnimation3D->CreateClip("Damage_Right", 10.71877193, 11.4);
	pAnimation3D->CreateClip("Smile", 11.42, 14.1);
	pAnimation3D->CreateClip("Angry", 14.124736842, 16.8);
	pAnimation3D->CreateClip("Jump", 16.829473684, 18.1);
	pAnimation3D->CreateClip("Death", 18.198538011, 19.0); //18.198538011, 19.033333333
	pAnimation3D->ChangeAnimation("Idle");
	SAFE_RELEASE(pAnimation3D);

	SAFE_RELEASE(pMeshObj);

	CreateBar();

	return true;
}

int CMonsterSC::Update(float fTime)
{
	// ������ �����.
	CDx11GameObject* pTerrainObj = m_pScene->FindGameObject("TerrainObj");
	CDx11Terrain* pTerrain = (CDx11Terrain*)pTerrainObj->FindComponentFromTag("TerrainCP");

	// ���� ��ġ������ ����� �븻�� ��ġ������ �����´�.
	TRIANGLE2RAY PosNormal = pTerrain->GetHeight(m_pGameObject);

	// �� ������Ʈ�� �ű��.
	//m_pTransform->SetWorldPos(PosNormal.Pos);
	m_pTransform->SetWorldPos(fTime * 10, PosNormal.Pos);

	// ������
	SAFE_RELEASE(pTerrainObj);
	SAFE_RELEASE(pTerrain);

	SetBarPos();

	return 0;
}

int CMonsterSC::LateUpdate(float fTime)
{
	return 0;
}

void CMonsterSC::Render(float fTime)
{
}

CMonsterSC * CMonsterSC::Clone()
{
	return new CMonsterSC(*this);
}

void CMonsterSC::CreateBar()
{
	// Bar Frame
	m_pFrameBar = CDx11GameObject::Create("FrameBar");
	m_pGameObject->AddChild(m_pFrameBar);

	CDx11Renderer*	pRenderer = m_pFrameBar->AddComponent<CDx11Renderer>("FrameBarRenderer");
	pRenderer->CreateMaterial("FrameBar", L"Health/HBarFrame.png");
	pRenderer->SetMesh(EFFECT_LT_MESH);
	pRenderer->SetShader(UISHADER);
	pRenderer->SetRenderState(DX11_RS_ALPHABLEND, RSF_BLEND);
	//pRenderer->SetRenderState(DX11_RS_DEPTH_DISABLE, RSF_DEPTHSTENCIL);
	SAFE_RELEASE(pRenderer);

	// �ʱ� ũ�⸦ �����Ѵ�.
	CDx11Transform*	pTransform = m_pFrameBar->GetTransform();
	pTransform->Scaling(3.f, 0.6f, 1.f);
	pTransform->MoveWorld(Vec3( 0, 0, 0));
	SAFE_RELEASE(pTransform);

	// ������ ������Ʈ�� �߰��Ѵ�.
	CDx11BillBoard*	pBillBoard = m_pFrameBar->AddComponent<CDx11BillBoard>("FrameBarBillBoard");
	SAFE_RELEASE(pBillBoard);

	// �÷��̾� HP Bar ����
	m_pHPBar = CDx11GameObject::Create("HPBar");
	m_pGameObject->AddChild(m_pHPBar);

	pRenderer = m_pHPBar->AddComponent<CDx11Renderer>("HPBarRenderer");
	pRenderer->CreateMaterial("HPBar2", L"Health/HBarBlue.png");
	pRenderer->SetMesh(EFFECT_LT_MESH);
	pRenderer->SetShader(UISHADER);
	pRenderer->SetRenderState(DX11_RS_ALPHABLEND, RSF_BLEND);
	pRenderer->SetRenderState(DX11_RS_DEPTH_DISABLE, RSF_DEPTHSTENCIL);
	SAFE_RELEASE(pRenderer);

	// �ʱ� ũ�⸦ �����Ѵ�.
	pTransform = m_pHPBar->GetTransform();
	pTransform->Scaling(3., 0.6, 1.f);
	pTransform->MoveWorld(Vec3(0, 0, 0));
	SAFE_RELEASE(pTransform);

	// �����带 �߰��Ѵ�.
	pBillBoard = m_pHPBar->AddComponent<CDx11BillBoard>("HPBarBillBoard");
	SAFE_RELEASE(pBillBoard);
}

void CMonsterSC::SetBarPos()
{
	// HP�� ��ġ�� �����Ѵ�.
	CDx11Transform* pTr = m_pTransform->GetTransform();
	Vec3 vMainObjPos = pTr->GetWorldPos();
	SAFE_RELEASE(pTr);

	// ������
	pTr = m_pFrameBar->GetTransform();

	pTr->SetWorldPos(
		vMainObjPos.x - 1.75,
		vMainObjPos.y + 4 + 0.03,
		vMainObjPos.z);

	SAFE_RELEASE(pTr);

	pTr = m_pHPBar->GetTransform();

	pTr->SetWorldPos(
		vMainObjPos.x - 1.75,
		vMainObjPos.y + 4,
		vMainObjPos.z);

	pTr->Scaling(3 * m_fHP, 0.6f, 1.f);

	SAFE_RELEASE(pTr);
}

void CMonsterSC::SetTarget(CDx11GameObject * pTar)
{
	m_pTarget = pTar;
}

CDx11GameObject * CMonsterSC::GetTarget()
{
	m_pTarget->AddRef();
	return m_pTarget;
}

void CMonsterSC::OnCollisionEnter(CDx11Collider * pMy, CDx11Collider * pYour, float fTime)
{
	if (pMy->GetTag() == MONSTER_VIEW)
	{
		if (pYour->GetTag() == PLAYER_BODY)
		{
			CDx11FSM* pFSM = (CDx11FSM*)m_pGameObject->FindComponentFromType(CT_FSM);

			if (pFSM->GetStateType() == AI_RUN || pFSM->GetStateType() == AI_JUMP) {
				SAFE_RELEASE(pFSM);
				return;
			}
			CDx11GameObject* pBodyObj = pYour->GetGameObject();
			CDx11GameObject* pTankObj = pBodyObj->GetParent();

			m_pTarget = pTankObj;
			SAFE_RELEASE(pTankObj);
			SAFE_RELEASE(pBodyObj);

			CJumpState* pState = pFSM->SetState<CJumpState>(m_pGameObject);
			SAFE_RELEASE(pState);
			SAFE_RELEASE(pFSM);
		}
	}

	if (pMy->GetTag() == MONSTER_BODY)
	{
		if (pYour->GetTag() == BULLET_BODY)
		{
			CDx11FSM* pFSM = (CDx11FSM*)m_pGameObject->FindComponentFromType(CT_FSM);

			if (pFSM->GetStateType() == AI_DAMAGE) {
				SAFE_RELEASE(pFSM);
				return;
			}

			m_fHP -= 0.25;

			if (m_fHP <= 0) {
				CDeathState* pState = pFSM->SetState<CDeathState>(m_pGameObject);
				SAFE_RELEASE(pState);
				SAFE_RELEASE(pFSM);
				return;
			}
		

			CDx11GameObject* pBulletObj =  pYour->GetGameObject();
			CBulletSC* pBulletSC = (CBulletSC*)pBulletObj->FindComponentFromTag("pBulletSC");

			CDx11GameObject* pTankObj =  pBulletSC->GetEmployer();

			m_pTarget = pTankObj;

			SAFE_RELEASE(pTankObj);
			SAFE_RELEASE(pBulletSC);
			SAFE_RELEASE(pBulletObj);

			CDamageState* pState = pFSM->SetState<CDamageState>(m_pGameObject);
			SAFE_RELEASE(pState);
			SAFE_RELEASE(pFSM);
		}
	}
}

void CMonsterSC::OnCollision(CDx11Collider * pMy, CDx11Collider * pYour, float fTime)
{
}

void CMonsterSC::OnCollisionExit(CDx11Collider * pMy, CDx11Collider * pYour, float fTime)
{
}
