#pragma once
#include "../Dx11Object.h"

DX11_BEGIN

class DX11_DLL CDx11Layer :
	public CDx11Object
{
private:
	friend class CDx11Scene;

private:
	CDx11Layer();
	~CDx11Layer();

private:
	list<class CDx11GameObject*>		m_ObjList;
	class CDx11Scene*		m_pScene;

public:
	bool Init();
	int Update(float fTime);
	// void UpdateMatrix();
	int LateUpdate(float fTime);
	void AddCollision();
	void Render(float fTime);
	void AddObject(CDx11GameObject* pGameObject);
	void SetScene(CDx11Scene* pScene);
	CDx11GameObject* FindObjectFromTag(const string& strTag);
	void Destroy();

public:
	CDx11Scene* GetScene();
};

DX11_END
