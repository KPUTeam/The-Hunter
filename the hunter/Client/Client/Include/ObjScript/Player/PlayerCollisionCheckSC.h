#pragma once
#include "Component\Dx11Script.h"

DX11_USING
class CPlayerCollisionCheckSC:
	public CDx11Script
{
public:
	CPlayerCollisionCheckSC();
	CPlayerCollisionCheckSC(const CPlayerCollisionCheckSC& script);
	~CPlayerCollisionCheckSC();

public:
	virtual bool Init();
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Render(float fTime);
	virtual CPlayerCollisionCheckSC* Clone();

private:
	class CDx11GameObject* m_pTarget;

public:
	CDx11GameObject* GetTarget();

public:
	virtual void OnCollisionEnter(class CDx11Collider* pMy, class CDx11Collider* pYour, float fTime);
	virtual void OnCollision(class CDx11Collider* pMy, class CDx11Collider* pYour, float fTime);
	virtual void OnCollisionExit(class CDx11Collider* pMy, class CDx11Collider* pYour, float fTime);
};