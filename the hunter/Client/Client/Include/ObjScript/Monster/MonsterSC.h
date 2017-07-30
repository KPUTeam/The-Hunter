#pragma once
#include "Component\Dx11Script.h"

DX11_USING

class CMonsterSC :
	public CDx11Script
{
public:
	CMonsterSC();
	CMonsterSC(const CMonsterSC& script);
	~CMonsterSC();

public:
	virtual bool Init();
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Render(float fTime);
	virtual CMonsterSC* Clone();

	CDx11GameObject* m_pTarget;

	float				m_fHP;
	CDx11GameObject*	m_pHPBar;
	CDx11GameObject*	m_pFrameBar;

public:
	void SetTarget(CDx11GameObject* pTar);
	CDx11GameObject* GetTarget();
	void CreateBar();
	void SetBarPos();

public:
	virtual void OnCollisionEnter(class CDx11Collider* pMy, class CDx11Collider* pYour, float fTime);
	virtual void OnCollision(class CDx11Collider* pMy, class CDx11Collider* pYour, float fTime);
	virtual void OnCollisionExit(class CDx11Collider* pMy, class CDx11Collider* pYour, float fTime);
};