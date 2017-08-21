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
	// ���� ���� ���ٸ� �����Ѵ�.
	if (!m_pScene)
		exit(1);

	// ���� ī�޶� ã�´�.
	CDx11GameObject* pCameraObj = m_pScene->FindCamera("MainCamera");

	// ī�޶��� Ʈ�������� ã�´�.
	CDx11Transform* pCameraObjTr = pCameraObj->GetTransform();

	// �ٵ� ������Ʈ�� ��ġ�� �����Ѵ�.
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
	// ���� ���� ���ٸ� �����Ѵ�.
	if (!m_pScene)
		return 0;

	// ���� ī�޶� ã�´�.
	CDx11GameObject* pCameraObj = m_pScene->FindCamera("MainCamera");

	// �ٵ� ������Ʈ�� ��ġ�� �����Ѵ�.
	CDx11GameObject* pBodyObj = m_pGameObject->FindChildFromTag("pBodyObj");

	// ī�޶� ������Ʈ�� ã�´�.
	CDx11Camera* pCamera = (CDx11Camera*)pCameraObj->FindComponentFromTypeName("CDx11Camera");
	pCamera->SetTarget(pBodyObj);

	SAFE_RELEASE(pCamera);
	SAFE_RELEASE(pBodyObj);
	SAFE_RELEASE(pCameraObj);

	// ī �޶� �� �Ʒ��� �����Ѵ�.
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

	// ���� ī�޶� ã�´�.
	CDx11GameObject* pCameraObj = m_pScene->FindCamera("MainCamera");

	// ī�޶� ������Ʈ�� ã�´�.
	CDx11Camera* pCamera = (CDx11Camera*)pCameraObj->FindComponentFromTypeName("CDx11Camera");

	// ī�޶� �� �Ʒ� ����
	CDx11Mouse*	pMouse = DX11_GET_SINGLE(CDx11Input)->GetMouse();
	Vec2 vMouseGap = pMouse->GetMouseGap();

	// �ӵ� 0.004f
	vMouseGap *= 0.0004f;

	//// �� �� ȸ��
	////if (vMouseGap.y != 0.f) {
	//	// ���콺 �� �Ʒ� gap�� ī�޶� �����Ѵ�.
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

	// �� �� ȸ��
	if (vMouseGap.x != 0.f)
		pCamera->RotateLeftRight(vMouseGap.x);
	TraceCameraViewLeftRight(fTime);

	// Ŀ�� �߾�
	pMouse->SetMousePositionToCenter();

	SAFE_RELEASE(pCamera);
	SAFE_RELEASE(pMouse);
	SAFE_RELEASE(pCameraObj);
}

void CTankCameraSC::TraceCameraViewLeftRight(float fTime)
{
	// ī�޶� ���⿡ ���� ȸ���Ѵ�.
	CDx11GameObject*	pCameraObj = m_pScene->GetCamera();
	CDx11Transform*		pCamTr = pCameraObj->GetTransform();

	// ��� ������Ʈ�� ã�´�.
	CDx11GameObject* pBodyObj = m_pGameObject->FindChildFromTag("pBodyObj");
	CDx11GameObject* pHeadObj = pBodyObj->FindChildFromTag("pHeadObj");
	CDx11Transform*	pHeadObjTr = pHeadObj->GetTransform();

	// ī�޶� Z��, y�� ����.
	Vec3 vCameraAxisZ = pCamTr->GetWorldAxis(AXIS_X);	vCameraAxisZ.y = 0;
	vCameraAxisZ *= -1;

	// ����� Z��, y�� ����.
	Vec3 vHeadAxisZ = pHeadObjTr->GetWorldAxis(AXIS_Z);	vHeadAxisZ.y = 0;

	// ȸ������ ���Ѵ�.
	float fAngle = vCameraAxisZ.Dot(vHeadAxisZ);

	// ��带 Y�� �������� fAngle ��ŭ ȸ���Ѵ�.
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

