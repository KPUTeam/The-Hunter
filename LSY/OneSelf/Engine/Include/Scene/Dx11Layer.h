#pragma once
#include "../Dx11Object.h"

DX11_BEGIN

class DX11_DLL  CDx11Layer:
	public CDx11Object
{
private:
	friend class CDx11Scene;

private:
	CDx11Layer();
	~CDx11Layer();

private:
	list<class CDx11GameObject*>	m_ObjList;
	class CDx11Scene*				m_pScene;
	list<class CDx11GameObject*>	m_FindList;

public:
	void SetScene(CDx11Scene* pScene);
	void AddObject(CDx11GameObject* pObj);

public:
	bool Init();
	void Input(float fTime);
	int Update(float fTime);
	int LateUpdate(float fTime);
	void Collision(float fTime);
	void Render(float fTime);

public:
	CDx11GameObject* FindObjectFromTag(const string& strTag);
	list<CDx11GameObject*>* FindObjectsFromTag(const string& strTag);
};

DX11_END
