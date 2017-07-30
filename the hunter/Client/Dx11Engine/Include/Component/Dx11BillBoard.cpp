#include "Dx11BillBoard.h"
#include "Dx11Transform.h"
#include "Dx11Camera.h"
#include "../GameObject/Dx11GameObject.h"
#include "../Scene/Dx11Scene.h"

DX11_USING

CDx11BillBoard::CDx11BillBoard()
{
	m_eComType = CT_BILLBOARD;
	SetTypeName<CDx11BillBoard>();
	m_vecPivot = VEC3ZERO;
}

CDx11BillBoard::CDx11BillBoard(const CDx11BillBoard & bill)	:
	CDx11Component(bill)
{
}

CDx11BillBoard::~CDx11BillBoard()
{
}

void CDx11BillBoard::SetPivot(const Vec3&  vecPivot)
{
	m_vecPivot = vecPivot;
}

bool CDx11BillBoard::Init()
{
	m_pTransform->SetWorldRotY(DX11_PI);

	return true;
}

int CDx11BillBoard::Update(float fTime)
{
	CDx11GameObject*	pCameraObj = m_pScene->GetCamera();
	CDx11Camera*		pCamera = (CDx11Camera*)pCameraObj->FindComponentFromType(CT_CAMERA);

	// ī�޶��� �� ����� ���´�.
	// ī�޶��� ����� Translate�� 0���� ���� 
	// ���ϴ� ��ġ(�Ǻ�)���� ����
	Matrix matView = pCamera->GetViewMatrix();

	float* pView = (float*)&matView;
	memset(pView + 12, 0, sizeof(Vec3));
	memcpy(pView + 12, &m_vecPivot, sizeof(Vec3));

	XMVECTOR v = XMMatrixDeterminant(matView);
	matView = XMMatrixInverse(&v, matView);

	// ������ ������Ʈ�� Ʈ��������
	// ������ ������Ʈ�� �����ִ� ���� ������Ʈ�� Ʈ�������� ����.
	m_pTransform->SetWorldRotMatrix(matView);


	SAFE_RELEASE(pCamera);
	SAFE_RELEASE(pCameraObj);
	return 0;
}

int CDx11BillBoard::LateUpdate(float fTime)
{
	return 0;
}

void CDx11BillBoard::Render(float fTime)
{
}

CDx11BillBoard * CDx11BillBoard::Clone()
{
	return new CDx11BillBoard(*this);
}
