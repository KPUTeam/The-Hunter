#pragma once
#include "Dx11Component.h"

DX11_BEGIN

class DX11_DLL CDx11Transform :
	public CDx11Component
{
private:
	friend class CDx11GameObject;

private:
	CDx11Transform();
	CDx11Transform(const CDx11Transform& transform);
	~CDx11Transform();

public:
	virtual bool Init();
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Render(float fTime);
	virtual CDx11Transform* Clone();

public:
	void Copy(CDx11Transform* pTransform);

private:
	XMVECTOR m_T, m_Q, m_S;
	Vec3 m_vAxis[AXIS_MAX];
	Vec3 m_vView;

	ROTATION_FLAG	m_eRotFlag;

private:
	int m_iParentFlag;


private:
	// ===============  Compute ================== 
	void ComputeAxis();
	void ChildMove(const Vec3& pos);
	void ChildRot(const Vec3& rot);

public:
	// ===============  Flag ================== 
	void SetParentFlag(int iFlag);
	void AddParentFlag(int iFlag);
	void DeleteParentFlag(int iFlag);
	int GetFlag();

	void SetRotationFlag(ROTATION_FLAG eFlag);

	// ?
	void SetWorldAxis(const Vec3 & vAxis, AXIS axis);
	void SetWorldRotMatrix(const Matrix& m);

	// ===============  Get Func ================== 
	Vec3 GetWorldRot();
	Vec3 GetWorldPos();
	Vec3 GetWorldScale();

	XMVECTOR GetWorldRotVec();
	XMVECTOR GetWorldPosVec();
	XMVECTOR GetWorldScaleVec();

	const Vec3* GetWorldAxis();
	Vec3 GetWorldAxis(AXIS axis);

	Matrix GetWorldRotMatrix();
	Matrix GetWorldMatrix();

	// ================================== Local ====================================
	void Scaling(const Vec3& vScale);
	void Scaling(float x, float y, float z);

	void SetLocalRotX(float x);
	void SetLocalRotY(float y);
	void SetLocalRotZ(float z);
	void SetLocalRot(const Vec3& vRot);
	void SetLocalRot(float x, float y, float z);
	void RotateLocal(const Vec3& vRot);
	void RotateLocal(float fAngle, float fTime, AXIS axis);

	void SetLocalPos(const Vec3& vPos);
	void SetLocalPos(float x, float y, float z);
	void MoveLocal(const Vec3& vPos);
	void MoveLocal(float fSpeed, float fTime, AXIS axis);

	// ================================== World ====================================
	void SetWorldView(const Vec3&  v);
	void SetWorldView(float x, float y, float z);

	void SetWorldRotX(float x);
	void SetWorldRotY(float y);
	void SetWorldRotZ(float z);
	void SetWorldRot(const Vec3& vRot);
	void SetWorldRot(float x, float y, float z);
	void RotateWorld(const XMVECTOR& vec);
	void RotateWorld(const Vec3& vRot);
	void RotateWorld(float fAngle, float fTime, AXIS axis);
	void RotateWorld(float fAngle, float fTime, const Vec3 & axis);

	void SetWorldPos(const XMVECTOR& T);
	void SetWorldPos(const Vec3& P);
	void SetWorldPos(float fTime, const Vec3& P);
	void SetWorldPos(float x, float y, float z);

	void MoveWorld(const Vec3& vPos);
	void MoveWorld(float fSpeed, float fTime);
	void MoveWorld(float fSpeed, float fTime, AXIS axis);
	void MoveWorld(float fSpeed, float fTime, const Vec3& v);

};

DX11_END

//#pragma once
//#include "Dx11Component.h"
//
//DX11_BEGIN
//
//class DX11_DLL CDx11Transform :
//	public CDx11Component
//{
//private:
//	friend class CDx11GameObject;
//	static Vec3	m_vOriginAxis[AXIS_MAX];
//
//private:
//	CDx11Transform();
//	CDx11Transform(const CDx11Transform& transform);
//	~CDx11Transform();
//
//private:	// Local
//	Vec3	m_vLocalPos;
//	Vec3	m_vLocalScale;
//	Vec3	m_vLocalRot;	// °¡¾Çµµ
//	Quaternion	m_qLocalRot;
//	Vec3	m_vLocalAxis[AXIS_MAX];
//
//	Matrix	m_matLocalScale;
//	Matrix	m_matLocalRot;
//	Matrix	m_matLocalTrans;
//	Matrix	m_matLocalMatrix;
//
//private:	// World
//	Vec3	m_vWorldPos;
//	Vec3	m_vWorldPrevPos;
//	Vec3	m_vWorldFinalPos;
//	Vec3	m_vWorldScale;
//	Vec3	m_vWorldRot;
//	Quaternion	m_qWorldRot;
//	Vec3	m_vWorldAxis[AXIS_MAX];
//	Vec3	m_vRotAxis;
//	Vec3	m_vLookAt;
//	AXIS	m_eMoveAxis;
//
//	Matrix	m_matWorldScale;
//	Matrix	m_matWorldRot;
//	Matrix	m_matWorldTrans;
//	Matrix	m_matWorldParent;
//	Matrix	m_matWorldMatrix;
//	Matrix	m_matWorldRT;
//
//private:
//	bool	m_bStatic;
//	bool	m_bUpdate;
//	bool	m_bAxis;
//	bool	m_bParentRot;
//
//public:
//	void Static(bool bStatic);
//	void EnableUpdate();
//	bool GetStatic();
//	bool GetUpdate();
//	void SetMoveAxis(AXIS eAxis);
//	void DisableParentRot();
//
//public:
//	virtual bool Init();
//	virtual int Update(float fTime);
//	virtual int LateUpdate(float fTime);
//	virtual void Render(float fTime);
//	virtual CDx11Transform* Clone();
//
//public:
//	void UpdateMatrix();
//
//public:
//	void SetLocalScale(const Vec3& vScale);
//	void SetLocalScale(float x, float y, float z);
//	void SetLocalPosition(const Vec3& vPos);
//	void SetLocalPosition(float x, float y, float z);
//	void SetLocalRotation(float x, float y, float z);
//	void SetLocalRotationX(float x);
//	void SetLocalRotationY(float y);
//	void SetLocalRotationZ(float z);
//
//public:
//	void ComputeLocalAxis();
//
//public:
//	void Scaling(const Vec3& vScale);
//	void SetWorldPos(const Vec3& vPos);
//	void SetWorldPos(float x, float y, float z);
//	void Scaling(float x, float y, float z);
//	void RotationWorld(float fAngle, float fTime, const Vec3 & vRot);
//	void SetWorldRotation(float x, float y, float z);
//	void SetWorldRotationX(float x);
//	void SetWorldRotationY(float y);
//	void SetWorldRotationZ(float z);
//
//	void SetRotAxis(const Vec3& vAxis);
//	void SetLookAt(const Vec3& vLookAt);
//	void SetWorldRotationMatrix(const Matrix& matRot);
//	void SetWorldAxis(const Vec3& vAxis, AXIS axis);
//
//public:
//	Vec3 GetLocalScale()	const;
//	Vec3 GetLocalRot()	const;
//	float GetLocalRot(AXIS axis)	const;
//	Vec3 GetLocalPos()	const;
//	const Vec3* GetLocalAxis()	const;
//	Vec3 GetLocalAxis(AXIS axis)	const;
//
//	Vec3 GetWorldScale()	const;
//	Vec3 GetWorldRot()	const;
//	float GetWorldRot(AXIS axis)	const;
//	Vec3 GetWorldPos()	const;
//	Vec3 GetWorldPrevPos()	const;
//	Vec3 GetWorldPos();
//	const Vec3* GetWorldAxis()	const;
//	Vec3 GetWorldAxis(AXIS axis)	const;
//
//public:
//	Matrix GetLocalMatrix()	const;
//	Matrix GetLocalScaleMatrix()	const;
//	Matrix GetLocalRotMatrix()	const;
//	Matrix GetLocalTranslateMatrix()	const;
//	Matrix GetWorldMatrix()	const;
//	Matrix GetWorldScaleMatrix()	const;
//	Matrix GetWorldRotMatrix()	const;
//	Matrix GetWorldTranslateMatrix()	const;
//	Matrix GetParentMatrix()	const;
//	Matrix GetWorldRT()	const;
//
//public:
//	void Move(const Vec3& vDir, float fSpeed, float fTime);
//	void Move(float fSpeed, float fTime);
//	void Move(float fSpeed, float fTime, AXIS axis);
//	void Move(const Vec3& vMove);
//	void RotateWorldX(float fSpeed, float fTime);
//	void RotateWorldY(float fSpeed, float fTime);
//	void RotateWorldZ(float fSpeed, float fTime);
//	void Copy(const CDx11Transform& transform);
//	void Copy(CDx11Transform* transform);
//	void ComputeWorldAxis();
//	void ComputeWorldMatrix();
//	void ComputeWorld();
//	void ComputeWorldPos();
//};
//
//DX11_END
