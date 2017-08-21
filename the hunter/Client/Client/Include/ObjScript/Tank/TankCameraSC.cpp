#include "TankCameraSC.h"
#include "Resources/Dx11Mesh.h"
#include "Resources/Dx11ResourcesManager.h"
#include "GameObject/Dx11GameObject.h"
#include "Component/Dx11Renderer.h"
#include "Component/Dx11Transform.h"
#include "Scene/Dx11Scene.h"
#include "Scene/Dx11layer.h"
#include "Core/Dx11Input.h"
#include "Component/Dx11Camera.h"
#include "Core/Dx11TimerManager.h"
#include "Core/Dx11Timer.h"
#include "Component/Dx11Mouse.h"
#include "Core/Dx11Input.h"

#include "../../Network/NetManager.h"
#include "../../Network/ZEROStream.h"

CTankCameraSC::CTankCameraSC()
{
}

CTankCameraSC::CTankCameraSC(const CTankCameraSC & script)
{
}

CTankCameraSC::~CTankCameraSC()
{
}

bool CTankCameraSC::Init()
{
	// 현재 씬이 없다면 리턴한다.
	if (!m_pScene)
		exit(1);

	// 메인 카메라를 찾는다.
	CDx11GameObject* pCameraObj = m_pScene->FindCamera("MainCamera");

	// 카메라의 트랜스폼을 찾는다.
	CDx11Transform* pCameraObjTr = pCameraObj->GetTransform();

	// 바디 오브젝트의 위치를 저장한다.
	CDx11GameObject* pBodyObj = m_pGameObject->FindChildFromTag("pBodyObj");
	CDx11Transform* pBodyObjTr = pBodyObj->GetTransform();
	Vec3 vBodyObjPos = pBodyObjTr->GetWorldPos();
	pCameraObjTr->SetWorldPos(
		vBodyObjPos.x + CAMERA_INITIAL_POSX,
		vBodyObjPos.y + CAMERA_INITIAL_POSY,
		vBodyObjPos.z + CAMERA_INITIAL_POSZ);

	SAFE_RELEASE(pBodyObj);
	SAFE_RELEASE(pBodyObjTr);

	SAFE_RELEASE(pCameraObj);
	SAFE_RELEASE(pCameraObjTr);

	return true;
}

int CTankCameraSC::Update(float fTime)
{
	// 현재 씬이 없다면 리턴한다.
	if (!m_pScene)
		return 0;

	// 메인 카메라를 찾는다.
	CDx11GameObject* pCameraObj = m_pScene->FindCamera("MainCamera");

	// 바디 오브젝트의 위치를 저장한다.
	CDx11GameObject* pBodyObj = m_pGameObject->FindChildFromTag("pBodyObj");

	// 카메라 컴포넌트를 찾는다.
	CDx11Camera* pCamera = (CDx11Camera*)pCameraObj->FindComponentFromTypeName("CDx11Camera");
	pCamera->SetTarget(pBodyObj);

	SAFE_RELEASE(pCamera);
	SAFE_RELEASE(pBodyObj);
	SAFE_RELEASE(pCameraObj);

	// 카 메라 위 아래를 제한한다.
	LimitUpDownCameraAngle(fTime);


	return 0;
} 

int CTankCameraSC::LateUpdate(float fTime)
{
	return 0;
}

void CTankCameraSC::Render(float fTime)
{
}

CTankCameraSC * CTankCameraSC::Clone()
{
	return new CTankCameraSC(*this);
}

void CTankCameraSC::LimitUpDownCameraAngle(float fTime)
{

	if (!DX11_GET_SINGLE(CDx11Input)->GetInput())
		return;

	// 메인 카메라를 찾는다.
	CDx11GameObject* pCameraObj = m_pScene->FindCamera("MainCamera");

	// 카메라 컴포넌트를 찾는다.
	CDx11Camera* pCamera = (CDx11Camera*)pCameraObj->FindComponentFromTypeName("CDx11Camera");

	// 카메라 위 아래 조절
	CDx11Mouse*	pMouse = DX11_GET_SINGLE(CDx11Input)->GetMouse();
	Vec2 vMouseGap = pMouse->GetMouseGap();

	// 속도 0.004f
	vMouseGap *= 0.0004f;

	//// 상 하 회전
	////if (vMouseGap.y != 0.f) {
	//	// 마우스 위 아래 gap을 카메라 저장한다.
	//	CameraUpDownAmounts += vMouseGap.y;

	//	if (CameraUpDownAmounts < -CAMERA_UPDOWN_LIMITE) {
	//		CameraUpDownAmounts = -CAMERA_UPDOWN_LIMITE;
	//	}

	//	else if (CameraUpDownAmounts  > CAMERA_UPDOWN_LIMITE) {
	//		CameraUpDownAmounts = CAMERA_UPDOWN_LIMITE;
	//	}

	//	else {
	//		pCamera->RotateUpDown(vMouseGap.y);
	//		TraceCameraViewUpDown(vMouseGap.y);
	//	}
	//}

	if (vMouseGap.y != 0.f) {
		pCamera->RotateUpDown(vMouseGap.y);
	}
	TraceCameraViewUpDown(fTime);

	// 좌 우 회전
	if (vMouseGap.x != 0.f)
		pCamera->RotateLeftRight(vMouseGap.x);
	TraceCameraViewLeftRight(fTime);

	// 커서 중앙
	pMouse->SetMousePositionToCenter();

	SAFE_RELEASE(pCamera);
	SAFE_RELEASE(pMouse);
	SAFE_RELEASE(pCameraObj);
}

void CTankCameraSC::TraceCameraViewLeftRight(float fTime)
{
	// 카메라 방향에 따라 회전한다.
	CDx11GameObject*	pCameraObj = m_pScene->GetCamera();
	CDx11Transform*		pCamTr = pCameraObj->GetTransform();

	// 헤드 오브젝트를 찾는다.
	CDx11GameObject* pBodyObj = m_pGameObject->FindChildFromTag("pBodyObj");
	CDx11GameObject* pHeadObj = pBodyObj->FindChildFromTag("pHeadObj");
	CDx11Transform*	pHeadObjTr = pHeadObj->GetTransform();

	// 카메라 Z축, y를 뺀다.
	Vec3 vCameraAxisZ = pCamTr->GetWorldAxis(AXIS_X);	vCameraAxisZ.y = 0;
	vCameraAxisZ *= -1;

	// 헤드의 Z축, y를 뺀다.
	Vec3 vHeadAxisZ = pHeadObjTr->GetWorldAxis(AXIS_Z);	vHeadAxisZ.y = 0;

	// 회전량을 구한다.
	float fAngle = vCameraAxisZ.Dot(vHeadAxisZ);

	// 헤드를 Y축 기준으로 fAngle 만큼 회전한다.
	Vec3 vHeadAxisY = pHeadObjTr->GetWorldAxis(AXIS_Y);
	pHeadObjTr->RotateWorld(fAngle, 1, vHeadAxisY);

	Vec3 vMyHeadAxisZ = pHeadObjTr->GetWorldAxis(AXIS_X);
	vMyHeadAxisZ *= -1;
	CNetManager::GetInst()->myHeadRotate(vMyHeadAxisZ);

	SAFE_RELEASE(pCameraObj);
	SAFE_RELEASE(pCamTr);
	SAFE_RELEASE(pBodyObj);
	SAFE_RELEASE(pHeadObj);
	SAFE_RELEASE(pHeadObjTr);
}

void CTankCameraSC::TraceCameraViewUpDown(float fGapY)
{
}

