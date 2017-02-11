#include "Dx11Transform.h"
#include "../GameObject/Dx11GameObject.h"

DX11_USING

CDx11Transform::CDx11Transform()
{
	m_eComType = CT_TRANSFORM;
	SetTypeName<CDx11Transform>();
	m_bStatic = false;
	m_bUpdate = true;
	m_iParentFlag = TPF_ALL;
}

CDx11Transform::CDx11Transform(const CDx11Transform & transform)
{
	*this = transform;
	m_iRefCount = 1;
}

CDx11Transform::~CDx11Transform()
{
}

bool CDx11Transform::Init()
{
	// Local 초기화
	m_vLocalScale = Vec3One;
	m_vLocalRot = Vec3Zero;
	m_vLocalPos = Vec3Zero;
	m_vLocalMove = Vec3Zero;
	m_vLocalAxis[AXIS_X] = Vec3X;
	m_vLocalAxis[AXIS_Y] = Vec3Y;
	m_vLocalAxis[AXIS_Z] = Vec3Z;
	m_vLocalView = Vec3Y;

	m_matLocalScale = XMMatrixIdentity();
	m_matLocalRotX = XMMatrixIdentity();
	m_matLocalRotY = XMMatrixIdentity();
	m_matLocalRotZ = XMMatrixIdentity();
	m_matLocalRot = XMMatrixIdentity();
	m_matLocalTranslate = XMMatrixIdentity();
	m_matLocalParent = XMMatrixIdentity();
	m_matLocal = XMMatrixIdentity();

	// World 초기화
	m_vWorldScale = Vec3One;
	m_vWorldRot = Vec3Zero;
	m_vWorldPos = Vec3Zero;
	m_vWorldMove = Vec3Zero;
	m_vWorldRev = Vec3Zero;
	m_vWorldFinalScale = Vec3One;
	m_vWorldFinalPos = Vec3Zero;
	m_vWorldFinalRot = Vec3One;
	m_vWorldAxis[AXIS_X] = Vec3X;
	m_vWorldAxis[AXIS_Y] = Vec3Y;
	m_vWorldAxis[AXIS_Z] = Vec3Z;
	m_vWorldView = Vec3Y;

	m_matWorldScale = XMMatrixIdentity();
	m_matWorldRotX = XMMatrixIdentity();
	m_matWorldRotY = XMMatrixIdentity();
	m_matWorldRotZ = XMMatrixIdentity();
	m_matWorldRot = XMMatrixIdentity();
	m_matWorldTranslate = XMMatrixIdentity();
	m_matWorldRevX = XMMatrixIdentity();
	m_matWorldRevY = XMMatrixIdentity();
	m_matWorldRevZ = XMMatrixIdentity();
	m_matWorldRev = XMMatrixIdentity();
	m_matWorldParent = XMMatrixIdentity();
	m_matWorld = XMMatrixIdentity();

	return true;
}

int CDx11Transform::Update(float fTime)
{
	if (m_bStatic)
		return 0;

	return 0;
}

int CDx11Transform::LateUpdate(float fTime)
{
	if (m_bStatic)
		return 0;

	else if (!m_bUpdate)
		return 0;

	/*
	CDx11GameObject*	pParent = m_pGameObject->GetParent();

	if (pParent)
	{
	CDx11Transform*	pTransform = pParent->GetTransform();

	m_matLocalParent = pTransform->GetLocalMatrix();
	//m_matWorldParent = pTransform->GetWorldMatrix();
	m_matWorldParent = XMMatrixIdentity();

	// 부모의 회전행렬을 이용해서 위치를 회전시킨다.
	Matrix	mat;

	if (m_iParentFlag & TPF_SCALE)
	{
	m_vWorldFinalScale = m_vWorldScale * pTransform->GetWorldScale();
	m_matWorldParent = pTransform->GetWorldScaleMatrix();
	}

	if (m_iParentFlag & TPF_ROT)
	{
	m_matWorldParent *= pTransform->GetWorldRotMatrix();
	mat = pTransform->GetWorldRotMatrix();
	}

	if (m_iParentFlag & TPF_POS)
	{
	m_vWorldFinalPos = m_vWorldPos + pTransform->GetWorldPos();
	m_matWorldParent *= pTransform->GetWorldTranslateMatrix();
	mat *= pTransform->GetWorldTranslateMatrix();
	}

	// 부모의 회전과 위치를 이용해 행렬을 만들어서 WorldPosition에 곱해주어서
	// 회전된 위치를 구해준다.
	Vec vRotPos = XMVector3TransformCoord(m_vWorldPos.Convert(), mat);
	m_vWorldFinalPos.Convert(vRotPos);

	SAFE_RELEASE(pTransform);
	SAFE_RELEASE(pParent);
	}

	else
	{
	m_matLocalParent = XMMatrixIdentity();
	m_matWorldParent = XMMatrixIdentity();
	}
	*/

	m_matLocal = m_matLocalScale * m_matLocalRot * m_matLocalTranslate;

	m_matWorld = m_matWorldScale * m_matWorldRot * m_matWorldTranslate;

	//ComputeLocalAxis();
	//ComputeWorldAxis();

	return 0;
}

void CDx11Transform::Render(float fTime)
{
	m_bUpdate = false;
}

CDx11Transform * CDx11Transform::Clone()
{
	return new CDx11Transform(*this);
}

Vec3 CDx11Transform::GetLocalScale()
{
	return m_vLocalScale;
}

Vec3 CDx11Transform::GetLocalRot()
{
	return m_vLocalRot;
}

float CDx11Transform::GetLocalRotX()
{
	return m_vLocalRot.x;
}

float CDx11Transform::GetLocalRotY()
{
	return m_vLocalRot.y;
}

float CDx11Transform::GetLocalRotZ()
{
	return m_vLocalRot.z;
}

Vec3 CDx11Transform::GetLocalPos()
{
	return m_vLocalPos;
}

Vec3 CDx11Transform::GetLocalView()
{
	return m_vLocalView;
}

const Vec3 * CDx11Transform::GetLocalAxis()
{
	return m_vLocalAxis;
}

Vec3 CDx11Transform::GetLocalAxis(AXIS axis)
{
	return m_vLocalAxis[axis];
}

Matrix CDx11Transform::GetLocalMatrix()
{
	return m_matLocal;
}

Matrix CDx11Transform::GetLocalParent()
{
	return m_matLocalParent;
}

void CDx11Transform::SetLocalScale(const Vec3 & vScale)
{
	Vec3	vSc = vScale - m_vLocalScale;
	ScaleLocal(vSc);
}

void CDx11Transform::SetLocalScale(float x, float y, float z)
{
	Vec3	vScale(x, y, z);
	Vec3	vSc = vScale - m_vLocalScale;
	ScaleLocal(vSc);
}

void CDx11Transform::SetLocalRot(const Vec3 & vRot)
{
	Vec3	vRotate = vRot - m_vLocalRot;
	RotateLocal(vRotate);
}

void CDx11Transform::SetLocalRot(float x, float y, float z)
{
	Vec3	vRot(x, y, z);
	Vec3	vRotate = vRot - m_vLocalRot;
	RotateLocal(vRotate);
}

void CDx11Transform::SetLocalRotX(float x)
{
	Vec3	vRot(x, m_vLocalRot.y, m_vLocalRot.z);
	Vec3	vRotate = vRot - m_vLocalRot;
	RotateLocal(vRotate);
}

void CDx11Transform::SetLocalRotY(float y)
{
	Vec3	vRot(m_vLocalRot.x, y, m_vLocalRot.z);
	Vec3	vRotate = vRot - m_vLocalRot;
	RotateLocal(vRotate);
}

void CDx11Transform::SetLocalRotZ(float z)
{
	Vec3	vRot(m_vLocalRot.x, m_vLocalRot.y, z);
	Vec3	vRotate = vRot - m_vLocalRot;
	RotateLocal(vRotate);
}

void CDx11Transform::SetLocalPos(const Vec3 & vPos)
{
	Vec3	vMove = vPos - m_vLocalPos;
	MoveLocal(vMove);
}

void CDx11Transform::SetLocalPos(float x, float y, float z)
{
	Vec3	vPos(x, y, z);
	Vec3	vMove = vPos - m_vLocalPos;
	MoveLocal(vMove);
}

void CDx11Transform::SetLocalView(const Vec3 & vView)
{
	m_vLocalView = vView;
}

void CDx11Transform::SetLocalView(float x, float y, float z)
{
	m_vLocalView.x = x;
	m_vLocalView.y = y;
	m_vLocalView.z = z;
}

void CDx11Transform::ComputeLocalAxis()
{
	for (int i = 0; i < AXIS_MAX; ++i)
	{
		Vec	v = m_vLocalAxis[i].Convert();
		v = XMVector3TransformNormal(v, m_matLocalRot);
		m_vLocalAxis[i].Convert(v);
		m_vLocalAxis[i] = m_vLocalAxis[i].Normalize();
	}
}

void CDx11Transform::MoveLocal(float fSpeed, float fTime, AXIS axis)
{
	Vec3	vMove = m_vLocalAxis[axis] * fSpeed * fTime;
	MoveLocal(vMove);
}

void CDx11Transform::MoveLocal(const Vec3 & vMove)
{
	m_vLocalPos += vMove;
	m_vWorldFinalPos = m_vLocalPos + m_vWorldPos;

	ComputeRotationPos();

	/*if (m_iParentFlag & TPF_POS)
	{
	CDx11GameObject*	pParent = m_pGameObject->GetParent();

	if (pParent)
	{
	CDx11Transform*	pTr = pParent->GetTransform();

	m_vWorldFinalPos += pTr->GetWorldPos();

	SAFE_RELEASE(pTr);
	SAFE_RELEASE(pParent);
	}
	}*/

	m_matWorldTranslate = XMMatrixTranslation(m_vWorldFinalPos.x, m_vWorldFinalPos.y,
		m_vWorldFinalPos.z);

	list<CDx11GameObject*>*	pChildList = m_pGameObject->GetChildList();
	list<CDx11GameObject*>::iterator	iter;
	list<CDx11GameObject*>::iterator	iterEnd = pChildList->end();

	for (iter = pChildList->begin(); iter != iterEnd; ++iter)
	{
		CDx11Transform*	pTr = (*iter)->GetTransform();

		pTr->MoveLocal(Vec3Zero);

		SAFE_RELEASE(pTr);
	}

	EnableUpdate();
}

void CDx11Transform::ScaleLocal(const Vec3 & vScale)
{
	if (m_bStatic)
		return;

	m_vLocalScale += vScale;
	m_vWorldFinalScale = m_vLocalScale * m_vWorldScale;

	if (m_iParentFlag & TPF_SCALE)
	{
		CDx11GameObject*	pParent = m_pGameObject->GetParent();

		if (pParent)
		{
			CDx11Transform*	pTr = pParent->GetTransform();

			m_vWorldFinalScale *= pTr->GetWorldScale();

			SAFE_RELEASE(pTr);
			SAFE_RELEASE(pParent);
		}
	}

	m_matWorldScale = XMMatrixScaling(m_vWorldFinalScale.x, m_vWorldFinalScale.y,
		m_vWorldFinalScale.z);

	list<CDx11GameObject*>*	pChildList = m_pGameObject->GetChildList();
	list<CDx11GameObject*>::iterator	iter;
	list<CDx11GameObject*>::iterator	iterEnd = pChildList->end();

	for (iter = pChildList->begin(); iter != iterEnd; ++iter)
	{
		CDx11Transform*	pTr = (*iter)->GetTransform();

		pTr->ScaleLocal(Vec3Zero);

		SAFE_RELEASE(pTr);
	}

	EnableUpdate();
}

void CDx11Transform::RotateLocal(const Vec3 & vRot)
{
	m_vLocalRot += vRot;
	m_vWorldFinalRot = m_vLocalRot + m_vWorldRot;

	if (m_iParentFlag & TPF_ROT)
	{
		CDx11GameObject*	pParent = m_pGameObject->GetParent();

		if (pParent)
		{
			CDx11Transform*	pTr = pParent->GetTransform();

			m_vWorldFinalRot += pTr->GetWorldRot();

			SAFE_RELEASE(pTr);
			SAFE_RELEASE(pParent);
		}
	}

	m_matLocalRotX = XMMatrixRotationX(m_vLocalRot.x);
	m_matLocalRotY = XMMatrixRotationY(m_vLocalRot.y);
	m_matLocalRotZ = XMMatrixRotationZ(m_vLocalRot.z);
	m_matLocalRot = m_matLocalRotX * m_matLocalRotY * m_matLocalRotZ;

	m_matWorldRotX = XMMatrixRotationX(m_vWorldFinalRot.x);
	m_matWorldRotY = XMMatrixRotationY(m_vWorldFinalRot.y);
	m_matWorldRotZ = XMMatrixRotationZ(m_vWorldFinalRot.z);
	m_matWorldRot = m_matWorldRotX * m_matWorldRotY * m_matWorldRotZ;

	ComputeLocalAxis();
	ComputeWorldAxis();
	ComputeRotationPos();

	list<CDx11GameObject*>*	pChildList = m_pGameObject->GetChildList();
	list<CDx11GameObject*>::iterator	iter;
	list<CDx11GameObject*>::iterator	iterEnd = pChildList->end();

	for (iter = pChildList->begin(); iter != iterEnd; ++iter)
	{
		CDx11Transform*	pTr = (*iter)->GetTransform();

		pTr->RotateLocal(Vec3Zero);

		SAFE_RELEASE(pTr);
	}

	EnableUpdate();
}

Vec3 CDx11Transform::GetWorldScale()
{
	return m_vWorldFinalScale;
}

Vec3 CDx11Transform::GetWorldRot()
{
	return m_vWorldFinalRot;
}

float CDx11Transform::GetWorldRotX()
{
	return m_vWorldFinalRot.x;
}

float CDx11Transform::GetWorldRotY()
{
	return m_vWorldFinalRot.y;
}

float CDx11Transform::GetWorldRotZ()
{
	return m_vWorldFinalRot.z;
}

Vec3 CDx11Transform::GetWorldPos()
{
	return m_vWorldFinalPos;
}

Vec3 CDx11Transform::GetWorldView()
{
	return m_vWorldView;
}

const Vec3 * CDx11Transform::GetWorldAxis()
{
	return m_vWorldAxis;
}

Vec3 CDx11Transform::GetWorldAxis(AXIS axis)
{
	return m_vWorldAxis[axis];
}

Matrix CDx11Transform::GetWorldMatrix()
{
	return m_matWorld;
}

Matrix CDx11Transform::GetWorldParent()
{
	return m_matWorldParent;
}

Matrix CDx11Transform::GetWorldScaleMatrix()
{
	return m_matWorldScale;
}

Matrix CDx11Transform::GetWorldRotMatrix()
{
	return m_matWorldRot;
}

Matrix CDx11Transform::GetWorldTranslateMatrix()
{
	return m_matWorldTranslate;
}

void CDx11Transform::SetWorldScale(const Vec3 & vScale)
{
	Vec3	vSc = vScale - m_vWorldScale;
	ScaleWorld(vSc);
}

void CDx11Transform::SetWorldScale(float x, float y, float z)
{
	Vec3	vScale(x, y, z);
	Vec3	vSc = vScale - m_vWorldScale;
	ScaleWorld(vSc);
}

void CDx11Transform::SetWorldRot(const Vec3 & vRot)
{
	Vec3	vRotate = vRot - m_vWorldRot;
	RotateWorld(vRotate);
}

void CDx11Transform::SetWorldRot(float x, float y, float z)
{
	Vec3	vRot(x, y, z);
	Vec3	vRotate = vRot - m_vWorldRot;
	RotateWorld(vRotate);
}

void CDx11Transform::SetWorldRotX(float x)
{
	Vec3	vRot(x, m_vWorldRot.y, m_vWorldRot.z);
	Vec3	vRotate = vRot - m_vWorldRot;
	RotateWorld(vRotate);
}

void CDx11Transform::SetWorldRotY(float y)
{
	Vec3	vRot(m_vWorldRot.x, y, m_vWorldRot.z);
	Vec3	vRotate = vRot - m_vWorldRot;
	RotateWorld(vRotate);
}

void CDx11Transform::SetWorldRotZ(float z)
{
	Vec3	vRot(m_vWorldRot.x, m_vWorldRot.y, z);
	Vec3	vRotate = vRot - m_vWorldRot;
	RotateWorld(vRotate);
}

void CDx11Transform::SetWorldPos(const Vec3 & vPos)
{
	m_vWorldMove = vPos - m_vWorldPos;
	MoveWorld(m_vWorldMove);
}

void CDx11Transform::SetWorldPos(float x, float y, float z)
{
	Vec3	vPos(x, y, z);
	m_vWorldMove = vPos - m_vWorldPos;
	MoveWorld(m_vWorldMove);
}

void CDx11Transform::SetWorldView(const Vec3 & vView)
{
	m_vWorldView = vView;
}

void CDx11Transform::SetWorldView(float x, float y, float z)
{
	m_vWorldView.x = x;
	m_vWorldView.y = y;
	m_vWorldView.z = z;
}

void CDx11Transform::ComputeWorldAxis()
{
	Vec3	vAxis[AXIS_MAX] = { Vec3X, Vec3Y, Vec3Z };

	for (int i = 0; i < AXIS_MAX; ++i)
	{
		Vec	v = vAxis[i].Convert();
		v = XMVector3TransformNormal(v, m_matWorldRot);
		m_vWorldAxis[i].Convert(v);
		m_vWorldAxis[i] = m_vWorldAxis[i].Normalize();
	}
}

void CDx11Transform::MoveWorld(float fSpeed, float fTime, AXIS axis)
{
	m_vWorldMove = m_vWorldAxis[axis] * fSpeed * fTime;
	MoveWorld(m_vWorldMove);
}

void CDx11Transform::MoveWorld(float fSpeed, float fTime)
{
	m_vWorldMove = m_vWorldView * fSpeed * fTime;
	MoveWorld(m_vWorldMove);
}

void CDx11Transform::MoveWorld(const Vec3 & vMove)
{
	m_vWorldPos += vMove;
	m_vWorldFinalPos = m_vLocalPos + m_vWorldPos;

	ComputeRotationPos();

	/*if (m_iParentFlag & TPF_POS)
	{
	CDx11GameObject*	pParent = m_pGameObject->GetParent();

	if (pParent)
	{
	CDx11Transform*	pTr = pParent->GetTransform();

	m_vWorldFinalPos += pTr->GetWorldPos();

	SAFE_RELEASE(pTr);
	SAFE_RELEASE(pParent);
	}
	}*/

	m_matWorldTranslate = XMMatrixTranslation(m_vWorldFinalPos.x, m_vWorldFinalPos.y,
		m_vWorldFinalPos.z);

	list<CDx11GameObject*>*	pChildList = m_pGameObject->GetChildList();
	list<CDx11GameObject*>::iterator	iter;
	list<CDx11GameObject*>::iterator	iterEnd = pChildList->end();

	for (iter = pChildList->begin(); iter != iterEnd; ++iter)
	{
		CDx11Transform*	pTr = (*iter)->GetTransform();

		pTr->MoveWorld(Vec3Zero);

		SAFE_RELEASE(pTr);
	}

	EnableUpdate();
}

void CDx11Transform::RotationWorld(float fAngle, float fTime, AXIS axis)
{
	Vec3	vRot = Vec3Zero;

	switch (axis)
	{
	case AXIS_X:
		vRot.x = fAngle * fTime;
		break;
	case AXIS_Y:
		vRot.y = fAngle * fTime;
		break;
	case AXIS_Z:
		vRot.z = fAngle * fTime;
		break;
	}

	RotateWorld(vRot);
}

void CDx11Transform::RotationWorld(float fAngle, float fTime, Vec3 vec)
{
	Vec3	vRot = Vec3Zero;

	vRot.x = fAngle * fTime * vec.x;
	vRot.y = fAngle * fTime * vec.y;
	vRot.z = fAngle * fTime * vec.z;

	RotateWorld(vRot);
}

void CDx11Transform::ScaleWorld(const Vec3 & vScale)
{
	m_vWorldScale += vScale;
	m_vWorldFinalScale = m_vWorldScale * m_vLocalScale;

	if (m_iParentFlag & TPF_SCALE)
	{
		CDx11GameObject*	pParent = m_pGameObject->GetParent();

		if (pParent)
		{
			CDx11Transform*	pTransform = pParent->GetTransform();
			m_vWorldFinalScale *= pTransform->GetWorldScale();
			SAFE_RELEASE(pTransform);
			SAFE_RELEASE(pParent);
		}
	}

	m_matWorldScale = XMMatrixScaling(m_vWorldFinalScale.x, m_vWorldFinalScale.y, m_vWorldFinalScale.z);

	list<CDx11GameObject*>*	pChildList = m_pGameObject->GetChildList();
	list<CDx11GameObject*>::iterator	iter;
	list<CDx11GameObject*>::iterator	iterEnd = pChildList->end();

	for (iter = pChildList->begin(); iter != iterEnd; ++iter)
	{
		CDx11Transform*	pTr = (*iter)->GetTransform();

		pTr->ScaleWorld(Vec3Zero);

		SAFE_RELEASE(pTr);
	}

	EnableUpdate();
}

void CDx11Transform::RotateWorld(const Vec3 & vRot)
{
	m_vWorldRot += vRot;
	m_vWorldFinalRot = m_vLocalRot + m_vWorldRot;

	if (m_iParentFlag & TPF_ROT)
	{
		CDx11GameObject*	pParent = m_pGameObject->GetParent();

		if (pParent)
		{
			CDx11Transform*	pTr = pParent->GetTransform();

			m_vWorldFinalRot += pTr->GetWorldRot();

			SAFE_RELEASE(pTr);
			SAFE_RELEASE(pParent);
		}
	}

	m_matWorldRotX = XMMatrixRotationX(m_vWorldFinalRot.x);
	m_matWorldRotY = XMMatrixRotationY(m_vWorldFinalRot.y);
	m_matWorldRotZ = XMMatrixRotationZ(m_vWorldFinalRot.z);
	m_matWorldRot = m_matWorldRotX * m_matWorldRotY * m_matWorldRotZ;

	ComputeWorldAxis();
	ComputeRotationPos();

	list<CDx11GameObject*>*	pChildList = m_pGameObject->GetChildList();
	list<CDx11GameObject*>::iterator	iter;
	list<CDx11GameObject*>::iterator	iterEnd = pChildList->end();

	for (iter = pChildList->begin(); iter != iterEnd; ++iter)
	{
		CDx11Transform*	pTr = (*iter)->GetTransform();

		pTr->RotateWorld(Vec3Zero);

		SAFE_RELEASE(pTr);
	}

	EnableUpdate();
}

void CDx11Transform::ComputeRotationPos()
{
	CDx11GameObject*	pParent = m_pGameObject->GetParent();

	if (pParent)
	{
		CDx11Transform*	pTr = pParent->GetTransform();

		Vec3	vDist = m_vLocalPos + m_vWorldPos;
		Matrix	matParent = pTr->GetWorldMatrix();
		Vec	v = XMVector3TransformCoord(vDist.Convert(), matParent);
		m_vWorldFinalPos.Convert(v);

		m_matWorldTranslate = XMMatrixTranslation(m_vWorldFinalPos.x,
			m_vWorldFinalPos.y, m_vWorldFinalPos.z);

		SAFE_RELEASE(pTr);
		SAFE_RELEASE(pParent);
	}
}

void CDx11Transform::Static(bool bStatic)
{
	m_bStatic = bStatic;
}

void CDx11Transform::ClearParentFlag()
{
	m_iParentFlag = TPF_NONE;
}

void CDx11Transform::SetParentFlag(int iFlag)
{
	m_iParentFlag = iFlag;
}

void CDx11Transform::AddParentFlag(int iFlag)
{
	m_iParentFlag |= iFlag;
}

void CDx11Transform::DeleteParentFlag(int iFlag)
{
	m_iParentFlag ^= iFlag;
}

void CDx11Transform::EnableUpdate()
{
	m_bUpdate = true;

	//m_pGameObject->UpdateChildTransform();
}

bool CDx11Transform::GetStatic()
{
	return m_bStatic;
}

void CDx11Transform::Copy(CDx11Transform * pTransform)
{
	CDx11Scene*	pScene = m_pScene;
	CDx11Layer*	pLayer = m_pLayer;
	CDx11GameObject*	pObj = m_pGameObject;

	*this = *pTransform;

	m_vWorldScale = m_vWorldFinalScale;
	m_vWorldPos = m_vWorldFinalPos;

	m_pTransform = this;
	m_pGameObject = pObj;
	m_pLayer = pLayer;
	m_pScene = pScene;
}
