#include "TankUI.h"
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
#include "Core/Dx11Input.h"
#include "TankCameraSC.h"


CTankUISC::CTankUISC()
{
}

CTankUISC::CTankUISC(const CTankUISC & script)
{
}

CTankUISC::~CTankUISC()
{
}

bool CTankUISC::Init()
{

	//// 탱크 프레임 바 생성
	//CDx11GameObject*  pFrameBar = CDx11GameObject::Create("TankFrameBar");
	//m_pGameObject->AddChild(pFrameBar);

	//CDx11Renderer*	pRenderer = pFrameBar->AddComponent<CDx11Renderer>("TankFrameBarRenderer");
	//pRenderer->CreateMaterial("TankFrameBar", L"Health/HBarFrame.png");
	//pRenderer->SetMesh(EFFECT_LT_MESH);
	//pRenderer->SetShader(UISHADER);
	//pRenderer->SetRenderState(DX11_RS_ALPHABLEND, RSF_BLEND);
	//SAFE_RELEASE(pRenderer);

	//// 초기 크기를 설정한다.
	//CDx11Transform*	pTransform = pFrameBar->GetTransform();
	//pTransform->Scaling(3.f, 0.6f, 1.f);
	//pTransform->MoveWorld(Vec3(0, 0, 0));
	//SAFE_RELEASE(pTransform);

	//// 빌보드 컴포넌트를 추가한다.
	//CDx11BillBoard*	pBillBoard = pFrameBar->AddComponent<CDx11BillBoard>("TankFrameBarBillBoard");
	//SAFE_RELEASE(pBillBoard);

	/***********************************************/
	//// 플레이어 HP Bar 생성
	//m_pHPBar = CDx11GameObject::Create("MonsterHPBar");
	//m_pGameObject->AddChild(m_pHPBar);

	//pRenderer = m_pHPBar->AddComponent<CDx11Renderer>("MonsterHPBarRenderer");
	//pRenderer->CreateMaterial("HPBar2", L"Health/HBarBlue.png");
	//pRenderer->SetMesh(EFFECT_LT_MESH);
	//pRenderer->SetShader(UISHADER);
	//pRenderer->SetRenderState(DX11_RS_ALPHABLEND, RSF_BLEND);
	//pRenderer->SetRenderState(DX11_RS_DEPTH_DISABLE, RSF_DEPTHSTENCIL);
	//SAFE_RELEASE(pRenderer);

	//// 초기 크기를 설정한다.
	//pTransform = m_pHPBar->GetTransform();
	//pTransform->Scaling(3., 0.6, 1.f);
	//pTransform->MoveWorld(Vec3(0, 0, 0));
	//SAFE_RELEASE(pTransform);

	//// 빌보드를 추가한다.
	//pBillBoard = m_pHPBar->AddComponent<CDx11BillBoard>("MonsterHPBarBillBoard");
	//SAFE_RELEASE(pBillBoard);

	return true;
}

int CTankUISC::Update(float fTime)
{
	return 0;
}

int CTankUISC::LateUpdate(float fTime)
{
	return 0;
}

void CTankUISC::Render(float fTime)
{
}

CTankUISC * CTankUISC::Clone()
{
	return new CTankUISC(*this);
}