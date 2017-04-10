#include "Dx11Transform.h"
#include "../GameObject/Dx11GameObject.h"

DX11_USING

CDx11Transform::CDx11Transform() :
	m_iParentFlag(0)
{
	m_eComType = CT_TRANSFORM;
	SetTypeName<CDx11Transform>();
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
	m_iParentFlag = TPF_ALL;

	m_vAxis[AXIS_X] = Vec3X;
	m_vAxis[AXIS_Y] = Vec3Y;
	m_vAxis[AXIS_Z] = Vec3Z;

	m_vView = { 0,1,0 };

	m_T = DirectX::XMVectorSet(0.0, 0.0, 0.0, 1.0);
	m_Q = DirectX::XMQuaternionRotationRollPitchYaw(0.0, 0.0, 0.0);
	m_S = DirectX::XMVectorSet(1.0, 1.0, 1.0, 1.0);

	m_eRotFlag = RF_XYZ;

	return true;
}

int CDx11Transform::Update(float fTime)
{
	return 0;
}

int CDx11Transform::LateUpdate(float fTime)
{
	return 0;
}

void CDx11Transform::Render(float fTime)
{
}

CDx11Transform * CDx11Transform::Clone()
{
	return new CDx11Transform(*this);
}

void CDx11Transform::Copy(CDx11Transform * pTransform)
{
	m_T = pTransform->GetWorldPosVec();
	m_Q = pTransform->GetWorldRotVec();
	m_vAxis[AXIS_X] = pTransform->GetWorldAxis(AXIS_X);
	m_vAxis[AXIS_Y] = pTransform->GetWorldAxis(AXIS_Y);
	m_vAxis[AXIS_Z] = pTransform->GetWorldAxis(AXIS_Z);

	m_vView = pTransform->m_vView;
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

int CDx11Transform::GetFlag()
{
	return m_iParentFlag;
}

void CDx11Transform::SetRotationFlag(ROTATION_FLAG eFlag)
{
	m_eRotFlag = eFlag;
}

void CDx11Transform::SetWorldAxis(const Vec3 & vAxis, AXIS axis)
{
	m_vAxis[axis] = vAxis;
}

void CDx11Transform::SetWorldRotMatrix(const Matrix& m)
{
	m_Q = XMQuaternionRotationMatrix(m);
	// m_Q = XMQuaternionMultiply(m_Q, Q);
}


void CDx11Transform::ComputeAxis()
{
	Vec3 vAxis[AXIS_MAX] = { Vec3X, Vec3Y, Vec3Z };
	Matrix M = XMMatrixRotationQuaternion(m_Q);

	for (int i = 0; i < AXIS_MAX; ++i)
	{
		Vec	v = vAxis[i].Convert();
		v = XMVector3TransformNormal(v, M);
		m_vAxis[i].Convert(v);
		m_vAxis[i] = m_vAxis[i].Normalize();
	}
}

void CDx11Transform::ChildMove(const Vec3& pos)
{
	list<CDx11GameObject*>* pChildList = m_pGameObject->GetChildList();
	list<CDx11GameObject*>::iterator iter;
	list<CDx11GameObject*>::iterator iterEnd = pChildList->end();

	for (iter = pChildList->begin(); iter != iterEnd; ++iter)
	{
		CDx11Transform* pTr = (*iter)->GetTransform();

		if (pTr->GetFlag() & TPF_POS)
			pTr->MoveWorld(pos);

		SAFE_RELEASE(pTr);
	}
}

void CDx11Transform::ChildRot(const Vec3& rot)
{
	list<CDx11GameObject*>* pChildList = m_pGameObject->GetChildList();
	list<CDx11GameObject*>::iterator iter;
	list<CDx11GameObject*>::iterator iterEnd = pChildList->end();

	// 각도
	FLOAT rx = XMConvertToRadians(rot.x);
	FLOAT ry = XMConvertToRadians(rot.y);
	FLOAT rz = XMConvertToRadians(rot.z);
	XMVECTOR V = XMVectorSet(rx, ry, rz, FLOAT(1.0));

	XMVECTOR N = XMVector3Normalize(V);
	XMVECTOR L = XMVector3Length(V);

	// 기준점, 부모의 위치
	XMFLOAT3 point = GetWorldPos();

	for (iter = pChildList->begin(); iter != iterEnd; ++iter)
	{
		CDx11Transform* pTr = (*iter)->GetTransform();

		// ================================== 자전  ====================================
		if (pTr->GetFlag() & TPF_ROT)
			pTr->RotateWorld(rot);

		// ================================== 공전 ====================================
		XMVECTOR T = XMVectorSet(point.x, point.y, point.z, 0);
		XMVECTOR ChildT = pTr->GetWorldPosVec();
		ChildT = ChildT - T;

		XMVECTOR QV = XMQuaternionRotationNormal(N, XMVectorGetX(L));
		XMMATRIX QM = XMMatrixRotationQuaternion(QV);
		ChildT = XMVector3Transform(ChildT, QM);

		ChildT = ChildT + T;
		pTr->SetWorldPos(ChildT);

		SAFE_RELEASE(pTr);
	}
}

Vec3 CDx11Transform::GetWorldRot()
{
	Vec3 vRot = { XMVectorGetX(m_Q), XMVectorGetY(m_Q), XMVectorGetZ(m_Q) };
	return vRot;
}

XMVECTOR CDx11Transform::GetWorldRotVec()
{
	return m_Q;
}


Vec3 CDx11Transform::GetWorldPos()
{
	Vec3 vPos = { XMVectorGetX(m_T), XMVectorGetY(m_T), XMVectorGetZ(m_T) };
	return vPos;
}

XMVECTOR CDx11Transform::GetWorldPosVec()
{
	return m_T;
}

Vec3 CDx11Transform::GetWorldScale()
{
	Vec3 vSc = { XMVectorGetX(m_S), XMVectorGetY(m_S), XMVectorGetZ(m_S) };
	return vSc;

}
XMVECTOR CDx11Transform::GetWorldScaleVec()
{
	return m_S;
}

const Vec3 * CDx11Transform::GetWorldAxis()
{
	return m_vAxis;
}

Vec3 CDx11Transform::GetWorldAxis(AXIS axis)
{
	return m_vAxis[axis];
}

Matrix CDx11Transform::GetWorldMatrix()
{
	XMMATRIX tM = XMMatrixTranslationFromVector(m_T);
	XMMATRIX rM = XMMatrixRotationQuaternion(m_Q);
	XMMATRIX sM = XMMatrixScalingFromVector(m_S);

	return sM * rM * tM;
}

// ================================== Local ====================================
void CDx11Transform::Scaling(const Vec3 & vScale)
{
	m_S = XMVectorSet(vScale.x, vScale.y, vScale.z, 1.f);
}

void CDx11Transform::Scaling(float x, float y, float z)
{
	m_S = XMVectorSet(x, y, z, 1.f);
}

void CDx11Transform::SetLocalRotX(float x)
{
	Vec3	vRot(
		x - XMVectorGetX(m_Q),
		0,
		0);

	RotateLocal(vRot);
}
void CDx11Transform::SetLocalRotY(float y)
{
	Vec3	vRot(
		0,
		y - XMVectorGetY(m_Q),
		0);

	RotateLocal(vRot);
}

void CDx11Transform::SetLocalRotZ(float z)
{
	Vec3	vRot(
		0,
		0,
		z - XMVectorGetZ(m_Q));

	RotateLocal(vRot);
}

void CDx11Transform::SetLocalRot(const Vec3& r)
{
	Vec3	vRot(
		r.x - XMVectorGetX(m_Q),
		r.y - XMVectorGetY(m_Q),
		r.z - XMVectorGetZ(m_Q));

	RotateLocal(vRot);
}

void CDx11Transform::SetLocalRot(float x, float y, float z)
{
	Vec3	vRot(
		x - XMVectorGetX(m_Q),
		y - XMVectorGetY(m_Q),
		z - XMVectorGetZ(m_Q));

	RotateLocal(vRot);
}

void CDx11Transform::RotateLocal(float fAngle, float fTime, AXIS axis)
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

	RotateLocal(vRot);
}

void CDx11Transform::RotateLocal(const Vec3& vRot)
{
	XMMATRIX M;
	XMVECTOR V;

	FLOAT rx = XMConvertToRadians(vRot.x);
	FLOAT ry = XMConvertToRadians(vRot.y);
	FLOAT rz = XMConvertToRadians(vRot.z);

	M = XMMatrixRotationQuaternion(m_Q);
	V = XMVector3Transform(XMVectorSet(rx, ry, rz, 1.f), M);

	XMVECTOR N = XMVector3Normalize(V);
	XMVECTOR L = XMVector3Length(V);

	if (XMVector3NotEqual(N, XMVectorZero()) == true)
	{
		XMVECTOR Q = XMQuaternionRotationNormal(N, XMVectorGetX(L));
		m_Q = XMQuaternionMultiply(m_Q, Q);
		ComputeAxis();
		ChildRot(vRot);
	}
}

void CDx11Transform::SetLocalPos(const Vec3& p)
{
	Vec3	vPos(
		p.x - XMVectorGetX(m_T),
		p.y - XMVectorGetY(m_T),
		p.z - XMVectorGetZ(m_T));

	MoveLocal(vPos);
}

void CDx11Transform::SetLocalPos(float x, float y, float z)
{
	Vec3	vPos(
		x - XMVectorGetX(m_T),
		y - XMVectorGetY(m_T),
		z - XMVectorGetZ(m_T));

	MoveLocal(vPos);
}

void CDx11Transform::MoveLocal(float fSpeed, float fTime, AXIS axis)
{
	Vec3 vMove = m_vAxis[axis] * fSpeed * fTime;
	MoveLocal(vMove);
}

void CDx11Transform::SetWorldView(const Vec3 & v)
{
	m_vView = v;
}

void CDx11Transform::SetWorldView(float x, float y, float z)
{
	m_vView.x = x;
	m_vView.y = y;
	m_vView.z = z;
}


void CDx11Transform::MoveLocal(const Vec3& vPos)
{
	//XMMATRIX M;
	//XMVECTOR V;

	//M = XMMatrixRotationQuaternion(m_Q);
	//V = XMVector3Transform(XMVectorSet(vPos.x, vPos.y, vPos.z, 1.f), M);
	//m_T = XMVectorAdd(m_T, V);

	XMVECTOR V = XMVectorSet(vPos.x, vPos.y, vPos.z, 0.0);
	m_T += V;

	ChildMove(vPos);
}


// ================================== World ====================================
void CDx11Transform::SetWorldRotX(float x)
{
	Vec3	vRot(
		x - XMVectorGetX(m_Q),
		0,
		0);

	RotateWorld(vRot);
}

void CDx11Transform::SetWorldRotY(float y)
{
	Vec3	vRot(
		0,
		y - XMVectorGetY(m_Q),
		0);

	RotateWorld(vRot);
}
void CDx11Transform::SetWorldRotZ(float z)
{
	Vec3	vRot(
		0,
		0,
		z - XMVectorGetZ(m_Q));

	RotateWorld(vRot);
}
void CDx11Transform::SetWorldRot(const Vec3& r)
{
	Vec3	vRot(
		r.x - XMVectorGetX(m_Q),
		r.y - XMVectorGetY(m_Q),
		r.z - XMVectorGetZ(m_Q));

	RotateWorld(vRot);
}

void CDx11Transform::SetWorldRot(float x, float y, float z)
{
	Vec3	vRot(
		x - XMVectorGetX(m_Q),
		y - XMVectorGetY(m_Q),
		z - XMVectorGetZ(m_Q));

	RotateWorld(vRot);
}

void CDx11Transform::RotateWorld(const XMVECTOR & vec)
{
	m_Q = vec;

	ComputeAxis();
}

void CDx11Transform::RotateWorld(float fAngle, float fTime, AXIS axis)
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

void CDx11Transform::RotateWorld(const Vec3& vRot)
{
	XMVECTOR V;

	//FLOAT rx = XMConvertToRadians(vRot.x);
	//FLOAT ry = XMConvertToRadians(vRot.y);
	//FLOAT rz = XMConvertToRadians(vRot.z);

	FLOAT rx = vRot.x;
	FLOAT ry = vRot.y;
	FLOAT rz = vRot.z;


	V = XMVectorSet(rx, ry, rz, FLOAT(1.0));


	XMVECTOR N = XMVector3Normalize(V);
	XMVECTOR L = XMVector3Length(V);

	if (XMVector3NotEqual(N, XMVectorZero()) == true)
	{
		XMVECTOR Q = XMQuaternionRotationNormal(N, XMVectorGetX(L));
		m_Q = XMQuaternionMultiply(m_Q, Q);
		ComputeAxis();
		ChildRot(vRot);
	}
}

void CDx11Transform::RotateWorld(float fAngle, float fTime, XMFLOAT3 axis)
{
	Vec3	vRot(
		fAngle*fTime*axis.x,
		fAngle*fTime*axis.y,
		fAngle*fTime*axis.z);

	RotateWorld(vRot);
}

void CDx11Transform::SetWorldPos(const XMVECTOR & T)
{
	m_T = T;
}

void CDx11Transform::SetWorldPos(const Vec3& p)
{
	Vec3	vPos(
		p.x - XMVectorGetX(m_T),
		p.y - XMVectorGetY(m_T),
		p.z - XMVectorGetZ(m_T));

	MoveWorld(vPos);

}

void CDx11Transform::SetWorldPos(float x, float y, float z)
{
	Vec3	vPos(
		x - XMVectorGetX(m_T),
		y - XMVectorGetY(m_T),
		z - XMVectorGetZ(m_T));

	MoveWorld(vPos);
}

void CDx11Transform::MoveWorld(float fSpeed, float fTime, AXIS axis)
{
	Vec3 vMove = m_vAxis[axis] * fSpeed * fTime;
	MoveWorld(vMove);
}


void CDx11Transform::MoveWorld(const Vec3& vPos)
{
	XMVECTOR V = XMVectorSet(vPos.x, vPos.y, vPos.z, 0.0);
	m_T += V;

	ChildMove(vPos);
}

void CDx11Transform::MoveWorld(float fSpeed, float fTime, const Vec3 & v)
{
	Vec3	vMove = v * fSpeed * fTime;
	MoveWorld(vMove);
}

void CDx11Transform::MoveWorld(float fSpeed, float fTime)
{
	Vec3 vMove = m_vView * fSpeed * fTime;
	MoveWorld(vMove);
}
