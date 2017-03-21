#pragma once
#include "Component\Dx11Script.h"

DX11_USING	

class CMonster :
	public CDx11Script
{
public:
	CMonster();
	CMonster(const CMonster& monster);
	~CMonster();

public:
	virtual bool Init();
	virtual void Input(float fTime);
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Render(float fTime);
	virtual CMonster* Clone();
	virtual void OnCollisionEnter(class CDx11Collider* pColl, float fTime);
	virtual void OnCollision(class CDx11Collider* pColl, float fTime);
	virtual void OnCollisionExit(class CDx11Collider* pColl, float fTime);

private:
	int m_iCnt;
	void Fire();
	class CDx11GameObject* CreateBullet(class CDx11Transform* pTransform);
};

