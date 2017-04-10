#include "PlayerCameraSC.h"
#include "GameObject/Dx11GameObject.h"
#include "Component/Dx11Camera.h"
#include "Component/Dx11Transform.h"
#include "Scene/Dx11Scene.h"


CPlayerCameraSC::CPlayerCameraSC()
{
}


CPlayerCameraSC::~CPlayerCameraSC()
{
}

bool CPlayerCameraSC::Init()
{
	// 플레이어 카메라를 생성한다.
	CDx11GameObject*	pCameraObj = m_pScene->CreateCamera("PlayerCamera", DX11_PI / 3.f, 0.1f, 1000.f);
	CDx11Transform*		pCameraTr = pCameraObj->GetTransform();
	CDx11Camera*		pCam = (CDx11Camera*)pCameraObj->FindComponentFromTypeName("CDx11Camera");

	pCam->SetTarget(m_pGameObject);
	pCam->SetCameraType(CVT_THIRD);
	m_pScene->ChangeCamera("PlayerCamera");
	SAFE_RELEASE(pCam);
	SAFE_RELEASE(pCameraTr);
	SAFE_RELEASE(pCameraObj);

	return true;
}

void CPlayerCameraSC::Input(float fTime)
{
}

int CPlayerCameraSC::Update(float fTime)
{
	return 0;
}

int CPlayerCameraSC::LateUpdate(float fTime)
{
	return 0;
}

void CPlayerCameraSC::Render(float fTime)
{
}

CPlayerCameraSC * CPlayerCameraSC::Clone()
{
	return new CPlayerCameraSC(*this);
}

void CPlayerCameraSC::OnCollisionEnter(CDx11Collider * pColl, float fTime)
{
}

void CPlayerCameraSC::OnCollision(CDx11Collider * pColl, float fTime)
{
}

void CPlayerCameraSC::OnCollisionExit(CDx11Collider * pColl, float fTime)
{
}
