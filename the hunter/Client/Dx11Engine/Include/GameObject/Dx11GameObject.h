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

private:
	bool m_bCollide = true;

	list<class CDx11Component*>		m_ComponentList;
	list<class CDx11Component*>		m_FindComponentList;
	class CDx11Transform*			m_pTransform;
	class CDx11Scene*				m_pScene;
	class CDx11Layer*				m_pLayer;

private:
	list<CDx11GameObject*>			m_ChildList;
	CDx11GameObject*				m_pParent;
	bool							m_bGravity;
	float							m_fGravityTime;

public:
	static CDx11GameObject* Create(const string& strTag = "Object", OBJECT_CREATE_TYPE eOct = OCT_NORMAL);
	static CDx11GameObject* Create(CDx11Scene* pScene,
		CDx11Layer* pLayer, const string& strTag = "Object",
		OBJECT_CREATE_TYPE eOct = OCT_NORMAL);

public:
	bool IsCollision();
	void SetCollision(bool bColl);

	const list<class CDx11Component*>* GetFindList();
	bool GetGravity()	const;
	float GetGravityTime()	const;

public:
	void ClearGravityTime();
	void EnableGravity(bool bGravity = true);
	bool Init();
	int Update(float fTime);
	// void UpdateMatrix();
	int LateUpdate(float fTime);
	void Render(float fTime);
	CDx11GameObject* Clone();
	void AddRenderObject();
	void OnCollisionEnter(class CDx11Collider* pMy, class CDx11Collider* pYour, float fTime);
	void OnCollision(class CDx11Collider* pMy, class CDx11Collider* pYour, float fTime);
	void OnCollisionExit(class CDx11Collider* pMy, class CDx11Collider* pYour, float fTime);
	void Destroy();
	void AddCollision();

public:
	void SetParent(CDx11GameObject* pParent);
	void AddChild(CDx11GameObject* pChild);
	void DeleteChild(CDx11GameObject* pChild);
	CDx11GameObject* GetParent();
	list<CDx11GameObject*>* GetChildList();



public:
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
	CDx11Transform* GetTransform()	const;
	CDx11Scene* GetScene()	const;
	CDx11Layer* GetLayer()	const;

public:
	void SetScene(CDx11Scene* pScene);
	void SetLayer(CDx11Layer* pLayer);

public:
	void DeleteComponent(const string& strTag);
	CDx11Component* FindComponentFromTag(const string& strTag)	const;
	CDx11Component* FindComponentFromTypeName(const string& strTypeName)	const;
	CDx11Component* FindComponentFromType(COMPONENT_TYPE eType)	const;
	const list<CDx11Component*>* FindComponentsFromTag(const string& strTag);
	const list<CDx11Component*>* FindComponentsFromTypeName(const string& strTypeName);
	const list<CDx11Component*>* FindComponentsFromType(COMPONENT_TYPE eType);
	list<CDx11Component*>* FindComponentsFromType_NonConst(COMPONENT_TYPE eType);

	CDx11GameObject* FindChildFromTag(const string& strTag)	const;
	CDx11GameObject* FindChildFromComponentTypeName(const string& strName)	const;
};

DX11_END
