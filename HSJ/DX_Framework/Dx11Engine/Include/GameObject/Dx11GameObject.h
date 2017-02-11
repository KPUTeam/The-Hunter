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
	static CDx11GameObject* Create(const string& strTag = "GameObject",
		OBJECT_TYPE ot = OT_NORMAL);

private:
	class CDx11Scene*	m_pScene;
	class CDx11Layer*	m_pLayer;

public:
	void SetScene(CDx11Scene* pScene);
	void SetLayer(CDx11Layer* pLayer);
	void Static(bool bStatic);

private:
	list<class CDx11Component*>		m_ComponentList;
	class CDx11Transform*					m_pTransform;
	list<class CDx11Component*>		m_FindList;

public:
	class CDx11Transform* GetTransform();

private:
	list<CDx11GameObject*>			m_ChildList;
	CDx11GameObject*				m_pParent;

public:
	void SetParent(CDx11GameObject* pParent);
	CDx11GameObject* GetParent();
	list<CDx11GameObject*>* GetChildList();

public:
	bool Init();
	void Input(float fTime);
	int Update(float fTime);
	int LateUpdate(float fTime);
	void Render(float fTime);
	CDx11GameObject* Clone()	const;

public:
	CDx11Component* FindComponentFromTag(const string& strTag);
	CDx11Component* FindComponentFromTypeName(const string& strTypeName);
	CDx11Component* FindComponentFromType(COMPONENT_TYPE eType);
	list<CDx11Component*>* FindComponentsFromTag(const string& strTag);
	list<CDx11Component*>* FindComponentsFromTypeName(const string& strTypeName);
	list<CDx11Component*>* FindComponentsFromType(COMPONENT_TYPE eType);
	CDx11GameObject* FindChildFromTag(const string& strTag);
	CDx11GameObject* FindChildFromTypeName(const string& strTypeName);

public:
	void AddChild(CDx11GameObject* pChild);
	void AddComponent(CDx11Component* pComponent);
	template <typename T>
	T* AddComponent(const string& strTag = "Component")
	{
		T*	pComponent = new T;

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

public:
	void UpdateChildTransform();
};

DX11_END