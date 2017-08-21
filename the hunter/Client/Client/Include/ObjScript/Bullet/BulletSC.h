#pragma once
#include "Component\Dx11Script.h"

DX11_USING

class CBulletSC :
	public CDx11Script
{
public:
	CBulletSC();
	CBulletSC(const CBulletSC& script);
	~CBulletSC();

private:
	float		m_fSpeed;
	float		m_fDist;
	float		m_fSize = 0.08f;
	CDx11GameObject* m_pEmployer;

public:
	void SetSpeed(float fSpeed);
	void SetDistance(float fDist);

public:
	virtual bool Init();
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Render(float fTime);
	virtual CBulletSC* Clone();

	void DeathBullet();

	virtual void OnCollisionEnter(class CDx11Collider* pMy, class CDx11Collider* pYour, float fTime);
	virtual void OnCollision(class CDx11Collider* pMy, class CDx11Collider* pYour, float fTime);
	virtual void OnCollisionExit(class CDx11Collider* pMy, class CDx11Collider* pYour, float fTime);

	void SetEmployer(CDx11GameObject* pTar);
	CDx11GameObject* GetEmployer();
};