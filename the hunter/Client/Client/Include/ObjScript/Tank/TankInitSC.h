#pragma once
#include "Component\Dx11Script.h"
#include "../../ClientKey.h"

DX11_USING

class CTankInitSC :
	public CDx11Script
{
public:
	CTankInitSC();
	CTankInitSC(const CTankInitSC& script);
	~CTankInitSC();

public:
	virtual bool Init();
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Render(float fTime);
	virtual CTankInitSC* Clone();

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

private:
	float m_fSpeed = 0.02;
	float m_vDir = 0;
	float m_vAcc = 0;

	bool LeftTurnTrigger = false;
	bool RightTurnTrigger = false;

private:
	bool CreateBody();
	bool CreateLTWheel();
	bool CreateRTWheel();
	bool CreateLBWheel();
	bool CreateRBWheel();
	bool CreateHead();

public:
	void Input(float fTime);
	void RotateWheel(float fSpeed);
	void SetHeight(float fTime);
	void CreateBullet(CDx11Transform * pTransform, const Vec3 & vDir);
};

