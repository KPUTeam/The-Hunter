#pragma once
#include "Component\Dx11Script.h"

DX11_USING

class CUser :
	public CDx11Script
{
public:
	CUser();
	CUser(const CUser& user);
	~CUser();

private:
	INT64		m_iSerialNumber;
	list<CUser*> m_UserObj;

public:
	INT64 GetSerialNumber() const
	{
		return m_iSerialNumber;
	}

	void SetSerialNumber(INT64 iSerialNumber)
	{
		m_iSerialNumber = iSerialNumber;
	}

	void AddUserObj(CUser* pObj);
	CUser* FindUserObj(INT64 iSerialNumber);

private:
	const float SCALE = 0.025;
	const float HEIGHT_ALPHA = 0.55;
	const float WHEEEL_ROTATION_SPEED = 0.2;
	class CDx11GameObject* pBodyObj;
	class CDx11GameObject* pLT;
	class CDx11GameObject* pRT;
	class CDx11GameObject* pLB;
	class CDx11GameObject* pRB;
	class CDx11GameObject* pHeadObj;

	float				m_fHP;
	CDx11GameObject*	m_pHPBar;
	CDx11GameObject*	m_pFrameBar;

private:
	float m_fSpeed = 0.02;
	float m_vDir = 0;
	float m_vAcc = 0;

	float m_fGlobalTime;

	bool LeftTurnTrigger = false;
	bool RightTurnTrigger = false;
	bool moveupdate = false;

private:
	bool CreateBody();
	bool CreateLTWheel();
	bool CreateRTWheel();
	bool CreateLBWheel();
	bool CreateRBWheel();
	bool CreateHead();

public:
	void RotateWheel(float fSpeed);
	void SetHeight(float fTime);

	void CreateBar();
	void SetBarPos();

public:
	virtual bool Init();
	virtual void Input(float fTime);
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Render(float fTime);
	virtual CUser* Clone();
	virtual void OnCollisionEnter(class CDx11Collider* pMy, class CDx11Collider* pYour, float fTime);
	virtual void OnCollision(class CDx11Collider* pMy, class CDx11Collider* pYour, float fTime);
	virtual void OnCollisionExit(class CDx11Collider* pMy, class CDx11Collider* pYour, float fTime);
};

