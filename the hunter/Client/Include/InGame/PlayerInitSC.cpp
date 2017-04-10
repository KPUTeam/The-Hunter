#include "PlayerInitSC.h"
#include "GameObject/Dx11GameObject.h"
#include "PlayerMeshSC.h"
#include "PlayerCameraSC.h"
#include "Component/Dx11Transform.h"

CPlayerInitSC::CPlayerInitSC()
{
}


CPlayerInitSC::~CPlayerInitSC()
{
}

bool CPlayerInitSC::Init()
{
	// 플레이어 객체에 매쉬와 랜더러를 추가한다.
	CPlayerMeshSC*	pMeshSC = m_pGameObject->AddComponent<CPlayerMeshSC>("PlayerMeshSC");
	SAFE_RELEASE(pMeshSC);

	// 플레이어 객체에 카메라를 추가한다.
	CPlayerCameraSC* pCameraSC = m_pGameObject->AddComponent<CPlayerCameraSC>("PlayerCameraSC");
	SAFE_RELEASE(pCameraSC);

	// 초기 크기를 설정한다.
	CDx11Transform* pTr = m_pGameObject->GetTransform();
	pTr->Scaling(0.05, 0.05, 0.05);
	SAFE_RELEASE(pTr);

	return true;
}

void CPlayerInitSC::Input(float fTime)
{
}

int CPlayerInitSC::Update(float fTime)
{
	return 0;
}

int CPlayerInitSC::LateUpdate(float fTime)
{
	return 0;
}

void CPlayerInitSC::Render(float fTime)
{

}

CPlayerInitSC * CPlayerInitSC::Clone()
{
	return new CPlayerInitSC(*this);
}

void CPlayerInitSC::OnCollisionEnter(CDx11Collider * pColl, float fTime)
{
}

void CPlayerInitSC::OnCollision(CDx11Collider * pColl, float fTime)
{
}

void CPlayerInitSC::OnCollisionExit(CDx11Collider * pColl, float fTime)
{
}
