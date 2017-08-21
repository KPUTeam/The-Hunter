#include "User.h"
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
#include "Core/Dx11TimerManager.h"
#include "Core/Dx11Timer.h"
#include "../Bullet/BulletSC.h"

#include "../../Network/NetManager.h"
#include "../../Network/ZEROStream.h"

CUser::CUser() :
	pBodyObj(NULL),
	pLT(NULL),
	pRT(NULL),
	pLB(NULL),
	pRB(NULL),
	pHeadObj(NULL),

	m_fHP(1),
	m_pHPBar(NULL),
	m_pFrameBar(NULL)
{
}

CUser::CUser(const CUser & user) :
	CDx11Script(user)
{
}


CUser::~CUser()
{
	SAFE_RELEASE(pBodyObj);
	SAFE_RELEASE(pLT);
	SAFE_RELEASE(pRT);
	SAFE_RELEASE(pLB);
	SAFE_RELEASE(pRB);
	SAFE_RELEASE(pHeadObj);

	SAFE_RELEASE(m_pHPBar);
	SAFE_RELEASE(m_pFrameBar);
}

void CUser::AddUserObj(CUser * pObj)
{
	m_UserObj.push_back(pObj);
}

CUser * CUser::FindUserObj(INT64 iSerialNumber)
{
	list<CUser*>::iterator	iter = m_UserObj.begin();
	list<CUser*>::iterator	iterEnd = m_UserObj.end();

	for (auto p = iter; p != iterEnd; ++p)
	{
		if ((*iter)->GetSerialNumber() == iSerialNumber)
		{
			(*iter)->AddRef();
			return *iter;
		}
	}
	return nullptr;
}

bool CUser::CreateBody()
{
	// �ٵ� ����Ѵ�.
	pBodyObj = CDx11GameObject::Create("pUserBodyObj");
	m_pGameObject->AddChild(pBodyObj);

	CDx11Transform* pBodyObjTr = pBodyObj->GetTransform();
	pBodyObjTr->Scaling(0, 0, 0);

	CDx11ColliderSphere* pSphere = pBodyObj->AddComponent<CDx11ColliderSphere>(USER_BODY);
	pSphere->SetSphereInfo(1, pBodyObjTr->GetWorldPos());
	SAFE_RELEASE(pSphere);

	// �浹ü�� �����Ѵ�.
	CDx11ColliderOBB* pOBB = pBodyObj->AddComponent<CDx11ColliderOBB>(USER_BODY);
	float fAxisLen[3];
	fAxisLen[0] = 0.7f;
	fAxisLen[1] = 0.8f;
	fAxisLen[2] = 0.7f;
	pOBB->SetOBBInfo(fAxisLen, pBodyObjTr);
	SAFE_RELEASE(pOBB);

	CDx11Renderer* pBodyObjRenderer = pBodyObj->AddComponent<CDx11Renderer>("pUserBodyObjRenderer");
	pBodyObjRenderer->SetMesh("CubeMesh");
	pBodyObjRenderer->SetShader("DefaultShader");
	pBodyObjRenderer->SetMaterialDiffuseTex("Default");

	// �ٵ� �޽��� ���δ�.
	CDx11GameObject* pBodyMesh = CDx11GameObject::Create("pUserBodyMesh");
	pBodyObj->AddChild(pBodyMesh);

	CDx11Renderer* pBodyMeshRenderer = pBodyMesh->AddComponent<CDx11Renderer>("pUserBodyMeshRenderer");
	pBodyMeshRenderer->SetMesh("rBody");
	pBodyMeshRenderer->SetShader("TexShader");

	// �ٵ�޽��� �ʱ�ũ�⸦ �����Ѵ�.
	CDx11Transform* pBodyMeshTr = pBodyMesh->GetTransform();
	pBodyMeshTr->Scaling(SCALE, SCALE, SCALE);
	pBodyMeshTr->SetWorldRotY(DX11_PI);

	SAFE_RELEASE(pBodyObjTr);
	SAFE_RELEASE(pBodyMeshTr);
	SAFE_RELEASE(pBodyMeshRenderer);
	SAFE_RELEASE(pBodyMesh);
	SAFE_RELEASE(pBodyObjRenderer);

	return true;
}

bool CUser::CreateLTWheel()
{
	// �»�� �� ������Ʈ�� �߰��Ѵ�.
	pLT = CDx11GameObject::Create("pUserLT");
	pBodyObj->AddChild(pLT);

	CDx11Transform* pLTTr = pLT->GetTransform();
	pLTTr->Scaling(0, 0, 0);

	CDx11Renderer* pLTRenderer = pLT->AddComponent<CDx11Renderer>("pUserLTRenderer");
	pLTRenderer->SetMesh("CubeMesh");
	pLTRenderer->SetShader("DefaultShader");
	pLTRenderer->SetMaterialDiffuseTex("Default");

	// ������ �޽��� ���δ�.
	CDx11GameObject* pLTMesh = CDx11GameObject::Create("pUserLTMesh");
	pLT->AddChild(pLTMesh);

	CDx11Renderer* pLTMeshRenderer = pLTMesh->AddComponent<CDx11Renderer>("pUserLTMeshRenderer");
	pLTMeshRenderer->SetMesh("Wheel");
	pLTMeshRenderer->SetShader("TexShader");

	// �����޽��� �ʱ�ũ�⸦ �����Ѵ�.
	CDx11Transform* pLTMeshTr = pLTMesh->GetTransform();
	pLTMeshTr->Scaling(SCALE, SCALE, SCALE);
	pLTMeshTr->SetWorldRotY(DX11_PI);
	pLTMeshTr->MoveWorld(Vec3(-0.52, -0.24, 0.65));

	SAFE_RELEASE(pLTTr);
	SAFE_RELEASE(pLTRenderer);
	SAFE_RELEASE(pLTMesh);
	SAFE_RELEASE(pLTMeshRenderer);
	SAFE_RELEASE(pLTMeshTr)

		return true;
}

bool CUser::CreateRTWheel()
{
	// ���� �� ������Ʈ�� �߰��Ѵ�.
	pRT = CDx11GameObject::Create("pUserRT");
	pBodyObj->AddChild(pRT);

	CDx11Transform* pRTTr = pRT->GetTransform();
	pRTTr->Scaling(0, 0, 0);

	CDx11Renderer* pRTRenderer = pRT->AddComponent<CDx11Renderer>("pUserRTRenderer");
	pRTRenderer->SetMesh("CubeMesh");
	pRTRenderer->SetShader("DefaultShader");
	pRTRenderer->SetMaterialDiffuseTex("Default");

	// ������ �޽��� ���δ�.
	CDx11GameObject* pRTMesh = CDx11GameObject::Create("pUserRTMesh");
	pRT->AddChild(pRTMesh);

	CDx11Renderer* pRTMeshRenderer = pRTMesh->AddComponent<CDx11Renderer>("pUserRTMeshRenderer");
	pRTMeshRenderer->SetMesh("Wheel");
	pRTMeshRenderer->SetShader("TexShader");

	// �����޽��� �ʱ�ũ�⸦ �����Ѵ�.
	CDx11Transform* pRTMeshTr = pRTMesh->GetTransform();
	pRTMeshTr->Scaling(SCALE, SCALE, SCALE);
	pRTMeshTr->SetWorldRotY(DX11_PI);
	pRTMeshTr->MoveWorld(Vec3(0.52, -0.24, 0.65));

	SAFE_RELEASE(pRTTr);
	SAFE_RELEASE(pRTRenderer);
	SAFE_RELEASE(pRTMesh);
	SAFE_RELEASE(pRTMeshRenderer);
	SAFE_RELEASE(pRTMeshTr)

	return true;
}

bool CUser::CreateLBWheel()
{
	// ���ϴ� �� ������Ʈ�� �߰��Ѵ�.
	pLB = CDx11GameObject::Create("pUserLB");
	pBodyObj->AddChild(pLB);

	CDx11Transform* pLBTr = pLB->GetTransform();
	pLBTr->Scaling(0, 0, 0);

	CDx11Renderer* pLBRenderer = pLB->AddComponent<CDx11Renderer>("pUserLBRenderer");
	pLBRenderer->SetMesh("CubeMesh");
	pLBRenderer->SetShader("DefaultShader");
	pLBRenderer->SetMaterialDiffuseTex("Default");

	// ������ �޽��� ���δ�.
	CDx11GameObject* pLBMesh = CDx11GameObject::Create("pUserLBMesh");
	pLB->AddChild(pLBMesh);

	CDx11Renderer* pLBMeshRenderer = pLBMesh->AddComponent<CDx11Renderer>("pUserLBMeshRenderer");
	pLBMeshRenderer->SetMesh("Wheel");
	pLBMeshRenderer->SetShader("TexShader");

	// �����޽��� �ʱ�ũ�⸦ �����Ѵ�.
	CDx11Transform* pLBMeshTr = pLBMesh->GetTransform();
	pLBMeshTr->Scaling(SCALE, SCALE, SCALE);
	pLBMeshTr->SetWorldRotY(DX11_PI);
	pLBMeshTr->MoveWorld(Vec3(-0.52, -0.24, -0.65));

	SAFE_RELEASE(pLBTr);
	SAFE_RELEASE(pLBRenderer);
	SAFE_RELEASE(pLBMesh);
	SAFE_RELEASE(pLBMeshRenderer);
	SAFE_RELEASE(pLBMeshTr);

	return true;
}

bool CUser::CreateRBWheel()
{
	// ���ϴ� �� ������Ʈ�� �߰��Ѵ�.
	pRB = CDx11GameObject::Create("pUserRB");
	pBodyObj->AddChild(pRB);

	CDx11Transform* pRBTr = pRB->GetTransform();
	pRBTr->Scaling(0, 0, 0);

	CDx11Renderer* pRBRenderer = pRB->AddComponent<CDx11Renderer>("pUserRBRenderer");
	pRBRenderer->SetMesh("CubeMesh");
	pRBRenderer->SetShader("DefaultShader");
	pRBRenderer->SetMaterialDiffuseTex("Default");

	// ������ �޽��� ���δ�.
	CDx11GameObject* pRBMesh = CDx11GameObject::Create("pUserRBMesh");
	pRB->AddChild(pRBMesh);

	CDx11Renderer* pRBMeshRenderer = pRBMesh->AddComponent<CDx11Renderer>("pUserRBMeshRenderer");
	pRBMeshRenderer->SetMesh("Wheel");
	pRBMeshRenderer->SetShader("TexShader");

	// �����޽��� �ʱ�ũ�⸦ �����Ѵ�.
	CDx11Transform* pRBMeshTr = pRBMesh->GetTransform();
	pRBMeshTr->Scaling(SCALE, SCALE, SCALE);
	pRBMeshTr->SetWorldRotY(DX11_PI);
	pRBMeshTr->MoveWorld(Vec3(0.52, -0.24, -0.65));

	SAFE_RELEASE(pRBTr);
	SAFE_RELEASE(pRBRenderer);
	SAFE_RELEASE(pRBMesh);
	SAFE_RELEASE(pRBMeshRenderer);
	SAFE_RELEASE(pRBMeshTr)

		return true;
}

bool CUser::CreateHead()
{
	// ��� ������Ʈ�� �߰��Ѵ�.
	pHeadObj = CDx11GameObject::Create("pUserHeadObj");
	pBodyObj->AddChild(pHeadObj);

	CDx11Transform* pHeadObjTr = pHeadObj->GetTransform();
	pHeadObjTr->Scaling(5, 0.1, 0.1);

//	CDx11Renderer* pHeadObjRenderer = pHeadObj->AddComponent<CDx11Renderer>("pUserHeadObjRenderer");
//	pHeadObjRenderer->SetMesh("CubeMesh");
//	pHeadObjRenderer->SetShader("DefaultShader");
//	pHeadObjRenderer->SetMaterialDiffuseTex("Default");

	// ���� ����Ʈ�� �߰��Ѵ�.
	CDx11GameObject* pShootingPoint = CDx11GameObject::Create("pUserShootingPoint");
	pHeadObj->AddChild(pShootingPoint);

//	CDx11Renderer* pShootingPointRenderer = pShootingPoint->AddComponent<CDx11Renderer>("pUserShootingPointRenderer");
//	pShootingPointRenderer->SetMesh("CubeMesh");
//	pShootingPointRenderer->SetShader("DefaultShader");
//	pShootingPointRenderer->SetMaterialDiffuseTex("Default");

	CDx11Transform* pShootingPointTr = pShootingPoint->GetTransform();
	pShootingPointTr->Scaling(0.0, 0.0, 0.0);

	Vec3 AxisZ = pHeadObjTr->GetWorldAxis(AXIS_Z);
	Vec3 AxisY = pHeadObjTr->GetWorldAxis(AXIS_Y);

	// ���� ����Ʈ�� �ʱ� ��ġ ����
	AxisZ *= 1;
	AxisZ += (AxisY*0.6);
	pShootingPointTr->MoveWorld(AxisZ);

	// ��忡 �޽��� ���δ�.
	CDx11GameObject* pHeadMesh = CDx11GameObject::Create("pUserHeadMesh");
	pHeadObj->AddChild(pHeadMesh);

	CDx11Renderer* pHeadMeshRenderer = pHeadMesh->AddComponent<CDx11Renderer>("pUserHeadMeshRenderer");
	pHeadMeshRenderer->SetMesh("rHead");
	pHeadMeshRenderer->SetShader("TexShader");

	// �����޽��� �ʱ�ũ�⸦ �����Ѵ�.
	CDx11Transform* pHeadMeshTr = pHeadMesh->GetTransform();
	pHeadMeshTr->Scaling(SCALE, SCALE, SCALE);
	pHeadMeshTr->SetWorldRotY(DX11_PI);

	SAFE_RELEASE(pShootingPointTr);
//	SAFE_RELEASE(pShootingPointRenderer);
	SAFE_RELEASE(pShootingPoint);
	SAFE_RELEASE(pHeadMeshTr);
	SAFE_RELEASE(pHeadMeshRenderer);
	SAFE_RELEASE(pHeadMesh);
//	SAFE_RELEASE(pHeadObjRenderer);
	SAFE_RELEASE(pHeadObjTr)

		return true;
}

void CUser::RotateWheel(float fSpeed)
{
	// �ٵ��� X���� �������� ������.
	CDx11GameObject* pBodyObj = pLT->GetParent();
	CDx11Transform* pTr = pBodyObj->GetTransform();
	Vec3 AxisX = pTr->GetWorldAxis(AXIS_X);
	SAFE_RELEASE(pTr);
	SAFE_RELEASE(pBodyObj);

	CDx11GameObject* pObj = pLT->FindChildFromTag("pUserLTMesh");
	pTr = pObj->GetTransform();
	pTr->RotateWorld(m_fSpeed, fSpeed, AxisX);
	SAFE_RELEASE(pTr);

	pObj = pRT->FindChildFromTag("pUserRTMesh");
	pTr = pObj->GetTransform();
	pTr->RotateWorld(m_fSpeed, fSpeed, AxisX);
	SAFE_RELEASE(pTr);

	pObj = pLB->FindChildFromTag("pUserLBMesh");
	pTr = pObj->GetTransform();
	pTr->RotateWorld(m_fSpeed, fSpeed, AxisX);
	SAFE_RELEASE(pTr);

	pObj = pRB->FindChildFromTag("pUserRBMesh");
	pTr = pObj->GetTransform();
	pTr->RotateWorld(m_fSpeed, fSpeed, AxisX);
	SAFE_RELEASE(pTr);
}

void CUser::SetHeight(float fTime)
{
	// �ٵ��� Ʈ���� ���� �����´�.
	CDx11Transform* pBodyObjTr = pBodyObj->GetTransform();

	// �׷��� ������Ʈ�� ã�´�.
	CDx11GameObject* pTerrainObj = m_pScene->FindGameObject("TerrainObj");
	CDx11Terrain* pTerrain = (CDx11Terrain*)pTerrainObj->FindComponentFromTag("TerrainCP");

	// ���� ��ġ������ ����� �븻�� ��ġ������ �����´�.
	TRIANGLE2RAY LTPN = pTerrain->GetHeight(pLT);
	TRIANGLE2RAY RTPN = pTerrain->GetHeight(pRT);
	TRIANGLE2RAY LBPN = pTerrain->GetHeight(pLB);
	TRIANGLE2RAY RBPN = pTerrain->GetHeight(pRB);

	// ������ ����� ���Ѵ�.
	// ���� ���� �׸��� ������ HEIGHT_ALPHA�� ���Ѵ�.
	float vAvePosY = LTPN.Pos.y + RTPN.Pos.y + LBPN.Pos.y + RBPN.Pos.y;
	vAvePosY /= 4;	vAvePosY += HEIGHT_ALPHA;

	// ��ũ ������Ʈ�� Ʈ�������� �����´�.
	// x, z�� �����ϸ鼭 y���� ��ü�Ѵ�.
	CDx11Transform* pTankObjTr = m_pGameObject->GetTransform();
	Vec3 vMainPos = pTankObjTr->GetWorldPos();
	vMainPos.y = vAvePosY;

	// �ٵ� ������Ʈ�� ������ ��ճ��̷� �ű��.
	pBodyObjTr->SetWorldPos(fTime * 10, vMainPos);

	SAFE_RELEASE(pTerrainObj);
	SAFE_RELEASE(pTerrain);
	SAFE_RELEASE(pTankObjTr);

	// �ٵ� ������Ʈ�� y���� �޾ƿ´�.
	Vec3 vMain_AxisY = pBodyObjTr->GetWorldAxis(AXIS_Y);

	// ���������� �븻�� ��հ� Y���� �̿��� ȸ����Ų��.
	// ȸ������ ȸ������ ���Ѵ�.
	Vec3 vAveNormal = LTPN.Normal + RTPN.Normal + LBPN.Normal + RBPN.Normal;

	vAveNormal /= 4;
	vAveNormal = vAveNormal.Normalize();

	float fAngle = vMain_AxisY.Dot(vAveNormal);
	Vec3 vRotation_Axis = vMain_AxisY.Cross(vAveNormal);

	// ȸ������ ȸ������ �̿��� ȸ����Ų��.
	pBodyObjTr->RotateWorld(fAngle, fTime * 10, vRotation_Axis);

	//////////////////////////////////////////////////////////////////////////////////////////////////////
	// �ٵ� ������Ʈ�� xz ���� ������ z���� �����´�.
	Vec3 vAxisZ = pBodyObjTr->GetWorldAxis(AXIS_Z);
	vAxisZ.y = 0;
	vAxisZ = vAxisZ.Normalize();

	// �θ������Ʈ�� Z���� �����´�.
	Vec3 ParentObj = m_pTransform->GetWorldAxis(AXIS_Z);
	ParentObj = ParentObj.Normalize();

	// �� ���� �̿��ؼ� ȸ������ ȸ������ ���Ѵ�.
	//fAngle = ParentObj.Dot(vAxisZ);
	//Vec3 vAxis = ParentObj.Cross(vAxisZ);

	//// �θ��� ���� ȸ����Ų��.
	//pBodyObjTr->DeleteParentFlag(TPF_ROT);
	//m_pTransform->RotateWorld(fAngle, 1, vAxis);
	//pBodyObjTr->AddParentFlag(TPF_ROT);

	//// ���� ������ �� ������� ������.
	//vMain_AxisY = pBodyObjTr->GetWorldAxis(AXIS_Y);

	//if (vMain_AxisY.y < 0) {
	//	m_pTransform->SetWorldRotY(DX11_PI);
	//}

	SAFE_RELEASE(pBodyObjTr);
}

void CUser::CreateBar()
{
	m_pFrameBar = CDx11GameObject::Create("FrameBar");
	pBodyObj->AddChild(m_pFrameBar);

	CDx11Renderer*	pRenderer = m_pFrameBar->AddComponent<CDx11Renderer>("FrameBarRenderer");
	pRenderer->CreateMaterial("FrameBar", L"Health/HBarFrame.png");
	pRenderer->SetMesh(EFFECT_LT_MESH);
	pRenderer->SetShader(UISHADER);
	pRenderer->SetRenderState(DX11_RS_ALPHABLEND, RSF_BLEND);
	//pRenderer->SetRenderState(DX11_RS_DEPTH_DISABLE, RSF_DEPTHSTENCIL);
	SAFE_RELEASE(pRenderer);

	// �ʱ� ũ�⸦ �����Ѵ�.
	CDx11Transform*	pTransform = m_pFrameBar->GetTransform();
	pTransform->Scaling(2.f, 0.4f, 1.f);
	pTransform->MoveWorld(Vec3(0, 0, 0));
	SAFE_RELEASE(pTransform);

	// ������ ������Ʈ�� �߰��Ѵ�.
	CDx11BillBoard*	pBillBoard = m_pFrameBar->AddComponent<CDx11BillBoard>("FrameBarBillBoard");
	SAFE_RELEASE(pBillBoard);

	// �÷��̾� HP Bar ����
	m_pHPBar = CDx11GameObject::Create("HPBar");
	pBodyObj->AddChild(m_pHPBar);

	pRenderer = m_pHPBar->AddComponent<CDx11Renderer>("HPBarRenderer");
	pRenderer->CreateMaterial("HPBar3", L"Health/HBarRed.png");
	pRenderer->SetMesh(EFFECT_LT_MESH);
	pRenderer->SetShader(UISHADER);
	pRenderer->SetRenderState(DX11_RS_ALPHABLEND, RSF_BLEND);
	pRenderer->SetRenderState(DX11_RS_DEPTH_DISABLE, RSF_DEPTHSTENCIL);
	SAFE_RELEASE(pRenderer);

	// �ʱ� ũ�⸦ �����Ѵ�.
	pTransform = m_pHPBar->GetTransform();
	pTransform->Scaling(2, 0.4, 1.f);
	pTransform->MoveWorld(Vec3(0, 0, 0));
	SAFE_RELEASE(pTransform);

	// �����带 �߰��Ѵ�.
	pBillBoard = m_pHPBar->AddComponent<CDx11BillBoard>("HPBarBillBoard");
	SAFE_RELEASE(pBillBoard);
}

void CUser::SetBarPos()
{
	// HP�� ��ġ�� �����Ѵ�.
	CDx11Transform* pTr = pBodyObj->GetTransform();
	Vec3 vMainObjPos = pTr->GetWorldPos();
	SAFE_RELEASE(pTr);

	vMainObjPos.x = vMainObjPos.x - 1;
	vMainObjPos.y = vMainObjPos.y + 1.7;

	// ������
	pTr = m_pFrameBar->GetTransform();

	pTr->SetWorldPos(vMainObjPos);

	SAFE_RELEASE(pTr);

	pTr = m_pHPBar->GetTransform();

	pTr->SetWorldPos(vMainObjPos);

	pTr->Scaling(2 * m_fHP, 0.4f, 1.f);

	SAFE_RELEASE(pTr);
}


bool CUser::Init()
{
	if (!m_pGameObject)
		return false;

	// �ٵ� �ʱ�ȭ�Ѵ�.
	CreateBody();

	// ���� �ʱ�ȭ�Ѵ�.
	CreateLTWheel();
	CreateRTWheel();
	CreateLBWheel();
	CreateRBWheel();

	// ��带 �ʱ�ȭ�Ѵ�.
	CreateHead();

	// �ٸ� �����Ѵ�.
	CreateBar();

	return true;
}

void CUser::Input(float fTime)
{
}

int CUser::Update(float fTime)
{
	// ������ Ż �� �ֵ��� �Ѵ�.
	SetHeight(fTime);

	// ���� ��ġ�� �����Ѵ�.
	SetBarPos();

	return 0;
}

int CUser::LateUpdate(float fTime)
{
	return 0;
}

void CUser::Render(float fTime)
{
}

CUser * CUser::Clone()
{
	return new CUser(*this);
}

void CUser::OnCollisionEnter(CDx11Collider * pMy, CDx11Collider * pYour, float fTime)
{
	if (pMy->GetTag() == USER_BODY)
	{
		if (pYour->GetTag() == MONSTER_ATTACK_BOX) {
			m_fHP -= 0.1;
		}

		if (pYour->GetTag() == BULLET_BODY) {
			m_fHP -= 0.1;
		}
	}
}

void CUser::OnCollision(CDx11Collider * pMy, CDx11Collider * pYour, float fTime)
{
}

void CUser::OnCollisionExit(CDx11Collider * pMy, CDx11Collider * pYour, float fTime)
{
}