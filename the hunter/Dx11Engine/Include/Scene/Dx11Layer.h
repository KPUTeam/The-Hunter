#pragma once
#include "../Dx11Object.h"

DX11_BEGIN

class DX11_DLL CDx11Layer :
	public CDx11Object
{
private:
	//Scene 클래스에서 Layer를 생성, 관리하므로 friend로 설정.
	friend class CDx11Scene;

private:
	CDx11Layer();
	~CDx11Layer();

private:
//	게임오브젝트는 추가, 삭제가 빈번하므로 리스트로 관리한다.
	list<class CDx11GameObject*>	m_ObjList;
	list<class CDx11GameObject*>	m_FindList;

private:
//	자신을 소유한 씬 정보를 갖는다.
	class CDx11Scene*				m_pScene;

public:
	CDx11GameObject*	FindGameObjectFromTag(const string& strTag);
	list<CDx11GameObject*>* FindGameObjectsFromTag(const string& strTag);

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
};

DX11_END