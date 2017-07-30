#include "InGameSceneSC.h"

#include "GameObject/Dx11GameObject.h"
#include "Scene/Dx11Scene.h"
#include "Scene/Dx11layer.h"
#include "Component/Dx11Renderer.h"
#include "Component/Dx11Transform.h"
#include "Component/Dx11Camera.h"
#include "Component/Dx11Camera3rd.h"
#include "Component/Dx11Terrain.h"
#include "Core/Dx11Scheduler.h"
#include "Resources/Dx11Mesh.h"
#include "Resources/Dx11ResourcesManager.h"
#include "Scene/Dx11SceneManager.h"
#include "Rendering/Dx11RenderManager.h"
#include "Component/DX11FSM.h"
#include "Core/Dx11Input.h"
#include "Component/Dx11Material.h"
#include "Component/Dx11ColliderRay.h".
#include "Component/Dx11ColliderSphere.h"
#include "Component/Dx11Animation2D.h"
#include "Component/Dx11BillBoard.h"
#include "Component/DX11ColliderFrustrum.h"
#include "Component/DX11ColliderOBB.h"

#include "LoadingSceneSC.h"
#include "../ObjScript/CoordinateArrow/CoordinateArrowSC.h"
#include "../ObjScript/Tank/TankInitSC.h"
#include "../ObjScript/Monster/MonsterSC.h"
#include "../AIState/WalkState.h"
#include "../AIState/IdleState.h"

#include "../Network/NetManager.h"
#include "../ClientCore.h"
#include "../Network/ZEROStream.h"

CInGameSceneSC::CInGameSceneSC()
{

}

CInGameSceneSC::CInGameSceneSC(const CInGameSceneSC & script)
{
	*this = script;
}


CInGameSceneSC::~CInGameSceneSC()
{
}

bool CInGameSceneSC::Init()
{
	// ��ī�̹ڽ� ����
	m_pScene->CreateSky("DesertSky", L"DesertSky.dds", "DesertSky");

	// ��ǥ�踦 �׸���.
	// CreateCoordinateArrow();

	// ������ �����Ѵ�.
	CreateAim();
	CreateAim2();

	// �ͷ��� ����
	CreateTerrain();

	// �׵θ� ������ �����Ѵ�.
	CreateWall();

	// ���͸� �����Ѵ�.
	CrateMonster();

	// �ٴڿ� �̳� �߰�
	CreateTerrainTexs();

	// ķ�� ���̾�
	CreateCampFire();

	// ��ֹ��� ��ġ�Ѵ�.
	CreateGrass();
	CreateCactus2();
	CreateCactus3();
	CreateCactus_Short1();
	CreateCactus_Short2();
	CreateCactus_Short3();
	CreateLeafy();
	CreateRock_Cone();
	CreateRock_Heavy();
	CreateRock_Large();
	CreateRock_Small();

	// �÷��̾� 1 ��ũ�� �����Ѵ�.
	CreateTank();

	return true;
}

int CInGameSceneSC::Update(float fTime)
{
	return 0;
}

int CInGameSceneSC::LateUpdate(float fTime)
{
	return 0;
}

void CInGameSceneSC::Render(float fTime)
{
}

CInGameSceneSC * CInGameSceneSC::Clone()
{
	return new CInGameSceneSC(*this);
}

void CInGameSceneSC::CreateAim()
{
	// ���̾ ã�´�.
	CDx11Layer*	pLayer = m_pScene->FindLayer("DefaultLayer");

	if (!pLayer)
		return;

	// ���� ������Ʈ�� �����Ѵ�.
	CDx11GameObject*	pAimObj = CDx11GameObject::Create("pAimObj");

	pLayer->AddObject(pAimObj);
	
	// ũ��, ��ġ ����
	CDx11Transform*		pAimObjTr = pAimObj->GetTransform();
	RESOLUTION_TYPE eRS = DX11_GET_SINGLE(CDx11Device)->GetResolutionType();

	const Vec3 vAimSize = { 150, 100, 1 };
	pAimObjTr->Scaling(vAimSize.x, vAimSize.y, 1.f);
	pAimObjTr->SetWorldPos(
		g_tRS[eRS].iWidth / 2- vAimSize.x/2,
		g_tRS[eRS].iHeight / 2- vAimSize.y/2,
		0.f);

	// ������ ����
	CDx11Renderer*	pRenderer = pAimObj->AddComponent<CDx11Renderer>("AimRederer");
	pRenderer->SetCameraTag("UICamera");
	pRenderer->SetMesh("UIMesh");
	pRenderer->SetShader("UIShader");
	pRenderer->SetRenderState(DX11_RS_DEPTH_DISABLE, RSF_DEPTHSTENCIL);
	pRenderer->SetRenderState(DX11_RS_ALPHABLEND, RSF_BLEND);

	pRenderer->CreateMaterial("Aim", L"CrossHairs/cross hairs_green.png");
	pRenderer->SetMaterialDiffuseTex("Aim");


	CDx11ColliderRay*	pRay = pAimObj->AddComponent<CDx11ColliderRay>("AimColl");
	SAFE_RELEASE(pRay);

	SAFE_RELEASE(pRenderer);
	SAFE_RELEASE(pAimObj);
	SAFE_RELEASE(pAimObjTr);
	SAFE_RELEASE(pLayer);
}

void CInGameSceneSC::CreateAim2()
{
}

void CInGameSceneSC::CreateTerrain()
{
	// ���̾ ã�´�.
	CDx11Layer*	pLayer = m_pScene->FindLayer("DefaultLayer");

	if (!pLayer)
		return;

	// �ͷ��� ������Ʈ
	CDx11GameObject* pTerrainObj = CDx11GameObject::Create("TerrainObj");
	CDx11Terrain* pTerrianCP = pTerrainObj->AddComponent<CDx11Terrain>("TerrainCP");
	pTerrianCP->CreateTerrian(80, 80, 10, "Height6.bmp", L"Terrain/G3.png", NULL, NULL);

	// �ʱ� ũ�� ����
	CDx11Transform* pTr = pTerrainObj->GetTransform();
	pTr->Scaling(2.5, 1, 2.5);

	pLayer->AddObject(pTerrainObj);

	SAFE_RELEASE(pTr);
	SAFE_RELEASE(pTerrianCP);
	SAFE_RELEASE(pTerrainObj);

	SAFE_RELEASE(pLayer);
}

void CInGameSceneSC::CreateCoordinateArrow()
{
	// ���̾� ����
	CDx11Layer*	pDefaultLayer = m_pScene->FindLayer("DefaultLayer");

	// Y�� ������ Ÿ�� ����
	CDx11GameObject* pCoordinateArrowObj = CDx11GameObject::Create("CoordinateArrowObj", OCT_PROTOTYPE);
	CCoordinateArrowSC* pCoordinateArrowSC = pCoordinateArrowObj->AddComponent<CCoordinateArrowSC>("CoordinateArrowSC");
	SAFE_RELEASE(pCoordinateArrowSC);
	SAFE_RELEASE(pCoordinateArrowObj);


	// Y ��
	pCoordinateArrowObj = CDx11GameObject::Create("CoordinateArrowObj", OCT_CLONE);
	pDefaultLayer->AddObject(pCoordinateArrowObj);
	SAFE_RELEASE(pCoordinateArrowObj);


	// X��
	pCoordinateArrowObj = CDx11GameObject::Create("CoordinateArrowObj", OCT_CLONE);
	CDx11Transform* pTr = pCoordinateArrowObj->GetTransform();
	pTr->SetWorldRotZ(DX11_PI / 2.f);
	SAFE_RELEASE(pTr);

	pDefaultLayer->AddObject(pCoordinateArrowObj);
	SAFE_RELEASE(pCoordinateArrowObj);


	// Z��
	pCoordinateArrowObj = CDx11GameObject::Create("CoordinateArrowObj", OCT_CLONE);
	pTr = pCoordinateArrowObj->GetTransform();
	pTr->SetWorldRotX(DX11_PI / 2.f);
	SAFE_RELEASE(pTr);

	pDefaultLayer->AddObject(pCoordinateArrowObj);
	SAFE_RELEASE(pCoordinateArrowObj);



	// ���̾� ����
	SAFE_RELEASE(pDefaultLayer);
}

void CInGameSceneSC::CreateTank()
{
	// ���̾ ã�´�.
	CDx11Layer*	pLayer = m_pScene->FindLayer("DefaultLayer");

	if (!pLayer)
		return;

	// ��ũ������Ʈ�� �߰��Ѵ�.
	CDx11GameObject* pTankObj = CDx11GameObject::Create("pTankObj");

	// ���̾ �߰��Ѵ�.
	pLayer->AddObject(pTankObj);

	// ��ũ��Ʈ�� �߰��Ѵ�.
	CTankInitSC* pTankInitSC = pTankObj->AddComponent<CTankInitSC>("pTankInitSC");

	// ��ũ 1�� �ʱ���ġ�� ��´�.
	// ��ũ 1�� 30, 30
	CDx11Transform* pTr =  pTankObj->GetTransform();
	pTr->SetWorldPos(45, 0, 96);

	// ������ �����.
	CDx11GameObject* pTerrainObj = m_pScene->FindGameObject("TerrainObj");
	CDx11Terrain* pTerrain = (CDx11Terrain*)pTerrainObj->FindComponentFromTag("TerrainCP");

	// ���� ��ġ������ ����� �븻�� ��ġ������ �����´�.
	TRIANGLE2RAY PosNormal = pTerrain->GetHeight(pTankObj);

	// �� ������Ʈ�� �ű��.
	pTr->SetWorldPos(PosNormal.Pos);

	// ������
	SAFE_RELEASE(pTerrainObj);
	SAFE_RELEASE(pTerrain);
	SAFE_RELEASE(pTr);

	// ������Ʈ ������
	// ������Ʈ, ��ũ��Ʈ ������
	SAFE_RELEASE(pTankInitSC);
	SAFE_RELEASE(pTankObj);

	//���̾ �����.
	SAFE_RELEASE(pLayer);
}

void CInGameSceneSC::CrateMonster()
{
	// ���̾ ã�´�.
	CDx11Layer*	pLayer = m_pScene->FindLayer("DefaultLayer");

	if (!pLayer)
		return;

	vector<int> vPosX, vPosZ, vRot;

	vPosX.push_back(21);			vPosZ.push_back(71);			 vRot.push_back(90);			// ������ ��
	vPosX.push_back(37.5);		vPosZ.push_back(124);			vRot.push_back(45);				// �Ҳ� ��
	vPosX.push_back(36);			vPosZ.push_back(126);			 vRot.push_back(90);
	vPosX.push_back(37);			vPosZ.push_back(128);			vRot.push_back(135);
	vPosX.push_back(39);			vPosZ.push_back(129);			 vRot.push_back(180);
	vPosX.push_back(41.5);		vPosZ.push_back(127);			 vRot.push_back(225);
	vPosX.push_back(41);			vPosZ.push_back(124);			 vRot.push_back(315);
	vPosX.push_back(94);			vPosZ.push_back(60);			 vRot.push_back(45);			// �� �ܴ� ��
	vPosX.push_back(96);			vPosZ.push_back(62);			 vRot.push_back(270);
	vPosX.push_back(92);			vPosZ.push_back(58);			 vRot.push_back(315);
	vPosX.push_back(95.5);		vPosZ.push_back(64);			 vRot.push_back(10);
	vPosX.push_back(91.5);		vPosZ.push_back(60);			 vRot.push_back(220);
	vPosX.push_back(110);			vPosZ.push_back(102);			 vRot.push_back(10);			// ���� ��
	vPosX.push_back(110);			vPosZ.push_back(100);			 vRot.push_back(300);
	vPosX.push_back(112);			vPosZ.push_back(108);			 vRot.push_back(150);
	vPosX.push_back(114);			vPosZ.push_back(104);			 vRot.push_back(240);
	vPosX.push_back(108);			vPosZ.push_back(98);			 vRot.push_back(315);
	vPosX.push_back(106);			vPosZ.push_back(96);			 vRot.push_back(270);
	vPosX.push_back(107);			vPosZ.push_back(105);			 vRot.push_back(270);
	vPosX.push_back(112);			vPosZ.push_back(96);			 vRot.push_back(270);
	vPosX.push_back(117);			vPosZ.push_back(100);			 vRot.push_back(180);
	vPosX.push_back(100);			vPosZ.push_back(101);			 vRot.push_back(225
	);


	for (int i = 0; i < vPosX.size(); ++i) {

		// ��ũ������Ʈ�� �߰��Ѵ�.
		CDx11GameObject* pMonsterObj = CDx11GameObject::Create("pMonsterObj");

		// ���̾ �߰��Ѵ�.
		pLayer->AddObject(pMonsterObj);

		// ��ũ��Ʈ�� �߰��Ѵ�.
		CMonsterSC* pMonsterSC = pMonsterObj->AddComponent<CMonsterSC>("pMonsterSC");

		//  �ʱ���ġ�� ��´�.
		CDx11Transform* pTr = pMonsterObj->GetTransform();
		pTr->SetWorldPos(vPosX[i], 0, vPosZ[i]);
		pTr->SetWorldRotY(DX11_PI / 180 * vRot[i]);

		// ������ �����.
		CDx11GameObject* pTerrainObj = m_pScene->FindGameObject("TerrainObj");
		CDx11Terrain* pTerrain = (CDx11Terrain*)pTerrainObj->FindComponentFromTag("TerrainCP");

		// ���� ��ġ������ ����� �븻�� ��ġ������ �����´�.
		TRIANGLE2RAY PosNormal = pTerrain->GetHeight(pMonsterObj);

		// �� ������Ʈ�� �ű��.
		pTr->SetWorldPos(PosNormal.Pos);

		// ���� �ӽ��� �����Ѵ�.
		CDx11FSM* pFSM = pMonsterObj->AddComponent<CDx11FSM>("FSM");
		CIdleState* pState = pFSM->SetState<CIdleState>(pMonsterObj);
		//CWalkState* pState = pFSM->SetState<CWalkState>(pMonsterObj);

		SAFE_RELEASE(pState);
		SAFE_RELEASE(pFSM);

		// ������
		SAFE_RELEASE(pTerrainObj);
		SAFE_RELEASE(pTerrain);

		SAFE_RELEASE(pTr);

		// ������Ʈ ������
		// ������Ʈ, ��ũ��Ʈ ������
		SAFE_RELEASE(pMonsterSC);
		SAFE_RELEASE(pMonsterObj);
	}

	//���̾ �����.
	SAFE_RELEASE(pLayer);
}

void CInGameSceneSC::CreateWall()
{
	// ���̾ ã�´�.
	CDx11Layer*	pLayer = m_pScene->FindLayer("DefaultLayer");                

	if (!pLayer)
		return;

	// ����
	for (int i = 0; i < 20; ++i) {
		// ������Ʈ�� �����.
		CDx11GameObject* pObj = CDx11GameObject::Create("pObj");
		pLayer->AddObject(pObj);

		// �������� �߰��Ѵ�.
		CDx11Renderer* pRenderer = pObj->AddComponent<CDx11Renderer>("pRenderer");
		pRenderer->SetMesh("CubeMesh");
		pRenderer->SetShader("TexShader");
		pRenderer->CreateMaterial("StoneWall", L"StoneWall.PNG");

		// Ʈ������
		CDx11Transform* pTr = pObj->GetTransform();
		pTr->Scaling(2, 10, 10);
		pTr->SetWorldPos(0, 8, 5 + (i*10));

		SAFE_RELEASE(pObj);
		SAFE_RELEASE(pRenderer);
		SAFE_RELEASE(pTr);
	}

	// ��
	for (int i = 0; i < 20; ++i) {
		// ������Ʈ�� �����.
		CDx11GameObject* pObj = CDx11GameObject::Create("pObj");
		pLayer->AddObject(pObj);

		// �������� �߰��Ѵ�.
		CDx11Renderer* pRenderer = pObj->AddComponent<CDx11Renderer>("pRenderer");
		pRenderer->SetMesh("CubeMesh");
		pRenderer->SetShader("TexShader");
		pRenderer->CreateMaterial("StoneWall2", L"StoneWall2.PNG");

		// Ʈ������
		CDx11Transform* pTr = pObj->GetTransform();
		pTr->Scaling(10, 10, 2);
		pTr->SetWorldPos(5 + (i * 10), 8, 198);

		SAFE_RELEASE(pObj);
		SAFE_RELEASE(pRenderer);
		SAFE_RELEASE(pTr);
	}

	// ������
	for (int i = 0; i < 20; ++i) {
		// ������Ʈ�� �����.
		CDx11GameObject* pObj = CDx11GameObject::Create("pObj");
		pLayer->AddObject(pObj);

		// �������� �߰��Ѵ�.
		CDx11Renderer* pRenderer = pObj->AddComponent<CDx11Renderer>("pRenderer");
		pRenderer->SetMesh("CubeMesh");
		pRenderer->SetShader("TexShader");
		pRenderer->CreateMaterial("StoneWall2", L"StoneWall2.PNG");

		// Ʈ������
		CDx11Transform* pTr = pObj->GetTransform();
		pTr->Scaling(2, 10, 10);
		pTr->SetWorldPos(198, 8, 5 + (i * 10));

		SAFE_RELEASE(pObj);
		SAFE_RELEASE(pRenderer);
		SAFE_RELEASE(pTr);
	}

	// �Ʒ�
	for (int i = 0; i < 20; ++i) {
		// ������Ʈ�� �����.
		CDx11GameObject* pObj = CDx11GameObject::Create("pObj");
		pLayer->AddObject(pObj);

		// �������� �߰��Ѵ�.
		CDx11Renderer* pRenderer = pObj->AddComponent<CDx11Renderer>("pRenderer");
		pRenderer->SetMesh("CubeMesh");
		pRenderer->SetShader("TexShader");
		pRenderer->CreateMaterial("StoneWall", L"StoneWall.PNG");

		// Ʈ������
		CDx11Transform* pTr = pObj->GetTransform();
		pTr->Scaling(10, 10, 2);
		pTr->SetWorldPos(5 + (i * 10), 8, 0);

		SAFE_RELEASE(pObj);
		SAFE_RELEASE(pRenderer);
		SAFE_RELEASE(pTr);
	}

	SAFE_RELEASE(pLayer);
}

void CInGameSceneSC::CreateGrass()
{
	for (int i = 0; i < 300; ++i) {
		// �޽� ȸ��, ũ��, ��ġ

		//CDx11GameObject* pBush = CreateObj("Bush1",
		//	/*ȸ��*/	0, DX11_PI/180*-5, 0,
		//	/*ũ��*/	0.02, 0.01, 0.02,
		//	/*��ġ*/	0, 0, 0);

		float fRot = rand()%180;
		int R = rand() % 30;

		CDx11GameObject* pBush;
			
		if(R<10)
		pBush = CreateObj("Leaves",
			/*ȸ��*/	0, 0, 0,
			/*ũ��*/	0.03, 0.022, 0.03,
			/*��ġ*/	0, 0.2, 0);

		else if (R >=10 || R<25)
		pBush = CreateObj("Leafy1",
			/*ȸ��*/	-DX11_PI / 2, 0, 0,
			/*ũ��*/	0.01, 0.01, 0.01,
			/*��ġ*/	0, 0, 0);

		else
		pBush = CreateObj("Leafy4",
			/*ȸ��*/	-DX11_PI / 2, 0, 0,
			/*ũ��*/	0.01, 0.01, 0.01,
			/*��ġ*/	0, 0.2, 0);

		pBush->SetCollision(false);

		CDx11Transform* pTr = pBush->GetTransform();

		//��ü �ʱ���ġ
		float fRandPosX = rand() % 1600 + 200; fRandPosX *= 0.1;
		float fRandPosY = rand() % 1600 + 200; fRandPosY *= 0.1;

		pTr->SetWorldPos(fRandPosX, 0, fRandPosY);
		pTr->SetWorldRotY(DX11_PI/180*fRot);

		// ������ �����.
		CDx11GameObject* pTerrainObj = m_pScene->FindGameObject("TerrainObj");
		CDx11Terrain* pTerrain = (CDx11Terrain*)pTerrainObj->FindComponentFromTag("TerrainCP");

		// ���� ��ġ������ ����� �븻�� ��ġ������ �����´�.
		TRIANGLE2RAY PosNormal = pTerrain->GetHeight(pBush);

		// �� ������Ʈ�� �ű��.
		pTr->SetWorldPos(PosNormal.Pos);

		// �ø��� ���� �ݸ������Ǿ�
		CDx11ColliderSphere* pSphere = pBush->AddComponent<CDx11ColliderSphere>("pSphere");
		pSphere->SetSphereInfo(0.5, PosNormal.Pos);
		SAFE_RELEASE(pSphere);

		// ������
		SAFE_RELEASE(pTerrainObj);
		SAFE_RELEASE(pTerrain);

		SAFE_RELEASE(pTr);
		SAFE_RELEASE(pBush);
	}
}

void CInGameSceneSC::CreateCampFire()
{
	// �޽� ȸ��, ũ��, ��ġ
	CDx11GameObject* pCactus = CreateObj("CampFire",
		/*ȸ��*/	0, 0, 0,
		/*ũ��*/	1, 1, 1,
		/*��ġ*/	0, 0.2, 0);

	pCactus->SetCollision(false);

	CDx11Transform* pTr = pCactus->GetTransform();

	//��ü �ʱ���ġ
	float fRot = rand() % 180;
	pTr->SetWorldRotY(fRot);
	pTr->SetWorldPos(39, 0, 126);

	// ������ �����.
	CDx11GameObject* pTerrainObj = m_pScene->FindGameObject("TerrainObj");
	CDx11Terrain* pTerrain = (CDx11Terrain*)pTerrainObj->FindComponentFromTag("TerrainCP");

	// ���� ��ġ������ ����� �븻�� ��ġ������ �����´�.
	TRIANGLE2RAY PosNormal = pTerrain->GetHeight(pCactus);

	// �� ������Ʈ�� �ű��.
	pTr->SetWorldPos(PosNormal.Pos);

	// �ø��� ���� �ݸ������Ǿ�
	CDx11ColliderSphere* pSphere = pCactus->AddComponent<CDx11ColliderSphere>("pSphere");
	pSphere->SetSphereInfo(0.5, PosNormal.Pos);
	SAFE_RELEASE(pSphere);

	// ������
	SAFE_RELEASE(pTerrainObj);
	SAFE_RELEASE(pTerrain);
	SAFE_RELEASE(pCactus);

	// �Ҳ�

	// ���̾ ã�´�.
	CDx11Layer*	pLayer = m_pScene->FindLayer("DefaultLayer");

	if (!pLayer)
		return;

	CDx11GameObject*	pSmallEffectObj = CDx11GameObject::Create("SmallEffectObject");
	pLayer->AddObject(pSmallEffectObj);

	pSmallEffectObj->SetCollision(false);

	CDx11Transform*	pEffectTr = pSmallEffectObj->GetTransform();
	pEffectTr->Scaling(2, 2.5, 2);

	Vec3 vEffectPos = pTr->GetWorldPos();	vEffectPos.y += 1.5;
	pEffectTr->SetWorldPos(vEffectPos);
	SAFE_RELEASE(pEffectTr);

	CDx11Renderer* pRenderer = pSmallEffectObj->AddComponent<CDx11Renderer>("SmallRenderer");
	pRenderer->CreateMaterial("SmallExplosion1");
	pRenderer->SetShader(EFFECTSHADER);
	pRenderer->SetMesh("EffectCenterMesh");
	pRenderer->SetRenderState(DX11_RS_ALPHABLEND, RSF_BLEND);
	SAFE_RELEASE(pRenderer);

	CDx11BillBoard* pBillBoard = pSmallEffectObj->AddComponent<CDx11BillBoard>("SmallBillBoard");
	SAFE_RELEASE(pBillBoard);

	CDx11Animation2D* pAnimation2D = pSmallEffectObj->AddComponent<CDx11Animation2D>("SmallAnim2D");
	pAnimation2D->Init();
	pAnimation2D->SetMaxFrame(75, 1);

	pAnimation2D->SetAnimation2DOption(A2DO_LOOP);
	pAnimation2D->SetAnimationLimitTime(1.f);

	for (int i = 1; i <= 75; ++i)
	{
		char	strKey[256] = {};
		sprintf_s(strKey, "fire1_%d", i);
		wchar_t	strPath[MAX_PATH] = {};
		wsprintf(strPath, L"Fire/fire1_%d.png", i);
		pAnimation2D->AddFrameTexture(strKey, strPath);
	}

	SAFE_RELEASE(pAnimation2D);
	SAFE_RELEASE(pSmallEffectObj);
	SAFE_RELEASE(pLayer);
	SAFE_RELEASE(pTr);
}

void CInGameSceneSC::CreateCactus2()
{
	vector<int> vPosX, vPosZ;

	vPosX.push_back(14); vPosZ.push_back(100);
	vPosX.push_back(34); vPosZ.push_back(106);
	vPosX.push_back(51); vPosZ.push_back(158);
	vPosX.push_back(8); vPosZ.push_back(109);
	vPosX.push_back(250); vPosZ.push_back(79);
	vPosX.push_back(172); vPosZ.push_back(131);
	vPosX.push_back(70); vPosZ.push_back(164);


	for (int i = 0; i < vPosX.size(); ++i) {
		// �޽� ȸ��, ũ��, ��ġ
		CDx11GameObject* pCactus = CreateObj("Cactus2",
			/*ȸ��*/	-DX11_PI / 2, 0, 0,
			/*ũ��*/	0.02, 0.02, 0.02,
			/*��ġ*/	0, -0.2, 0);
		CDx11Transform* pTr = pCactus->GetTransform();

		//��ü �ʱ���ġ
		float fRot = rand() % 180;
		pTr->SetWorldRotY(fRot);
		pTr->SetWorldPos(vPosX[i], 0, vPosZ[i]);

		// ������ �����.
		CDx11GameObject* pTerrainObj = m_pScene->FindGameObject("TerrainObj");
		CDx11Terrain* pTerrain = (CDx11Terrain*)pTerrainObj->FindComponentFromTag("TerrainCP");

		// ���� ��ġ������ ����� �븻�� ��ġ������ �����´�.
		TRIANGLE2RAY PosNormal = pTerrain->GetHeight(pCactus);

		// �� ������Ʈ�� �ű��.
		pTr->SetWorldPos(PosNormal.Pos);

		CDx11ColliderOBB* pOBB = pCactus->AddComponent<CDx11ColliderOBB>(STATIC_MESH);
		float fAxisLen[3];
		fAxisLen[0] = 0.4f;
		fAxisLen[1] = 7.f;
		fAxisLen[2] = 0.4f;
		pOBB->SetOBBInfo(fAxisLen, pTr);
		SAFE_RELEASE(pOBB);

		// �ø��� ���� �ݸ������Ǿ�
		CDx11ColliderSphere* pSphere = pCactus->AddComponent<CDx11ColliderSphere>(STATIC_MESH);
		pSphere->SetSphereInfo(0.5, PosNormal.Pos);
		SAFE_RELEASE(pSphere);

		// ������
		SAFE_RELEASE(pTerrainObj);
		SAFE_RELEASE(pTerrain);

		SAFE_RELEASE(pTr);
		SAFE_RELEASE(pCactus);
	}
}

void CInGameSceneSC::CreateCactus3()
{
	vector<int> vPosX, vPosZ;

	vPosX.push_back(27); vPosZ.push_back(26);
	vPosX.push_back(30); vPosZ.push_back(30);
	vPosX.push_back(49); vPosZ.push_back(24);
	vPosX.push_back(61); vPosZ.push_back(45);
	vPosX.push_back(65); vPosZ.push_back(81);
	vPosX.push_back(73); vPosZ.push_back(82);
	vPosX.push_back(22); vPosZ.push_back(36);
	vPosX.push_back(21); vPosZ.push_back(69);
	vPosX.push_back(96); vPosZ.push_back(34);
	vPosX.push_back(115); vPosZ.push_back(29);
	vPosX.push_back(136); vPosZ.push_back(62);
	vPosX.push_back(128); vPosZ.push_back(65);
	vPosX.push_back(131); vPosZ.push_back(75);
	vPosX.push_back(36);  vPosZ.push_back(105);
	vPosX.push_back(32);  vPosZ.push_back(142);
	vPosX.push_back(36);  vPosZ.push_back(146);
	vPosX.push_back(53);  vPosZ.push_back(161);
	vPosX.push_back(74);  vPosZ.push_back(164);
	vPosX.push_back(114); vPosZ.push_back(166);
	vPosX.push_back(158); vPosZ.push_back(139);
	vPosX.push_back(173); vPosZ.push_back(107);
	vPosX.push_back(123); vPosZ.push_back(105);
	vPosX.push_back(129); vPosZ.push_back(116);

	for (int i = 0; i < vPosX.size(); ++i) {
		// �޽� ȸ��, ũ��, ��ġ
		CDx11GameObject* pCactus = CreateObj("Cactus3",
			/*ȸ��*/	-DX11_PI / 2, 0, 0,
			/*ũ��*/	0.02, 0.015, 0.02,
			/*��ġ*/	0, -0.2, 0);

		//CDX11ColliderFrustrum* pColl = pCactus->AddComponent<CDX11ColliderFrustrum>(STATIC_MESH);
		//pColl->SetFrustrumScale(1, 1, 1);
		//pColl->SetInfo("Cac3", 0.5, 7, 0.5, 0.5, 7, 0.5);
		//SAFE_RELEASE(pColl);

		CDx11Transform* pTr = pCactus->GetTransform();

		CDx11ColliderOBB* pOBB = pCactus->AddComponent<CDx11ColliderOBB>(STATIC_MESH);
		float fAxisLen[3];
		fAxisLen[0] = 0.5f;
		fAxisLen[1] = 7.f;
		fAxisLen[2] = 0.5f;
		pOBB->SetOBBInfo(fAxisLen, pTr);
		SAFE_RELEASE(pOBB);

		//��ü �ʱ���ġ
		float fRot = rand() % 180;
		pTr->SetWorldRotY(fRot);
		pTr->SetWorldPos(vPosX[i], 0, vPosZ[i]);

		// ������ �����.
		CDx11GameObject* pTerrainObj = m_pScene->FindGameObject("TerrainObj");
		CDx11Terrain* pTerrain = (CDx11Terrain*)pTerrainObj->FindComponentFromTag("TerrainCP");

		// ���� ��ġ������ ����� �븻�� ��ġ������ �����´�.
		TRIANGLE2RAY PosNormal = pTerrain->GetHeight(pCactus);

		// �� ������Ʈ�� �ű��.
		pTr->SetWorldPos(PosNormal.Pos);

		// �ø��� ���� �ݸ������Ǿ�
		CDx11ColliderSphere* pSphere = pCactus->AddComponent<CDx11ColliderSphere>(STATIC_MESH);
		pSphere->SetSphereInfo(0.5, PosNormal.Pos);
		SAFE_RELEASE(pSphere);

		// ������
		SAFE_RELEASE(pTerrainObj);
		SAFE_RELEASE(pTerrain);

		SAFE_RELEASE(pTr);
		SAFE_RELEASE(pCactus);
	}
}

void CInGameSceneSC::CreateCactus_Short1()
{
	vector<int> vPosX, vPosZ;

	vPosX.push_back(22); vPosZ.push_back(31);
	vPosX.push_back(23); vPosZ.push_back(57);
	vPosX.push_back(33); vPosZ.push_back(104);
	vPosX.push_back(34); vPosZ.push_back(147);
	vPosX.push_back(77); vPosZ.push_back(163);
	vPosX.push_back(102); vPosZ.push_back(164);
	vPosX.push_back(126); vPosZ.push_back(120);
	vPosX.push_back(173); vPosZ.push_back(109);
	vPosX.push_back(189); vPosZ.push_back(65);
	vPosX.push_back(169); vPosZ.push_back(36);
	vPosX.push_back(88); vPosZ.push_back(18);

	for (int i = 0; i < vPosX.size(); ++i) {
		// �޽� ȸ��, ũ��, ��ġ
		CDx11GameObject* pCactus = CreateObj("Cactus_Short1",
			/*ȸ��*/	-DX11_PI / 2, 0, 0,
			/*ũ��*/	0.02, 0.02, 0.02,
			/*��ġ*/	0, 0, 0);

		pCactus->SetCollision(false);

		CDx11Transform* pTr = pCactus->GetTransform();

		//��ü �ʱ���ġ
		float fRot = rand() % 180;
		pTr->SetWorldRotY(fRot);
		pTr->SetWorldPos(vPosX[i], 0, vPosZ[i]);

		// ������ �����.
		CDx11GameObject* pTerrainObj = m_pScene->FindGameObject("TerrainObj");
		CDx11Terrain* pTerrain = (CDx11Terrain*)pTerrainObj->FindComponentFromTag("TerrainCP");

		// ���� ��ġ������ ����� �븻�� ��ġ������ �����´�.
		TRIANGLE2RAY PosNormal = pTerrain->GetHeight(pCactus);

		// �� ������Ʈ�� �ű��.
		pTr->SetWorldPos(PosNormal.Pos);

		// �ø��� ���� �ݸ������Ǿ�
		CDx11ColliderSphere* pSphere = pCactus->AddComponent<CDx11ColliderSphere>("pSphere");
		pSphere->SetSphereInfo(0.5, PosNormal.Pos);
		SAFE_RELEASE(pSphere);

		// ������
		SAFE_RELEASE(pTerrainObj);
		SAFE_RELEASE(pTerrain);

		SAFE_RELEASE(pTr);
		SAFE_RELEASE(pCactus);
	}
}

void CInGameSceneSC::CreateCactus_Short2()
{
}

void CInGameSceneSC::CreateCactus_Short3()
{
}

void CInGameSceneSC::CreateLeafy()
{
	vector<int> vPosX, vPosZ;

	vPosX.push_back(180); vPosZ.push_back(34);
	vPosX.push_back(31); vPosZ.push_back(29);
	vPosX.push_back(71); vPosZ.push_back(46);
	vPosX.push_back(66); vPosZ.push_back(79);
	vPosX.push_back(35); vPosZ.push_back(104);
	vPosX.push_back(33); vPosZ.push_back(141);
	vPosX.push_back(52); vPosZ.push_back(159);
	vPosX.push_back(74); vPosZ.push_back(162);
	vPosX.push_back(154); vPosZ.push_back(175);
	vPosX.push_back(171); vPosZ.push_back(130);
	vPosX.push_back(139); vPosZ.push_back(50);


	for (int i = 0; i < vPosX.size(); ++i) {

		CDx11GameObject* pBush;

			pBush = CreateObj("Leafy2",
				/*ȸ��*/	-DX11_PI / 2, 0, 0,
				/*ũ��*/	0.01, 0.01, 0.01,
				/*��ġ*/	0, 0.2, 0);

		pBush->SetCollision(false);

		CDx11Transform* pTr = pBush->GetTransform();
		pTr->SetWorldPos(vPosX[i], 0, vPosZ[i]);

		// ������ �����.
		CDx11GameObject* pTerrainObj = m_pScene->FindGameObject("TerrainObj");
		CDx11Terrain* pTerrain = (CDx11Terrain*)pTerrainObj->FindComponentFromTag("TerrainCP");

		// ���� ��ġ������ ����� �븻�� ��ġ������ �����´�.
		TRIANGLE2RAY PosNormal = pTerrain->GetHeight(pBush);

		// �� ������Ʈ�� �ű��.
		pTr->SetWorldPos(PosNormal.Pos);

		// �ø��� ���� �ݸ������Ǿ�
		CDx11ColliderSphere* pSphere = pBush->AddComponent<CDx11ColliderSphere>("pSphere");
		pSphere->SetSphereInfo(0.5, PosNormal.Pos);
		SAFE_RELEASE(pSphere);

		// ������
		SAFE_RELEASE(pTerrainObj);
		SAFE_RELEASE(pTerrain);

		SAFE_RELEASE(pTr);
		SAFE_RELEASE(pBush);
	}
}


void CInGameSceneSC::CreateRock_Cone()
{
	vector<int> vPosX, vPosZ;

	vPosX.push_back(16); vPosZ.push_back(90);
	vPosX.push_back(21); vPosZ.push_back(92);
	vPosX.push_back(70); vPosZ.push_back(48);

	vPosX.push_back(16 + 50); vPosZ.push_back(90 + 50);
	vPosX.push_back(21 + 50); vPosZ.push_back(92 + 50);

	vPosX.push_back(120); vPosZ.push_back(110);
	vPosX.push_back(21 + 100); vPosZ.push_back(92 + 15);
	vPosX.push_back(141); vPosZ.push_back(50);
	vPosX.push_back(32); vPosZ.push_back(146);
	vPosX.push_back(132); vPosZ.push_back(77);
	vPosX.push_back(112); vPosZ.push_back(167);

	for (int i = 0; i < vPosX.size(); ++i) {
		// �޽� ȸ��, ũ��, ��ġ
		CDx11GameObject* pCactus = CreateObj("Rock_Cone",
			/*ȸ��*/	-DX11_PI / 2, 0, 0,
			/*ũ��*/	0.02, 0.02, 0.02,
			/*��ġ*/	0, -0.2, 0);

		//CDX11ColliderFrustrum* pColl = pCactus->AddComponent<CDX11ColliderFrustrum>(STATIC_MESH);
		//pColl->SetFrustrumScale(1, 1, 1);
		//pColl->SetInfo("Cone", 0.6, 7, 0.6, 0.6, 7, 0.6);
		//SAFE_RELEASE(pColl);

		CDx11Transform* pTr = pCactus->GetTransform();

		CDx11ColliderOBB* pOBB = pCactus->AddComponent<CDx11ColliderOBB>(STATIC_MESH);
		float fAxisLen[3];
		fAxisLen[0] = 0.65;
		fAxisLen[1] = 7.f;
		fAxisLen[2] = 0.65;
		pOBB->SetOBBInfo(fAxisLen, pTr);
		SAFE_RELEASE(pOBB);

		//��ü �ʱ���ġ
		float fRot = rand() % 180;
		pTr->SetWorldRotY(fRot);
		pTr->SetWorldPos(vPosX[i], 0, vPosZ[i]);

		// ������ �����.
		CDx11GameObject* pTerrainObj = m_pScene->FindGameObject("TerrainObj");
		CDx11Terrain* pTerrain = (CDx11Terrain*)pTerrainObj->FindComponentFromTag("TerrainCP");

		// ���� ��ġ������ ����� �븻�� ��ġ������ �����´�.
		TRIANGLE2RAY PosNormal = pTerrain->GetHeight(pCactus);

		// �� ������Ʈ�� �ű��.
		pTr->SetWorldPos(PosNormal.Pos);

		// �ø��� ���� �ݸ������Ǿ�
		CDx11ColliderSphere* pSphere = pCactus->AddComponent<CDx11ColliderSphere>(STATIC_MESH);
		pSphere->SetSphereInfo(0.5, PosNormal.Pos);
		SAFE_RELEASE(pSphere);

		// ������
		SAFE_RELEASE(pTerrainObj);
		SAFE_RELEASE(pTerrain);

		SAFE_RELEASE(pTr);
		SAFE_RELEASE(pCactus);
	}
}

void CInGameSceneSC::CreateRock_Heavy()
{
	vector<int> vPosX, vPosZ;

	vPosX.push_back(19); vPosZ.push_back(89);
	vPosX.push_back(70); vPosZ.push_back(46);

	vPosX.push_back(19 + 50); vPosZ.push_back(89 + 50);
	vPosX.push_back(19 + 100); vPosZ.push_back(89 + 15);

	vPosX.push_back(113); vPosZ.push_back(34);

	for (int i = 0; i < vPosX.size(); ++i) {
		// �޽� ȸ��, ũ��, ��ġ
		CDx11GameObject* pCactus = CreateObj("Rock_Heavy",
			/*ȸ��*/	-DX11_PI / 2, 0, 0,
			/*ũ��*/	0.02, 0.02, 0.02,
			/*��ġ*/	0, -0.2, 0);


		//CDX11ColliderFrustrum* pColl = pCactus->AddComponent<CDX11ColliderFrustrum>(STATIC_MESH);
		//pColl->SetFrustrumScale(1, 1, 1);
		//pColl->SetInfo("Heavy", 1, 3.5, 1, 1, 3.5, 1);
		//SAFE_RELEASE(pColl);

		CDx11Transform* pTr = pCactus->GetTransform();

		CDx11ColliderOBB* pOBB = pCactus->AddComponent<CDx11ColliderOBB>(STATIC_MESH);
		float fAxisLen[3];
		fAxisLen[0] = 1;
		fAxisLen[1] = 3.5f;
		fAxisLen[2] = 1;
		pOBB->SetOBBInfo(fAxisLen, pTr);
		SAFE_RELEASE(pOBB);

		//��ü �ʱ���ġ
		float fRot = rand() % 180;
		pTr->SetWorldRotY(fRot);
		pTr->SetWorldPos(vPosX[i], 0, vPosZ[i]);

		// ������ �����.
		CDx11GameObject* pTerrainObj = m_pScene->FindGameObject("TerrainObj");
		CDx11Terrain* pTerrain = (CDx11Terrain*)pTerrainObj->FindComponentFromTag("TerrainCP");

		// ���� ��ġ������ ����� �븻�� ��ġ������ �����´�.
		TRIANGLE2RAY PosNormal = pTerrain->GetHeight(pCactus);

		// �� ������Ʈ�� �ű��.
		pTr->SetWorldPos(PosNormal.Pos);

		// �ø��� ���� �ݸ������Ǿ�
		CDx11ColliderSphere* pSphere = pCactus->AddComponent<CDx11ColliderSphere>(STATIC_MESH);
		pSphere->SetSphereInfo(0.5, PosNormal.Pos);
		SAFE_RELEASE(pSphere);

		// ������
		SAFE_RELEASE(pTerrainObj);
		SAFE_RELEASE(pTerrain);

		SAFE_RELEASE(pTr);
		SAFE_RELEASE(pCactus);
	}
}

void CInGameSceneSC::CreateRock_Large()
{
	vector<int> vPosX, vPosZ;

	vPosX.push_back(17); vPosZ.push_back(94);
	vPosX.push_back(17+50); vPosZ.push_back(94+50);
	vPosX.push_back(123); vPosZ.push_back(109);
	vPosX.push_back(107); vPosZ.push_back(31);
	vPosX.push_back(155); vPosZ.push_back(179);

	for (int i = 0; i < vPosX.size(); ++i) {
		// �޽� ȸ��, ũ��, ��ġ
		CDx11GameObject* pCactus = CreateObj("Rock_Large",
			/*ȸ��*/	-DX11_PI / 2, 0, 0,
			/*ũ��*/	0.02, 0.02, 0.02,
			/*��ġ*/	0, -0.2, 0);

		//CDX11ColliderFrustrum* pColl = pCactus->AddComponent<CDX11ColliderFrustrum>(STATIC_MESH);
		//pColl->SetFrustrumScale(1, 1, 1);
		//pColl->SetInfo("Large", 1.5, 9, 1.5, 1.5, 9, 1.5);
		//SAFE_RELEASE(pColl);

		CDx11Transform* pTr = pCactus->GetTransform();

		CDx11ColliderOBB* pOBB = pCactus->AddComponent<CDx11ColliderOBB>(STATIC_MESH);
		float fAxisLen[3];
		fAxisLen[0] = 1.8;
		fAxisLen[1] = 8.f;
		fAxisLen[2] = 1.8f;
		pOBB->SetOBBInfo(fAxisLen, pTr);
		SAFE_RELEASE(pOBB);

		//��ü �ʱ���ġ
		float fRot = rand() % 180;
		pTr->SetWorldRotY(fRot);
		pTr->SetWorldPos(vPosX[i], 0, vPosZ[i]);

		// ������ �����.
		CDx11GameObject* pTerrainObj = m_pScene->FindGameObject("TerrainObj");
		CDx11Terrain* pTerrain = (CDx11Terrain*)pTerrainObj->FindComponentFromTag("TerrainCP");

		// ���� ��ġ������ ����� �븻�� ��ġ������ �����´�.
		TRIANGLE2RAY PosNormal = pTerrain->GetHeight(pCactus);

		// �� ������Ʈ�� �ű��.
		pTr->SetWorldPos(PosNormal.Pos);

		// �ø��� ���� �ݸ������Ǿ�
		CDx11ColliderSphere* pSphere = pCactus->AddComponent<CDx11ColliderSphere>(STATIC_MESH);
		pSphere->SetSphereInfo(0.5, PosNormal.Pos);
		SAFE_RELEASE(pSphere);

		// ������
		SAFE_RELEASE(pTerrainObj);
		SAFE_RELEASE(pTerrain);

		SAFE_RELEASE(pTr);
		SAFE_RELEASE(pCactus);
	}
}

void CInGameSceneSC::CreateRock_Small()
{
	vector<int> vPosX, vPosZ;

	vPosX.push_back(72); vPosZ.push_back(48);
	vPosX.push_back(142); vPosZ.push_back(53);
	vPosX.push_back(53); vPosZ.push_back(177);
	vPosX.push_back(171); vPosZ.push_back(109);

	for (int i = 0; i < vPosX.size(); ++i) {
		// �޽� ȸ��, ũ��, ��ġ
		CDx11GameObject* pCactus = CreateObj("Rock_Small",
			/*ȸ��*/	-DX11_PI / 2, 0, 0,
			/*ũ��*/	0.02, 0.02, 0.02,
			/*��ġ*/	0, -0.2, 0);

		//CDX11ColliderFrustrum* pColl = pCactus->AddComponent<CDX11ColliderFrustrum>(STATIC_MESH);
		//pColl->SetFrustrumScale(1, 1, 1);
		//pColl->SetInfo("Small", 0.7, 0.5, 0.7, 0.7, 0.5, 0.7);
		//SAFE_RELEASE(pColl);

		CDx11Transform* pTr = pCactus->GetTransform();

		CDx11ColliderOBB* pOBB = pCactus->AddComponent<CDx11ColliderOBB>(STATIC_MESH);
		float fAxisLen[3];
		fAxisLen[0] = 0.7;
		fAxisLen[1] = 0.5;
		fAxisLen[2] = 0.7;
		pOBB->SetOBBInfo(fAxisLen, pTr);
		SAFE_RELEASE(pOBB);

		//��ü �ʱ���ġ
		float fRot = rand() % 180;
		pTr->SetWorldRotY(fRot);
		pTr->SetWorldPos(vPosX[i], 0, vPosZ[i]);

		// ������ �����.
		CDx11GameObject* pTerrainObj = m_pScene->FindGameObject("TerrainObj");
		CDx11Terrain* pTerrain = (CDx11Terrain*)pTerrainObj->FindComponentFromTag("TerrainCP");

		// ���� ��ġ������ ����� �븻�� ��ġ������ �����´�.
		TRIANGLE2RAY PosNormal = pTerrain->GetHeight(pCactus);

		// �� ������Ʈ�� �ű��.
		pTr->SetWorldPos(PosNormal.Pos);

		// �ø��� ���� �ݸ������Ǿ�
		CDx11ColliderSphere* pSphere = pCactus->AddComponent<CDx11ColliderSphere>(STATIC_MESH);
		pSphere->SetSphereInfo(0.5, PosNormal.Pos);
		SAFE_RELEASE(pSphere);

		// ������
		SAFE_RELEASE(pTerrainObj);
		SAFE_RELEASE(pTerrain);

		SAFE_RELEASE(pTr);
		SAFE_RELEASE(pCactus);
	}
}

void CInGameSceneSC::CreateTerrainTexs()
{
	vector<int> vPosX, vPosY, vPosZ;

	vPosX.push_back(66); vPosY.push_back(0); vPosZ.push_back(79);
	vPosX.push_back(68); vPosY.push_back(1000); vPosZ.push_back(78);
	vPosX.push_back(38); vPosY.push_back(500); vPosZ.push_back(103);
	vPosX.push_back(36); vPosY.push_back(0); vPosZ.push_back(103);
	vPosX.push_back(33); vPosY.push_back(0); vPosZ.push_back(103);
	vPosX.push_back(33); vPosY.push_back(0); vPosZ.push_back(106);
	vPosX.push_back(61); vPosY.push_back(0); vPosZ.push_back(43);
	vPosX.push_back(21); vPosY.push_back(0); vPosZ.push_back(69);
	vPosX.push_back(96); vPosY.push_back(500); vPosZ.push_back(56);
	vPosX.push_back(93); vPosY.push_back(0); vPosZ.push_back(56);
	vPosX.push_back(99); vPosY.push_back(0); vPosZ.push_back(54);

	vPosX.push_back(53); vPosY.push_back(0); vPosZ.push_back(159);
	vPosX.push_back(156); vPosY.push_back(0); vPosZ.push_back(173);
	vPosX.push_back(170); vPosY.push_back(0); vPosZ.push_back(107);
	vPosX.push_back(136); vPosY.push_back(10); vPosZ.push_back(67);
	vPosX.push_back(153); vPosY.push_back(1000); vPosZ.push_back(173);

	vPosX.push_back(74); vPosY.push_back(0); vPosZ.push_back(81);

	for (int i = 0; i < vPosX.size(); ++i) {
		char cName[32] = {};
		sprintf(cName, "TerrainTex%d", i);
		CreateTerrainTex(cName, Vec3(vPosX[i], 0, vPosZ[i]), vPosY[i]);
	}
}

void CInGameSceneSC::CreateTerrainTex(const char* pName, Vec3 p, int i)
{

	// ��ġ�� ���Ѵ�.
	Vec3 vPos = p;

	// ������ �����.
	CDx11GameObject* pTerrainObj = m_pScene->FindGameObject("TerrainObj");
	CDx11Terrain* pTerrain = (CDx11Terrain*)pTerrainObj->FindComponentFromTag("TerrainCP");

	Vec3 P[9];
	P[0] = pTerrain->GetTex(vPos, 0);
	P[1] = pTerrain->GetTex(vPos, 1);
	P[2] = pTerrain->GetTex(vPos, 2);
	P[3] = pTerrain->GetTex(vPos, 3);
	P[4] = pTerrain->GetTex(vPos, 4);
	P[5] = pTerrain->GetTex(vPos, 5);
	P[6] = pTerrain->GetTex(vPos, 6);
	P[7] = pTerrain->GetTex(vPos, 7);
	P[8] = pTerrain->GetTex(vPos, 8);
	
	for (int i = 0; i < 9; ++i)
		P[i].y += 0.005 + i*0.001;

	TEXVERTEX	tTexVtx[16] =
	{
		// 1
		{ P[0], Vec3( 0, 1, 0), Vec2(0.f, 0.f) },		//0
		{ P[1], Vec3( 0, 1, 0), Vec2(0.5, 0.f) },	//1
		{ P[4], Vec3( 0, 1, 0), Vec2(0.5, 0.5) },	//2
		{ P[3], Vec3( 0, 1, 0), Vec2(0.f, 0.5) },	//3

		// 2
		{ P[1], Vec3(0, 1, 0), Vec2(0.5, 0.f) },		//4
		{ P[2], Vec3(0, 1, 0), Vec2(1.0, 0.f) },		//5
		{ P[5], Vec3(0, 1, 0), Vec2(1.f, 0.5) },		//6
		{ P[4], Vec3(0, 1, 0), Vec2(0.5, 0.5) },	//7

		// 3
		{ P[3], Vec3(0, 1, 0), Vec2(0.f, 0.5) },  	//8
		{ P[4], Vec3(0, 1, 0), Vec2(0.5, 0.5) },	//9
		{ P[7], Vec3(0, 1, 0), Vec2(0.5, 1.f) },		//10
		{ P[6], Vec3(0, 1, 0), Vec2(0.f, 1.f) },  	//11

		// 4
		{ P[4], Vec3(0, 1, 0), Vec2(0.5, 0.5) },	//12
		{ P[5], Vec3(0, 1, 0), Vec2(1.f, 0.5) }, 	//13
		{ P[8], Vec3(0, 1, 0), Vec2(1.f, 1.f) },		//14
		{ P[7], Vec3(0, 1, 0), Vec2(0.5, 1.f) }, 	//15
	};

	UINT	iIndex[24] = {
		0, 1, 2,
		0, 2, 3,

		4, 5, 6,
		4, 6, 7,

		8, 9, 10,
		8, 10, 11,

		12,13,14,
		12,14,15
	};

	CDx11Mesh* pMesh = DX11_GET_SINGLE(CDx11ResourcesManager)->CreateMesh(pName, tTexVtx, 16, sizeof(TEXVERTEX), iIndex, 24, sizeof(UINT), DXGI_FORMAT_R32_UINT);
	SAFE_RELEASE(pMesh);

	// ���̾ ã�´�.
	CDx11Layer*	pLayer = m_pScene->FindLayer("DefaultLayer");

	if (!pLayer)
		return;

	// ������Ʈ�� �߰��Ѵ�.
	CDx11GameObject* pObj = CDx11GameObject::Create("pTexObj");
	pObj->SetCollision(false);

	// ���̾ �߰��Ѵ�.
	pLayer->AddObject(pObj);

	CDx11Renderer* pRenderer = pObj->AddComponent<CDx11Renderer>("pRenderer");
	pRenderer->SetMesh(pName);
	pRenderer->SetShader("TexShader");
	pRenderer->CreateMaterial("GrassTile", L"Terrain/GrassTile.PNG");
	pRenderer->SetRenderState(DX11_RS_ALPHABLEND, RSF_BLEND);
	 
	SAFE_RELEASE(pRenderer);
	SAFE_RELEASE(pLayer);
	SAFE_RELEASE(pObj);
	SAFE_RELEASE(pTerrainObj);
	SAFE_RELEASE(pTerrain);
}

CDx11GameObject* CInGameSceneSC::CreateObj(
	const char * cName, 
	float fRotX, float fRotY, float fRotZ,
	float fScalX, float fScalY, float fScalZ,
	float fPosX, float fPosY, float fPosZ)
{
	// ���̾ ã�´�.
	CDx11Layer*	pLayer = m_pScene->FindLayer("DefaultLayer");

	if (!pLayer)
		return NULL;

	// ������Ʈ�� �����.
	CDx11GameObject* Obj = CDx11GameObject::Create(cName);
	pLayer->AddObject(Obj);

	// ����Ʈ �������� �߰��Ѵ�.
	CDx11Renderer* ObjRenderer = Obj->AddComponent<CDx11Renderer>("ObjRenderer");
	ObjRenderer->SetMesh("CubeMesh");
	ObjRenderer->SetShader("DefaultShader");
	ObjRenderer->SetMaterialDiffuseTex("Default");

	// Ʈ������
	CDx11Transform* Tr = Obj->GetTransform();
	Tr->Scaling(0, 0, 0);

	// �޽� ������Ʈ�� �߰��Ѵ�.
	CDx11GameObject* Mesh = CDx11GameObject::Create("Mesh");
	Obj->AddChild(Mesh);

	// FBX �޽��� �߰��Ѵ�.
	CDx11Renderer* MeshRenderer = Mesh->AddComponent<CDx11Renderer>("MeshRenderer");
	MeshRenderer->SetMesh(cName);
	MeshRenderer->SetShader("TexShader");
	MeshRenderer->SetRenderState(DX11_RS_ALPHABLEND, RSF_BLEND);
	//MeshRenderer->SetRenderState(DX11_RS_NONECULL, RSF_RASTERIZER);

	// �ʱ� ũ��, ��ġ
	CDx11Transform* MeshTr = Mesh->GetTransform();
	MeshTr->SetWorldRotX(fRotX);
	MeshTr->SetWorldRotY(fRotY);
	MeshTr->SetWorldRotZ(fRotZ);
	MeshTr->Scaling(fScalX, fScalY, fScalZ);
	MeshTr->SetWorldPos(Vec3(fPosX, fPosY, fPosZ));


	SAFE_RELEASE(MeshTr);
	SAFE_RELEASE(MeshRenderer);
	SAFE_RELEASE(Mesh);

	SAFE_RELEASE(Tr);
	SAFE_RELEASE(ObjRenderer);

	SAFE_RELEASE(pLayer);

	return Obj;
}
