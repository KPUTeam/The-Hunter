#pragma once
#include "../Dx11Object.h"

DX11_BEGIN

class DX11_DLL CDx11CollisionManager :
	public CDx11Object
{
	DX11_SINGLE_DECLARE(CDx11CollisionManager)

private:
	vector<class CDx11GameObject*>	m_PlayerList;
	vector<class CDx11GameObject*>	m_MonsterList;
	vector<class CDx11GameObject*>	m_BulletList;
	vector<class CDx11GameObject*>	m_StaticList;
	vector<class CDx11GameObject*>	m_ObjList;


	bool		m_bColliderRender;

public:
	bool Init();
	void Input(float fTime);
	void AddCollisionObject(class CDx11GameObject* pObj);
	void Collision(float fTime);
	void SetColliderRenderEnable(bool bEnable);
	void ColliderRenderEnable();
	bool GetColliderRender();

private:
	void Collision(float fTime, CDx11GameObject* pObj1,  CDx11GameObject* pObj2 );
};

DX11_END
