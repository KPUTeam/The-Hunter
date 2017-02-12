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

	// ===============  Get Func ================== 
	Vec3 GetWorldRot();
	Vec3 GetWorldPos();

	XMVECTOR GetWorldRotVec();
	XMVECTOR GetWorldPosVec();

	const Vec3* GetWorldAxis();
	Vec3 GetWorldAxis(AXIS axis);

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
	void SetWorldRotX(float x);
	void SetWorldRotY(float y);
	void SetWorldRotZ(float z);
	void SetWorldRot(const Vec3& vRot);
	void SetWorldRot(float x, float y, float z);
	void RotateWorld(const Vec3& vRot);
	void RotateWorld(float fAngle, float fTime, AXIS axis);
	void RotateWorld(float fAngle, float fTime, XMFLOAT3 axis);

	void SetWorldPos(const XMVECTOR& T);
	void SetWorldPos(const Vec3& vPos);
	void SetWorldPos(float x, float y, float z);
	void MoveWorld(const Vec3& vPos);
	void MoveWorld(float fSpeed, float fTime, AXIS axis);

};

DX11_END