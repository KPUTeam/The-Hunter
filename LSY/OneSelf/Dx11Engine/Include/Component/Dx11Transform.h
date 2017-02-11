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

private:	// Local
	Vec3		m_vLocalScale;
	Vec3		m_vLocalRot;
	Vec3		m_vLocalPos;
	Vec3		m_vLocalMove;
	Vec3		m_vLocalAxis[AXIS_MAX];
	Vec3		m_vLocalView;

	Matrix		m_matLocalScale;
	Matrix		m_matLocalRotX;
	Matrix		m_matLocalRotY;
	Matrix		m_matLocalRotZ;
	Matrix		m_matLocalRot;
	Matrix		m_matLocalTranslate;
	Matrix		m_matLocalParent;
	Matrix		m_matLocal;

public:
	Vec3 GetLocalScale();
	Vec3 GetLocalRot();
	float GetLocalRotX();
	float GetLocalRotY();
	float GetLocalRotZ();
	Vec3 GetLocalPos();
	Vec3 GetLocalView();
	const Vec3* GetLocalAxis();
	Vec3 GetLocalAxis(AXIS axis);
	Matrix GetLocalMatrix();
	Matrix GetLocalParent();

public:
	void SetLocalScale(const Vec3& vScale);
	void SetLocalScale(float x, float y, float z);
	void SetLocalRot(const Vec3& vRot);
	void SetLocalRot(float x, float y, float z);
	void SetLocalRotX(float x);
	void SetLocalRotY(float y);
	void SetLocalRotZ(float z);
	void SetLocalPos(const Vec3& vPos);
	void SetLocalPos(float x, float y, float z);
	void SetLocalView(const Vec3& vView);
	void SetLocalView(float x, float y, float z);
	void ComputeLocalAxis();
	void MoveLocal(float fSpeed, float fTime, AXIS axis);
	void MoveLocal(const Vec3& vMove);
	void ScaleLocal(const Vec3& vScale);
	void RotateLocal(const Vec3& vRot);

private:	// World
	Vec3		m_vWorldScale;
	Vec3		m_vWorldFinalScale;
	Vec3		m_vWorldRot;
	Vec3		m_vWorldFinalRot;
	Vec3		m_vWorldPos;
	Vec3		m_vWorldFinalPos;
	Vec3		m_vWorldRev;
	Vec3		m_vWorldMove;
	Vec3		m_vWorldAxis[AXIS_MAX];
	Vec3		m_vWorldView;

	Matrix		m_matWorldScale;
	Matrix		m_matWorldRotX;
	Matrix		m_matWorldRotY;
	Matrix		m_matWorldRotZ;
	Matrix		m_matWorldRot;
	Matrix		m_matWorldTranslate;
	Matrix		m_matWorldRevX;
	Matrix		m_matWorldRevY;
	Matrix		m_matWorldRevZ;
	Matrix		m_matWorldRev;
	Matrix		m_matWorldParent;
	Matrix		m_matWorld;

public:
	Vec3 GetWorldScale();
	Vec3 GetWorldRot();
	float GetWorldRotX();
	float GetWorldRotY();
	float GetWorldRotZ();
	Vec3 GetWorldPos();
	Vec3 GetWorldView();
	const Vec3* GetWorldAxis();
	Vec3 GetWorldAxis(AXIS axis);
	Matrix GetWorldMatrix();
	Matrix GetWorldParent();
	Matrix GetWorldScaleMatrix();
	Matrix GetWorldRotMatrix();
	Matrix GetWorldTranslateMatrix();

public:
	void SetWorldScale(const Vec3& vScale);
	void SetWorldScale(float x, float y, float z);
	void SetWorldRot(const Vec3& vRot);
	void SetWorldRot(float x, float y, float z);
	void SetWorldRotX(float x);
	void SetWorldRotY(float y);
	void SetWorldRotZ(float z);
	void SetWorldPos(const Vec3& vPos);
	void SetWorldPos(float x, float y, float z);
	void SetWorldView(const Vec3& vView);
	void SetWorldView(float x, float y, float z);
	void ComputeWorldAxis();
	void MoveWorld(float fSpeed, float fTime, AXIS axis);
	void MoveWorld(float fSpeed, float fTime);
	void MoveWorld(const Vec3& vMove);
	void RotationWorld(float fAngle, float fTime, AXIS axis);
	void RotationWorld(float fAngle, float fTime, Vec3 vec);
	void ScaleWorld(const Vec3& vScale);
	void RotateWorld(const Vec3& vRot);
	void ComputeRotationPos();

private:
	bool		m_bStatic;
	bool		m_bUpdate;
	int			m_iParentFlag;

public:
	void Static(bool bStatic);
	void ClearParentFlag();
	void SetParentFlag(int iFlag);
	void AddParentFlag(int iFlag);
	void DeleteParentFlag(int iFlag);
	void EnableUpdate();

public:
	bool GetStatic();

public:
	void Copy(CDx11Transform* pTransform);
};

DX11_END