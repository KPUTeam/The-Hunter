#include "TankInitSC.h"
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
#include "Core/Dx11TimerManager.h"
#include "Core/Dx11Timer.h"
#include "Core/Dx11Debug.h"
#include "Core/Dx11Device.h"

#include "TankCameraSC.h"
#include "TackCollisionSC.h"
#include "../Bullet/BulletSC.h"

#include "../../Network/NetManager.h"
#include "../../Network/ZEROStream.h"

extern bool startFlag;

CTankInitSC::CTankInitSC():
	pBodyObj(NULL),
	pLT(NULL),
	pRT(NULL),
	pLB(NULL),
	pRB(NULL),
	pHeadObj(NULL),
	pTankHP(NULL)
{
}

CTankInitSC::CTankInitSC(const CTankInitSC & script)
{
}

CTankInitSC::~CTankInitSC()
{
	SAFE_RELEASE(pBodyObj);
	SAFE_RELEASE(pLT);
	SAFE_RELEASE(pRT);
	SAFE_RELEASE(pLB);
	SAFE_RELEASE(pRB);
	SAFE_RELEASE(pHeadObj);
	SAFE_RELEASE(pTankHP);
}

bool CTankInitSC::Init()
{
	if (!m_pGameObject)
		return false;

	// 바디를 초기화한다.
	CreateBody();
	
	// 휠을 초기화한다.
	CreateLTWheel();
	CreateRTWheel();
	CreateLBWheel();
	CreateRBWheel();

	// 헤드를 초기화한다.
	CreateHead();

	// 스크립트를 추가한다.

	// 1. 카메라 스크립트
	CTankCameraSC* pTankCameraSC = m_pGameObject->AddComponent<CTankCameraSC>("CTankCameraSC");
	SAFE_RELEASE(pTankCameraSC);

	// 키를 설정한다.
	DX11_GET_SINGLE(CDx11Input)->CreateKey('W', "MoveFront");
	DX11_GET_SINGLE(CDx11Input)->CreateKey('S', "MoveBack");
	DX11_GET_SINGLE(CDx11Input)->CreateKey('A', "TurnLeftTrigger");
	DX11_GET_SINGLE(CDx11Input)->CreateKey('D', "RightTurnTrigger");
	DX11_GET_SINGLE(CDx11Input)->CreateKey(VK_LBUTTON, "Shoot");

	CreateBar();
	CreateUI();

	return true;
}

int CTankInitSC::Update(float fTime)
{
	// 키입력을 받는다.
	Input(fTime);

	// 지형을 탈 수 있도록 한다.
	SetHeight(fTime);
	SetBarPos();
	SetUI();

	return 0;
}

int CTankInitSC::LateUpdate(float fTime)
{
	// 가로세로 제한
	Vec3 MyPos = m_pTransform->GetWorldPos();

	if (MyPos.x < 2)
		MyPos.x = 2;

	if (MyPos.z < 2)
		MyPos.z = 2;

	if (MyPos.x >195)
		MyPos.x = 195;

	if (MyPos.z >195)
		MyPos.z = 195;

	m_pTransform->SetWorldPos(MyPos);

	CDx11Timer* pTimer = DX11_GET_SINGLE(CDx11TimerManager)->FindTimer("MainTimer");
	pTimer->Update();
	float fFrameTime = pTimer->GetDeltaTime();
	m_fGlobalTime += fFrameTime;

   if (startFlag) // 게임이 시작되고나서
   {
		if (m_fGlobalTime >= 0.01)   // 0.01초마다
		{
			if (TransformUpdate)
			{
				GET_SINGLE(CNetManager)->myMove(m_pTransform->GetWorldPos());

				CDx11Transform* pTr = pBodyObj->GetTransform();
				GET_SINGLE(CNetManager)->myRotate(pTr->GetWorldAxis(AXIS_Z));
				SAFE_RELEASE(pTr);

				m_fGlobalTime = 0;
			}
		}
   }

	return 0;
}

void CTankInitSC::Render(float fTime)
{
}

CTankInitSC * CTankInitSC::Clone()
{
	return new CTankInitSC(*this);
}

bool CTankInitSC::CreateBody()
{
	// 바디를 출력한다.
	pBodyObj = CDx11GameObject::Create("pBodyObj");
	m_pGameObject->AddChild(pBodyObj);

	CDx11Transform* pBodyObjTr = pBodyObj->GetTransform();
	pBodyObjTr->Scaling(0,0,0);

	// 콜리더스피어
	CDx11ColliderSphere* pSphere = pBodyObj->AddComponent<CDx11ColliderSphere>(PLAYER_BODY);
	pSphere->SetSphereInfo(1, pBodyObjTr->GetWorldPos());
	SAFE_RELEASE(pSphere);

	// 충돌체를 설정한다.
	CDx11ColliderOBB* pOBB = pBodyObj->AddComponent<CDx11ColliderOBB>(PLAYER_BODY);
	float fAxisLen[3];
	fAxisLen[0] = 0.7f;
	fAxisLen[1] = 0.8f;
	fAxisLen[2] = 0.7f;
	pOBB->SetOBBInfo(fAxisLen, pBodyObjTr);
	SAFE_RELEASE(pOBB);

	// 충돌 확인 컴포넌트를 추가한다.
	CTankCollisionSC* pCollisionSC = pBodyObj->AddComponent<CTankCollisionSC>("pCollisionSC");
	SAFE_RELEASE(pCollisionSC);

	CDx11Renderer* pBodyObjRenderer = pBodyObj->AddComponent<CDx11Renderer>("pBodyObjRenderer");
	pBodyObjRenderer->SetMesh("CubeMesh");
	pBodyObjRenderer->SetShader("DefaultShader");
	pBodyObjRenderer->SetMaterialDiffuseTex("Default");

	// 바디에 메쉬를 붙인다.
	CDx11GameObject* pBodyMesh = CDx11GameObject::Create("pBodyMesh");
	pBodyObj->AddChild(pBodyMesh);

	CDx11Renderer* pBodyMeshRenderer = pBodyMesh->AddComponent<CDx11Renderer>("pBodyMeshRenderer");
	pBodyMeshRenderer->SetMesh("gBody");
	pBodyMeshRenderer->SetShader("TexShader");

	// 바디메쉬의 초기크기를 설정한다.
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

bool CTankInitSC::CreateLTWheel()
{
	// 좌상단 휠 오브젝트를 추가한다.
	pLT = CDx11GameObject::Create("pLT");
	pBodyObj->AddChild(pLT);

	CDx11Transform* pLTTr = pLT->GetTransform();
	pLTTr->Scaling(0, 0, 0);

	CDx11Renderer* pLTRenderer = pLT->AddComponent<CDx11Renderer>("pLTRenderer");
	pLTRenderer->SetMesh("CubeMesh");
	pLTRenderer->SetShader("DefaultShader");
	pLTRenderer->SetMaterialDiffuseTex("Default");

	// 바퀴에 메쉬를 붙인다.
	CDx11GameObject* pLTMesh = CDx11GameObject::Create("pLTMesh");
	pLT->AddChild(pLTMesh);

	CDx11Renderer* pLTMeshRenderer = pLTMesh->AddComponent<CDx11Renderer>("pLTMeshRenderer");
	pLTMeshRenderer->SetMesh("Wheel");
	pLTMeshRenderer->SetShader("TexShader");

	// 바퀴메쉬의 초기크기를 설정한다.
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

bool CTankInitSC::CreateRTWheel()
{
	// 우상단 휠 오브젝트를 추가한다.
	pRT = CDx11GameObject::Create("pRT");
	pBodyObj->AddChild(pRT);

	CDx11Transform* pRTTr = pRT->GetTransform();
	pRTTr->Scaling(0, 0, 0);

	CDx11Renderer* pRTRenderer = pRT->AddComponent<CDx11Renderer>("pRTRenderer");
	pRTRenderer->SetMesh("CubeMesh");
	pRTRenderer->SetShader("DefaultShader");
	pRTRenderer->SetMaterialDiffuseTex("Default");

	// 바퀴에 메쉬를 붙인다.
	CDx11GameObject* pRTMesh = CDx11GameObject::Create("pRTMesh");
	pRT->AddChild(pRTMesh);

	CDx11Renderer* pRTMeshRenderer = pRTMesh->AddComponent<CDx11Renderer>("pRTMeshRenderer");
	pRTMeshRenderer->SetMesh("Wheel");
	pRTMeshRenderer->SetShader("TexShader");

	// 바퀴메쉬의 초기크기를 설정한다.
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

bool CTankInitSC::CreateLBWheel()
{
	// 좌하단 휠 오브젝트를 추가한다.
	pLB = CDx11GameObject::Create("pLB");
	pBodyObj->AddChild(pLB);

	CDx11Transform* pLBTr = pLB->GetTransform();
	pLBTr->Scaling(0, 0, 0);

	CDx11Renderer* pLBRenderer = pLB->AddComponent<CDx11Renderer>("pLBRenderer");
	pLBRenderer->SetMesh("CubeMesh");
	pLBRenderer->SetShader("DefaultShader");
	pLBRenderer->SetMaterialDiffuseTex("Default");

	// 바퀴에 메쉬를 붙인다.
	CDx11GameObject* pLBMesh = CDx11GameObject::Create("pLBMesh");
	pLB->AddChild(pLBMesh);

	CDx11Renderer* pLBMeshRenderer = pLBMesh->AddComponent<CDx11Renderer>("pLBMeshRenderer");
	pLBMeshRenderer->SetMesh("Wheel");
	pLBMeshRenderer->SetShader("TexShader");

	// 바퀴메쉬의 초기크기를 설정한다.
	CDx11Transform* pLBMeshTr = pLBMesh->GetTransform();
	pLBMeshTr->Scaling(SCALE, SCALE, SCALE);
	pLBMeshTr->SetWorldRotY(DX11_PI);
	pLBMeshTr->MoveWorld(Vec3(-0.52, -0.24, -0.65));

	SAFE_RELEASE(pLBTr);
	SAFE_RELEASE(pLBRenderer);
	SAFE_RELEASE(pLBMesh);
	SAFE_RELEASE(pLBMeshRenderer);
	SAFE_RELEASE(pLBMeshTr)

	return true;
}

bool CTankInitSC::CreateRBWheel()
{
	// 우하단 휠 오브젝트를 추가한다.
	pRB = CDx11GameObject::Create("pRB");
	pBodyObj->AddChild(pRB);

	CDx11Transform* pRBTr = pRB->GetTransform();
	pRBTr->Scaling(0, 0, 0);

	CDx11Renderer* pRBRenderer = pRB->AddComponent<CDx11Renderer>("pRBRenderer");
	pRBRenderer->SetMesh("CubeMesh");
	pRBRenderer->SetShader("DefaultShader");
	pRBRenderer->SetMaterialDiffuseTex("Default");

	// 바퀴에 메쉬를 붙인다.
	CDx11GameObject* pRBMesh = CDx11GameObject::Create("pRBMesh");
	pRB->AddChild(pRBMesh);

	CDx11Renderer* pRBMeshRenderer = pRBMesh->AddComponent<CDx11Renderer>("pRBMeshRenderer");
	pRBMeshRenderer->SetMesh("Wheel");
	pRBMeshRenderer->SetShader("TexShader");

	// 바퀴메쉬의 초기크기를 설정한다.
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

bool CTankInitSC::CreateHead()
{
	// 헤드 오브젝트를 추가한다.
	pHeadObj = CDx11GameObject::Create("pHeadObj");
	pBodyObj->AddChild(pHeadObj);

	CDx11Transform* pHeadObjTr = pHeadObj->GetTransform();
	pHeadObjTr->Scaling(0, 0, 0);

	CDx11Renderer* pHeadObjRenderer = pHeadObj->AddComponent<CDx11Renderer>("pHeadObjRenderer");
	pHeadObjRenderer->SetMesh("CubeMesh");
	pHeadObjRenderer->SetShader("DefaultShader");
	pHeadObjRenderer->SetMaterialDiffuseTex("Default");

	// 슈팅 포인트를 추가한다.
	CDx11GameObject* pShootingPoint = CDx11GameObject::Create("pShootingPoint");
	pHeadObj->AddChild(pShootingPoint);

	CDx11Renderer* pShootingPointRenderer = pShootingPoint->AddComponent<CDx11Renderer>("pShootingPointRenderer");
	pShootingPointRenderer->SetMesh("CubeMesh");
	pShootingPointRenderer->SetShader("DefaultShader");
	pShootingPointRenderer->SetMaterialDiffuseTex("Default");

	CDx11Transform* pShootingPointTr = pShootingPoint->GetTransform();
	pShootingPointTr->Scaling(0, 0, 0);

	Vec3 AxisZ = pHeadObjTr->GetWorldAxis(AXIS_Z);
	Vec3 AxisY = pHeadObjTr->GetWorldAxis(AXIS_Y);
	
	// 슈팅 포인트의 초기 위치 지정
	AxisZ *= 1.4;
	AxisZ += (AxisY*0.6);
	pShootingPointTr->MoveWorld(AxisZ);

	// 헤드에 메쉬를 붙인다.
	CDx11GameObject* pHeadMesh = CDx11GameObject::Create("pHeadMesh");
	pHeadObj->AddChild(pHeadMesh);

	CDx11Renderer* pHeadMeshRenderer = pHeadMesh->AddComponent<CDx11Renderer>("pHeadMeshRenderer");
	pHeadMeshRenderer->SetMesh("gHead");
	pHeadMeshRenderer->SetShader("TexShader");

	// 바퀴메쉬의 초기크기를 설정한다.
	CDx11Transform* pHeadMeshTr = pHeadMesh->GetTransform();
	pHeadMeshTr->Scaling(SCALE, SCALE, SCALE);
	pHeadMeshTr->SetWorldRotY(DX11_PI);

	SAFE_RELEASE(pShootingPointTr);
	SAFE_RELEASE(pShootingPointRenderer);
	SAFE_RELEASE(pShootingPoint);
	SAFE_RELEASE(pHeadMeshTr);
	SAFE_RELEASE(pHeadMeshRenderer);
	SAFE_RELEASE(pHeadMesh);
	SAFE_RELEASE(pHeadObjRenderer);
	SAFE_RELEASE(pHeadObjTr)

	return true;
}

void CTankInitSC::CreateBar()
{
}

void CTankInitSC::SetBarPos()
{
}

void CTankInitSC::CreateUI()
{

	// 레이어를 찾는다.
	CDx11Layer*	pLayer = m_pScene->FindLayer("DefaultLayer");

	RESOLUTION_TYPE	eRSType = DX11_GET_SINGLE(CDx11Device)->GetResolutionType();
	float sizeX = (float)g_tRS[eRSType].iWidth * 0.3;
	float posX = (float)g_tRS[eRSType].iWidth * 0.15;

	// HP Frame
	CDx11GameObject*	pTankHPFrame = CDx11GameObject::Create("pTankHPFrame", OCT_NORMAL);

	// 렌더러를 설정한다.
	CDx11Renderer*	pRenderer = pTankHPFrame->AddComponent<CDx11Renderer>("pTankHPFrameRenderer");
	pRenderer->SetCameraTag("UICamera");
	pRenderer->SetMesh("UIMesh");
	pRenderer->SetShader("UIShader");
	// pRenderer->SetRenderState(DX11_RS_DEPTH_DISABLE, RSF_DEPTHSTENCIL);
	pRenderer->SetRenderState(DX11_RS_ALPHABLEND, RSF_BLEND);
	pRenderer->CreateMaterial("pTankHPFrame", L"Health/HBarFrame.png");
	SAFE_RELEASE(pRenderer);

	// 크기를 조정한다.
	CDx11Transform*	pTr = pTankHPFrame->GetTransform();
	pTr->Scaling(sizeX, (float)g_tRS[eRSType].iHeight / 12, 1.f);
	pTr->SetWorldPos(posX, (float)g_tRS[eRSType].iHeight / 10, 1.f);
	SAFE_RELEASE(pTr);

	// HP
	pTankHP = CDx11GameObject::Create("pTankHP", OCT_NORMAL);

	// 렌더러를 설정한다.
	pRenderer = pTankHP->AddComponent<CDx11Renderer>("pTankHPRenderer");
	pRenderer->SetCameraTag("UICamera");
	pRenderer->SetMesh("UIMesh");
	pRenderer->SetShader("UIShader");
	// pRenderer->SetRenderState(DX11_RS_DEPTH_DISABLE, RSF_DEPTHSTENCIL);
	pRenderer->SetRenderState(DX11_RS_ALPHABLEND, RSF_BLEND);
	pRenderer->CreateMaterial("TankHPBar", L"Health/HBarBlue.png");
	SAFE_RELEASE(pRenderer);

	// 크기를 조정한다.
	pTr = pTankHP->GetTransform();
	pTr->Scaling(sizeX, (float)g_tRS[eRSType].iHeight / 12, 1.f);
	pTr->SetWorldPos(posX, (float)g_tRS[eRSType].iHeight / 10, 1.f);
	SAFE_RELEASE(pTr);

	// 탱크 모형
	CDx11GameObject*	pTankUI = CDx11GameObject::Create("pTankUI", OCT_NORMAL);

	// 렌더러를 설정한다.
	pRenderer = pTankUI->AddComponent<CDx11Renderer>("pTankUIRenderer");
	pRenderer->SetCameraTag("UICamera");
	pRenderer->SetMesh("UIMesh");
	pRenderer->SetShader("UIShader");
	// pRenderer->SetRenderState(DX11_RS_DEPTH_DISABLE, RSF_DEPTHSTENCIL);
	pRenderer->SetRenderState(DX11_RS_ALPHABLEND, RSF_BLEND);
	pRenderer->CreateMaterial("pTankUI", L"greenHead.png");
	SAFE_RELEASE(pRenderer);

	// 크기를 조정한다.
	pTr = pTankUI->GetTransform();
	pTr->Scaling((float)g_tRS[eRSType].iWidth / 8, (float)g_tRS[eRSType].iHeight / 6, 1.f);
	pTr->SetWorldPos((float)g_tRS[eRSType].iWidth / 20, (float)g_tRS[eRSType].iHeight / 20, 1.f);
	SAFE_RELEASE(pTr);

	// 레이어에 추가한다.
	pLayer->AddObject(pTankUI);
	pLayer->AddObject(pTankHP);
	pLayer->AddObject(pTankHPFrame);

	SAFE_RELEASE(pTankHPFrame);
	SAFE_RELEASE(pTankUI);
	SAFE_RELEASE(pLayer);
}

void CTankInitSC::SetUI()
{
	// HP의 위치를 적용한다.
	CDx11Transform* pTr = pTankHP->GetTransform();

	// 프레임
	CTankCollisionSC* pCollisionSC = (CTankCollisionSC*)pBodyObj->FindComponentFromTag("pCollisionSC");
	float hp = pCollisionSC->GetHP();
	SAFE_RELEASE(pCollisionSC);

	RESOLUTION_TYPE	eRSType = DX11_GET_SINGLE(CDx11Device)->GetResolutionType();
	float sizeX = (float)g_tRS[eRSType].iWidth * 0.3;

	pTr->Scaling(sizeX * hp, (float)g_tRS[eRSType].iHeight / 12, 1.f);

	SAFE_RELEASE(pTr);
}

void CTankInitSC::Input(float fTime)
{
	if (startFlag)
	{
		if (DX11_GET_SINGLE(CDx11Input)->KeyDown("TurnLeftTrigger"))
		{
			TransformUpdate = true;
			LeftTurnTrigger = true;
		}

		if (DX11_GET_SINGLE(CDx11Input)->KeyUp("TurnLeftTrigger"))
		{
			LeftTurnTrigger = false;
		}

		if (DX11_GET_SINGLE(CDx11Input)->KeyDown("RightTurnTrigger"))
		{
			TransformUpdate = true;
			RightTurnTrigger = true;
		}

		if (DX11_GET_SINGLE(CDx11Input)->KeyUp("RightTurnTrigger"))
		{
			RightTurnTrigger = false;
		}

		if (DX11_GET_SINGLE(CDx11Input)->KeyDown("MoveFront") || DX11_GET_SINGLE(CDx11Input)->KeyStay("MoveFront"))
		{
			TransformUpdate = true;

			if (!LeftTurnTrigger) {
				TransformUpdate = true;
				m_pTransform->RotateWorld(DX11_PI / 2.5, fTime, AXIS_Y);
			}

			if (!RightTurnTrigger) {
				TransformUpdate = true;
				m_pTransform->RotateWorld(-DX11_PI / 2.5, fTime, AXIS_Y);
			}

			m_vAcc = 1;

			m_vDir += m_vAcc * m_fSpeed;

			if (m_vDir > 7.f) m_vDir = 7.f;

			m_pTransform->MoveWorld(m_vDir, fTime, AXIS_Z);

			RotateWheel(m_vDir*WHEEEL_ROTATION_SPEED);
		}

		else if (DX11_GET_SINGLE(CDx11Input)->KeyDown("MoveBack") || DX11_GET_SINGLE(CDx11Input)->KeyStay("MoveBack"))
		{
			TransformUpdate = true;

			if (!LeftTurnTrigger) {
				TransformUpdate = true;
				m_pTransform->RotateWorld(DX11_PI / 3, fTime, AXIS_Y);
			}

			if (!RightTurnTrigger) {
				TransformUpdate = true;
				m_pTransform->RotateWorld(-DX11_PI / 3, fTime, AXIS_Y);
			}

			m_vAcc = -1;

			m_vDir += m_vAcc * m_fSpeed;

			if (m_vDir < -5.f) m_vDir = -5.f;

			m_pTransform->MoveWorld(fTime, m_vDir, AXIS_Z);

			RotateWheel(m_vDir*WHEEEL_ROTATION_SPEED);
		}

		else
		{
			TransformUpdate = true;
			m_vDir -= m_vAcc * m_fSpeed;

			if (m_vDir * m_vAcc > 0) {
				m_pTransform->MoveWorld(fTime, m_vDir, AXIS_Z);
				RotateWheel(m_vDir*WHEEEL_ROTATION_SPEED);
			}

			else {
				TransformUpdate = false;
				m_vDir = 0;
				m_vAcc = 0;
			}
		}

		if (DX11_GET_SINGLE(CDx11Input)->KeyDown("Shoot"))
		{
			CDx11Layer*	pLayer = m_pScene->FindLayer("DefaultLayer");
			CDx11GameObject* pObj = pLayer->FindObjectFromTag("pAimObj");
			CDx11ColliderRay* pRay = (CDx11ColliderRay*)pObj->FindComponentFromTypeName("CDx11ColliderRay");

			// 슈팅 포인트
			CDx11GameObject* pShootingPoint = pHeadObj->FindChildFromTag("pShootingPoint");
			CDx11Transform* pShootingPointTr = pShootingPoint->GetTransform();
			pRay->SetStartPos(pShootingPointTr->GetWorldPos());

			RAY tRay = pRay->GetRay();

			//	총알 객체 생성
			CreateBullet(pShootingPointTr, tRay.vDir);

			SAFE_RELEASE(pShootingPointTr);
			SAFE_RELEASE(pShootingPoint);
			SAFE_RELEASE(pRay);
			SAFE_RELEASE(pObj);
			SAFE_RELEASE(pLayer);
		}
	}
}

void CTankInitSC::RotateWheel(float fSpeed)
{
	// 바디의 X축을 기준으로 돌린다.
	CDx11GameObject* pBodyObj = pLT->GetParent();
	CDx11Transform* pTr = pBodyObj->GetTransform();
	Vec3 AxisX = pTr->GetWorldAxis(AXIS_X);
	SAFE_RELEASE(pTr);
	SAFE_RELEASE(pBodyObj);

	CDx11GameObject* pObj = pLT->FindChildFromTag("pLTMesh");
	 pTr = pObj->GetTransform();
	pTr->RotateWorld(m_fSpeed, fSpeed, AxisX);
	SAFE_RELEASE(pTr);

	pObj = pRT->FindChildFromTag("pRTMesh");
	pTr = pObj->GetTransform();
	pTr->RotateWorld(m_fSpeed, fSpeed, AxisX);
	SAFE_RELEASE(pTr);

	pObj = pLB->FindChildFromTag("pLBMesh");
	pTr = pObj->GetTransform();
	pTr->RotateWorld(m_fSpeed, fSpeed, AxisX);
	SAFE_RELEASE(pTr);

	pObj = pRB->FindChildFromTag("pRBMesh");
	pTr = pObj->GetTransform();
	pTr->RotateWorld(m_fSpeed, fSpeed, AxisX);
	SAFE_RELEASE(pTr);
}


void CTankInitSC::SetHeight(float fTime)
{
	// 바디의 트랜스 폼을 가져온다.
	CDx11Transform* pBodyObjTr = pBodyObj->GetTransform();

	// 바디의 위치를 출력한다.
	char	strMsg[500] = {};
	sprintf_s(strMsg, "x : %.5f, z : %5.f\n", pBodyObjTr->GetWorldPos().x, pBodyObjTr->GetWorldPos().z);
	TCHAR	strMsg1[500] = {};
	MultiByteToWideChar(CP_ACP, 0, strMsg, -1, strMsg1, strlen(strMsg) * 2);
	DX11_GET_SINGLE(CDx11Debug)->OutputWindowTitle(strMsg1);

	// 테레인 오브젝트를 찾는다.
	CDx11GameObject* pTerrainObj = m_pScene->FindGameObject("TerrainObj");
	CDx11Terrain* pTerrain = (CDx11Terrain*)pTerrainObj->FindComponentFromTag("TerrainCP");

	// 바퀴 위치에서의 평면의 노말과 위치정보를 가져온다.
	TRIANGLE2RAY LTPN = pTerrain->GetHeight(pLT);
	TRIANGLE2RAY RTPN = pTerrain->GetHeight(pRT);
	TRIANGLE2RAY LBPN = pTerrain->GetHeight(pLB);
	TRIANGLE2RAY RBPN = pTerrain->GetHeight(pRB);

	// 높이의 평균을 구한다.
	// 조금 높이 그리기 떄문에 HEIGHT_ALPHA를 더한다.
	float vAvePosY = LTPN.Pos.y + RTPN.Pos.y + LBPN.Pos.y + RBPN.Pos.y;
	vAvePosY /= 4;	vAvePosY += HEIGHT_ALPHA;

	// 탱크 오브젝트의 트랜스폼을 가져온다.
	// x, z는 유지하면서 y값만 교체한다.
	CDx11Transform* pTankObjTr = m_pGameObject->GetTransform();
	Vec3 vMainPos = pTankObjTr->GetWorldPos();
	vMainPos.y = vAvePosY;

	// 바디 오브젝트를 바퀴의 평균높이로 옮긴다.
	pBodyObjTr->SetWorldPos(fTime * 10, vMainPos);

	SAFE_RELEASE(pTerrainObj);
	SAFE_RELEASE(pTerrain);
	SAFE_RELEASE(pTankObjTr);

	// 바디 오브젝트의 y축을 받아온다.
	Vec3 vMain_AxisY = pBodyObjTr->GetWorldAxis(AXIS_Y);

	// 바퀴에서의 노말의 평균과 Y축을 이용해 회전시킨다.
	// 회전량과 회전축을 구한다.
	Vec3 vAveNormal = LTPN.Normal + RTPN.Normal + LBPN.Normal + RBPN.Normal;

	vAveNormal /= 4;
	vAveNormal = vAveNormal.Normalize();

	float fAngle = vMain_AxisY.Dot(vAveNormal);
	Vec3 vRotation_Axis = vMain_AxisY.Cross(vAveNormal);

	// 회전량과 회전축을 이용해 회전시킨다.
	pBodyObjTr->RotateWorld(fAngle, fTime * 10, vRotation_Axis);

	//////////////////////////////////////////////////////////////////////////////////////////////////////
	// 바드 오브젝트의 xz 평면과 평행한 z축을 가져온다.
	Vec3 vAxisZ = pBodyObjTr->GetWorldAxis(AXIS_Z);
	vAxisZ.y = 0;
	vAxisZ = vAxisZ.Normalize();

	// 부모오브젝트의 Z축을 가져온다.
	Vec3 ParentObj = m_pTransform->GetWorldAxis(AXIS_Z);
	ParentObj = ParentObj.Normalize();

	// 두 축을 이용해서 회전량과 회전축을 구한다.
	 fAngle = ParentObj.Dot(vAxisZ);
	Vec3 vAxis = ParentObj.Cross(vAxisZ);

	// 부모의 축을 회전시킨다.
	pBodyObjTr->DeleteParentFlag(TPF_ROT);
	m_pTransform->RotateWorld(fAngle, 1, vAxis);
	pBodyObjTr->AddParentFlag(TPF_ROT);

	// 축이 돌았을 때 원래대로 돌린다.
	vMain_AxisY = pBodyObjTr->GetWorldAxis(AXIS_Y);

	if (vMain_AxisY.y < 0) {
		m_pTransform->SetWorldRotY(DX11_PI);
	}

	SAFE_RELEASE(pBodyObjTr);
}

void CTankInitSC::CreateBullet(CDx11Transform * pTransform, const Vec3 & vDir)
{
	// 총알 오브젝트 추가
	CDx11GameObject*	pBulletObj = CDx11GameObject::Create("pBulletObj");
	m_pLayer->AddObject(pBulletObj);

	// 초기 위치를 정한다.
	CDx11Transform*		pBulletObjTr = pBulletObj->GetTransform();
	pBulletObjTr->Copy(pTransform);
	pBulletObjTr->Scaling(0.05f, 0.05f, 0.05f);
	pBulletObjTr->SetWorldView(vDir);

	// 렌더러를 생성한다.
	CDx11Renderer* pRenderer = pBulletObj->AddComponent<CDx11Renderer>("pBulletObjRenderer");
	pRenderer->SetMesh("ColorSphere");
	pRenderer->SetShader("DefaultShader");
	pRenderer->SetMaterialDiffuseTex("Default");

	GET_SINGLE(CNetManager)->GetInst()->myShoot(pBulletObjTr->GetView());

	// 총알 스크립트를 컴포넌트에 추가한다.
	CBulletSC*	pBulletSC = pBulletObj->AddComponent<CBulletSC>("pBulletSC");
	pBulletSC->SetEmployer(m_pGameObject);
	SAFE_RELEASE(pBulletSC);

	SAFE_RELEASE(pRenderer);
	SAFE_RELEASE(pBulletObj);
	SAFE_RELEASE(pBulletObjTr);
}
