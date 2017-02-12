#pragma once
#include "../Dx11Object.h"

DX11_BEGIN

class DX11_DLL CDx11CollisionManager :
	public CDx11Object
{
	DX11_DECLARE_SINGLE(CDx11CollisionManager)

private:
	vector<class CDx11GameObject*>	m_ObjList;

public:
	bool Init();
	void AddGameObject(class CDx11GameObject* pObj);
	void Collision(float fTime);
	
private:
	bool Collision(CDx11GameObject* pObj1, CDx11GameObject* pObj2);
};

DX11_END
