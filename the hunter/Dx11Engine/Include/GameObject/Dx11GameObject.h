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
//	static으로 만들어 private 생성자에 접근
	static CDx11GameObject* Create(const string& strTag = "GameObject",
		OBJECT_TYPE ot = OT_NORMAL);

private:
//	게임오브젝트는 자신이 속한 씬과 레이어 정보를 가진다.
	class CDx11Scene*	m_pScene;
	class CDx11Layer*	m_pLayer;

public:
// 자신의 씬과 레이어를 셋한다.
	void SetScene(CDx11Scene* pScene);
	void SetLayer(CDx11Layer* pLayer);
	void Static(bool bStatic);

private:
// 게임오브젝트는 컴포넌트들로 이루어져있다.
// 컴포넌트는 추가, 삭제가 잦으므로 리스트로 관리한다.
	list<class CDx11Component*>		m_ComponentList;
	list<class CDx11Component*>		m_FindList;

private:
	class CDx11Transform*			m_pTransform;

public:
// 변환정보 Getter 함수
	class CDx11Transform* GetTransform();

private:
// 자신의 차일드 리스트와 부모 오브젝트
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
//	컴포넌트를 문자열로 찾는 함수들이다.
//	하나의 컴포넌트를 태그 또는 타입네임으로 찾거나,
//	중복된 이름의 컴포넌트들을 태그 또는 타입네임으로 찾는다.
	CDx11Component* FindComponentFromTag(const string& strTag);
	CDx11Component* FindComponentFromTypeName(const string& strTypeName);
	CDx11Component* FindComponentFromType(COMPONENT_TYPE eType);
	list<CDx11Component*>* FindComponentsFromTag(const string& strTag);
	list<CDx11Component*>* FindComponentsFromTypeName(const string& strTypeName);
	list<CDx11Component*>* FindComponentsFromType(COMPONENT_TYPE eType);

//	차일드를 문자열로 찾는 함수.
	CDx11GameObject* FindChildFromTag(const string& strTag);
	CDx11GameObject* FindChildFromTypeName(const string& strTypeName);

public:
//	RenderManager
	void AddRenderObject();

//	차일드를 생성하는 함수
	void AddChild(CDx11GameObject* pChild);

//	컴포넌트를 추가하는 함수
//	이미 있는 컴포넌트인 경우
	void AddComponent(CDx11Component* pComponent);

//	어떤 타입으로 추가하게 될지 모르기 때문에 템플릿으로 작성한다.
	template <typename T>
	T* AddComponent(const string& strTag = "Component")
	{
		// 해당 타입의 컴포넌트를 동적 메모리 할당
		T* pComponent = new T;

		// 컴포넌트에게 자신이 가지고 있는 씬, 레이어 정보를 넘겨준다.
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