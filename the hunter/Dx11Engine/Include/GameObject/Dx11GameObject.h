#pragma once
#include "../Dx11Object.h"

DX11_BEGIN

class DX11_DLL CDx11GameObject :
	public CDx11Object
{
private:
	CDx11GameObject();
	CDx11GameObject(const CDx11GameObject& obj);
	~CDx11GameObject();

public:
//	static���� ����� private �����ڿ� ����
	static CDx11GameObject* Create(const string& strTag = "GameObject",
		OBJECT_TYPE ot = OT_NORMAL);

private:
//	���ӿ�����Ʈ�� �ڽ��� ���� ���� ���̾� ������ ������.
	class CDx11Scene*	m_pScene;
	class CDx11Layer*	m_pLayer;

public:
// �ڽ��� ���� ���̾ ���Ѵ�.
	void SetScene(CDx11Scene* pScene);
	void SetLayer(CDx11Layer* pLayer);
	void Static(bool bStatic);

private:
// ���ӿ�����Ʈ�� ������Ʈ��� �̷�����ִ�.
// ������Ʈ�� �߰�, ������ �����Ƿ� ����Ʈ�� �����Ѵ�.
	list<class CDx11Component*>		m_ComponentList;
	list<class CDx11Component*>		m_FindList;

private:
	class CDx11Transform*			m_pTransform;

public:
// ��ȯ���� Getter �Լ�
	class CDx11Transform* GetTransform();

private:
// �ڽ��� ���ϵ� ����Ʈ�� �θ� ������Ʈ
	list<CDx11GameObject*>			m_ChildList;
	CDx11GameObject*				m_pParent;

public:
	void SetParent(CDx11GameObject* pParent);
	CDx11GameObject* GetParent();
	list<CDx11GameObject*>*	GetChildList();

public:
	bool Init();
	void Input(float fTime);
	int Update(float fTime);
	int LateUpdate(float fTime);
	void Render(float fTime);
	CDx11GameObject* Clone()	const;
	void OnCollisionEnter(class CDx11Collider* pColl, float fTime);
	void OnCollision(class CDx11Collider* pColl, float fTime);
	void OnCollisionExit(class CDx11Collider* pColl, float fTime);
	void AddCollider();

public:
//	������Ʈ�� ���ڿ��� ã�� �Լ����̴�.
//	�ϳ��� ������Ʈ�� �±� �Ǵ� Ÿ�Գ������� ã�ų�,
//	�ߺ��� �̸��� ������Ʈ���� �±� �Ǵ� Ÿ�Գ������� ã�´�.
	CDx11Component* FindComponentFromTag(const string& strTag);
	CDx11Component* FindComponentFromTypeName(const string& strTypeName);
	CDx11Component* FindComponentFromType(COMPONENT_TYPE eType);
	list<CDx11Component*>* FindComponentsFromTag(const string& strTag);
	list<CDx11Component*>* FindComponentsFromTypeName(const string& strTypeName);
	list<CDx11Component*>* FindComponentsFromType(COMPONENT_TYPE eType);

//	���ϵ带 ���ڿ��� ã�� �Լ�.
	CDx11GameObject* FindChildFromTag(const string& strTag);
	CDx11GameObject* FindChildFromTypeName(const string& strTypeName);

public:
//	RenderManager
	void AddRenderObject();

//	���ϵ带 �����ϴ� �Լ�
	void AddChild(CDx11GameObject* pChild);

//	������Ʈ�� �߰��ϴ� �Լ�
//	�̹� �ִ� ������Ʈ�� ���
	void AddComponent(CDx11Component* pComponent);

//	� Ÿ������ �߰��ϰ� ���� �𸣱� ������ ���ø����� �ۼ��Ѵ�.
	template <typename T>
	T* AddComponent(const string& strTag = "Component")
	{
		// �ش� Ÿ���� ������Ʈ�� ���� �޸� �Ҵ�
		T* pComponent = new T;

		// ������Ʈ���� �ڽ��� ������ �ִ� ��, ���̾� ������ �Ѱ��ش�.
		pComponent->SetScene(m_pScene);
		pComponent->SetLayer(m_pLayer);
		pComponent->SetGameObject(this);
		pComponent->SetTransform(m_pTransform);
		pComponent->SetTag(strTag);

		if (!pComponent->Init())
		{
			SAFE_RELEASE(pComponent);
			return NULL;
		}

		pComponent->AddRef();
		m_ComponentList.push_back(pComponent);

		return pComponent;
	}

};

DX11_END