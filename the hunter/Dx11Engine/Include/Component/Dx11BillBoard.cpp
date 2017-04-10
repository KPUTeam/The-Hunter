#include "Dx11BillBoard.h"
#include "Dx11Transform.h"
#include "Dx11Camera.h"
#include "../GameObject/Dx11GameObject.h"
#include "../Scene/Dx11Scene.h"
#include "Dx11UIBar.h"

DX11_USING

CDx11BillBoard::CDx11BillBoard()
{
	m_eComType = CT_BILLBOARD;
	SetTypeName<CDx11BillBoard>();
	
}


CDx11BillBoard::CDx11BillBoard(const CDx11BillBoard & board)	:
	CDx11Component(board)
{
	m_vPivot = board.m_vPivot;
}

CDx11BillBoard::~CDx11BillBoard()
{
}

void CDx11BillBoard::SetPivot(const Vec3 & vPivot)
{
	m_vPivot = vPivot;
}

bool CDx11BillBoard::Init()
{
	m_pTransform->SetRotationFlag(RF_BILLBOARD);
	m_vPivot = Vec3Zero;
	return true;
}

int CDx11BillBoard::Update(float fTime)
{
	return 0;
}

int CDx11BillBoard::LateUpdate(float fTime)
{
	// ���� ī�޶� ���´�.
	CDx11GameObject*	pCameraObj = m_pScene->GetMainCamera();
	CDx11Camera*		pCamera = (CDx11Camera*)pCameraObj->FindComponentFromType(CT_CAMERA);

	// ī�޶��� �� ����� ���´�.
	// ī�޶��� ����� Translate�� 0���� ���� 
	// ���ϴ� ��ġ(�Ǻ�)���� ����
	Matrix matView = pCamera->GetViewMatrix();

	float* pView = (float*)&matView;
	memset(pView + 12, 0, sizeof(Vec3));
	memcpy(pView + 12, &m_vPivot, sizeof(Vec3));

	XMVECTOR v = XMMatrixDeterminant(matView);
	matView = XMMatrixInverse(&v, matView);

	// ������ ������Ʈ�� Ʈ��������
	// ������ ������Ʈ�� �����ִ� ���� ������Ʈ�� Ʈ�������� ����.
	m_pTransform->SetWorldRotMatrix(matView);
	

	SAFE_RELEASE(pCamera);
	SAFE_RELEASE(pCameraObj);
	return 0;
}

void CDx11BillBoard::Render(float fTime)
{
}

CDx11BillBoard * CDx11BillBoard::Clone()
{
	return new CDx11BillBoard(*this);
}
