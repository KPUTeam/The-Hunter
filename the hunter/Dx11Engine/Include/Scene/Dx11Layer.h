#pragma once
#include "../Dx11Object.h"

DX11_BEGIN

class DX11_DLL CDx11Layer :
	public CDx11Object
{
private:
	//Scene Ŭ�������� Layer�� ����, �����ϹǷ� friend�� ����.
	friend class CDx11Scene;

private:
	CDx11Layer();
	~CDx11Layer();

private:
//	���ӿ�����Ʈ�� �߰�, ������ ����ϹǷ� ����Ʈ�� �����Ѵ�.
	list<class CDx11GameObject*>	m_ObjList;
	list<class CDx11GameObject*>	m_FindList;

private:
//	�ڽ��� ������ �� ������ ���´�.
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