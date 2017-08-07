#include "UserScript.h"
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


CUserScript::CUserScript() :
	m_pSession(NULL)
{
}

CUserScript::CUserScript(const CUserScript & user) :
	CDx11Script(user)
{
	m_iSerialNumber = user.m_iSerialNumber;
	m_pSession = NULL;
}


CUserScript::~CUserScript()
{
}

void CUserScript::SetSession(CUserSession * pSession)
{
	m_pSession = pSession;
}

void CUserScript::SetSerialNumber(INT64 iSerialNumber)
{
	m_iSerialNumber = iSerialNumber;
}

void CUserScript::CreateBar()
{
	m_pFrameBar = CDx11GameObject::Create("FrameBar");

	CDx11Renderer* pRenderer = m_pFrameBar->AddComponent<CDx11Renderer>("FrameBarRenderer");
	SAFE_RELEASE(pRenderer);

	// 초기 크기를 설정한다.
	CDx11Transform*	pTransform = m_pFrameBar->GetTransform();
	pTransform->Scaling(3.5f, 0.6f, 1.f);
	SAFE_RELEASE(pTransform);

	// 빌보드 컴포넌트를 추가한다.
	CDx11BillBoard*	pBillBoard = m_pFrameBar->AddComponent<CDx11BillBoard>("FrameBarBillBoard");
	SAFE_RELEASE(pBillBoard);

	// 플레이어 HP Bar 생성
	m_pHPBar = CDx11GameObject::Create("HPBar");
	SAFE_RELEASE(pRenderer);

	// 초기 크기를 설정한다.
	pTransform = m_pHPBar->GetTransform();
	pTransform->Scaling(3.5f, 0.6f, 1.f);
	SAFE_RELEASE(pTransform);

	// 빌보드를 추가한다.
	pBillBoard = m_pHPBar->AddComponent<CDx11BillBoard>("HPBarBillBoard");
	SAFE_RELEASE(pBillBoard);
}

void CUserScript::SetBarPos(float fTime)
{
	// HP의 위치를 적용한다.
	CDx11GameObject* pMainObj = CDx11GameObject::Create("pMainObj");
	CDx11Transform* pTr = pMainObj->GetTransform();
	Vec3 vMainObjPos = pTr->GetWorldPos();
	SAFE_RELEASE(pTr);

	// 프레임
	CDx11GameObject* pBar = pMainObj->FindChildFromTag("FrameBar");
	pTr = pBar->GetTransform();

	pTr->SetWorldPos(
		vMainObjPos.x - 1.75,
		vMainObjPos.y + 4 + 0.03,
		vMainObjPos.z);

	SAFE_RELEASE(pTr);
	SAFE_RELEASE(pBar);

	pBar = pMainObj->FindChildFromTag("HPBar");
	pTr = pBar->GetTransform();

	pTr->SetWorldPos(
		vMainObjPos.x - 1.75,
		vMainObjPos.y + 4,
		vMainObjPos.z);

	pTr->Scaling(3.5f*m_fHP, 0.6f, 1.f);

	SAFE_RELEASE(pTr);
	SAFE_RELEASE(pBar);
}

bool CUserScript::init()
{
	m_iSerialNumber = 0;

	return true;
}

void CUserScript::Input(float fTime)
{
}

int CUserScript::Update(float fTime)
{
	return 0;
}

int CUserScript::LateUpdate(float fTime)
{
	return 0;
}

void CUserScript::Render(float fTime)
{
}

CUserScript * CUserScript::Clone()
{
	return new CUserScript(*this);
}
