#include "Dx11GameObject.h"
#include "../Scene/Dx11Scene.h"
#include "../Scene/Dx11Layer.h"
#include "../Component/Dx11Transform.h"
#include "../Component/Dx11Renderer.h"
#include "Dx11GameObjectManager.h"
#include "../Rendering/Dx11RenderManager.h"
#include "../Core/Dx11CollisionManager.h"

DX11_USING

CDx11GameObject::CDx11GameObject() :
	m_pTransform(NULL),
	m_pParent(NULL)
{
	SetTypeName<CDx11GameObject>();
}

//	깊은 복사처리를하는 복사생성자
CDx11GameObject::CDx11GameObject(const CDx11GameObject & obj)
{
//	단순한 값 복사
	*this = obj;

//	먼저 "얕은 복사"가 되어 있는 컴포넌트 리스트를 비움
	m_ComponentList.clear();

	list<class CDx11Component*>::const_iterator iter;
	list<class CDx11Component*>::const_iterator iterEnd = obj.m_ComponentList.end();

//	컴포넌트를 모두 깊은 복사한다.
	for (iter = obj.m_ComponentList.begin(); iter != iterEnd; ++iter)
	{
		CDx11Component* pCom = (*iter)->Clone();
		// 트랜스폼을 얻어쓰기 편하게 저장해둠
		if (pCom->GetComponentType() == CT_TRANSFORM)
			m_pTransform = (CDx11Transform*)pCom;

		pCom->SetTransform(m_pTransform);
		pCom->SetGameObject(this);
		pCom->SetLayer(m_pLayer);
		pCom->SetScene(m_pScene);
		m_ComponentList.push_back(pCom);
	}

	m_pTransform->AddRef();

// 차일드 복사
	m_ChildList.clear();

	list<CDx11GameObject*>::const_iterator	iter1;
	list<CDx11GameObject*>::const_iterator	iter1End = obj.m_ChildList.end();

	for (iter1 = obj.m_ChildList.begin(); iter1 != iter1End; ++iter1)
	{
	//	차일드 리스트를 순회하면서 모든 차일드를 복사해준다.
		CDx11GameObject*	pChild = (*iter1)->Clone();

	//	부모 또한 똑같이 설정
		pChild->SetParent(this);
		m_ChildList.push_back(pChild);
	}
}

CDx11GameObject::~CDx11GameObject()
{
	SAFE_RELEASE(m_pTransform);
	Safe_Release_VecList(m_FindList);
	Safe_Release_VecList(m_ChildList);
	Safe_Release_VecList(m_ComponentList);
//	DX11_GET_SINGLE(CDx11RenderManager)->ReleaseObjCount();
}

CDx11GameObject * CDx11GameObject::Create(const string & strTag, 
	OBJECT_TYPE ot)
{
	CDx11GameObject*	pObj = NULL;

	switch (ot)
	{
	case OT_NORMAL:
	//	노말이면 그냥 할당해줌
		pObj = new CDx11GameObject;
		if (!pObj->Init())
		{
			SAFE_RELEASE(pObj);
			return NULL;
		}
		break;
	case OT_PROTOTYPE:
	//	프로토타입일 경우 메모리 할당해주고,
		pObj = new CDx11GameObject;
		if (!pObj->Init())
		{
			SAFE_RELEASE(pObj);
			return NULL;
		}
	//	오브젝트 매니저에 AddPrototype함수를 호출하여 만들어진 프로토타입을 추가해줌.
		if (DX11_GET_SINGLE(CDx11GameObjectManager)->AddPrototype(pObj, strTag))
			pObj->Release();
		break;
	case OT_CLONE:
	//	클론일 경우 오브젝트 매니저에서 CreateObject 함수를 호출
		pObj = DX11_GET_SINGLE(CDx11GameObjectManager)->CreateObject(strTag);
		break;
	}

	pObj->SetTag(strTag);
	return pObj;

}

void CDx11GameObject::SetScene(CDx11Scene * pScene)
{
	m_pScene = pScene;
	
	list<CDx11Component*>::iterator iter;
	list<CDx11Component*>::iterator iterEnd = m_ComponentList.end();

	for (iter = m_ComponentList.begin(); iter != iterEnd; ++iter)
	{
		(*iter)->SetScene(pScene);
	}

	list<CDx11GameObject*>::iterator iter1;
	list<CDx11GameObject*>::iterator iter1End = m_ChildList.end();

	for (iter1 = m_ChildList.begin(); iter1 != iter1End; ++iter1)
	{
		(*iter1)->SetScene(pScene);
	}

}

void CDx11GameObject::SetLayer(CDx11Layer * pLayer)
{
	m_pLayer = pLayer;

	list<CDx11Component*>::iterator iter;
	list<CDx11Component*>::iterator iterEnd = m_ComponentList.end();

	for (iter = m_ComponentList.begin(); iter != iterEnd; ++iter)
	{
		(*iter)->SetLayer(pLayer);
	}

	list<CDx11GameObject*>::iterator	iter1;
	list<CDx11GameObject*>::iterator	iter1End = m_ChildList.end();

	for (iter1 = m_ChildList.begin(); iter1 != iter1End; ++iter1)
	{
		(*iter1)->SetLayer(pLayer);
	}
}

void CDx11GameObject::Static(bool bStatic)
{
	//m_pTransform->Static(bStatic);
	//list<CDx11GameObject*>::iterator	iter1;
	//list<CDx11GameObject*>::iterator	iter1End = m_ChildList.end();

	//for (iter1 = m_ChildList.begin(); iter1 != iter1End; ++iter1)
	//{
	//	(*iter1)->Static(bStatic);
	//}
}

CDx11Transform * CDx11GameObject::GetTransform()
{
	m_pTransform->AddRef();

	return m_pTransform;
}

void CDx11GameObject::SetParent(CDx11GameObject * pParent)
{
	m_pParent = pParent;
}

CDx11GameObject * CDx11GameObject::GetParent()
{
//	부모가 없으면 NULL을 리턴
	if (!m_pParent)
		return NULL;

	m_pParent->AddRef();
	return m_pParent;
}

list<CDx11GameObject*>* CDx11GameObject::GetChildList()
{
	return &m_ChildList;
}

bool CDx11GameObject::Init()
{
	// 트랜스폼은 모든 게임오브젝트가 반드시 가지고 있어야한다.
	m_pTransform = AddComponent<CDx11Transform>("Transform");

	return true;
}

void CDx11GameObject::Input(float fTime)
{
	list<class CDx11Component*>::iterator	iter;
	list<class CDx11Component*>::iterator	iterEnd = m_ComponentList.end();

	for (iter = m_ComponentList.begin(); iter != iterEnd;)
	{
		if (!(*iter)->GetEnable())
		{
			++iter;
			continue;
		}

		else if ((*iter)->GetComponentType() == CT_TRANSFORM)
		{
			++iter;
			continue;
		}

		(*iter)->Input(fTime);

		if (!(*iter)->GetAlive())
		{
			SAFE_RELEASE((*iter));
			iter = m_ComponentList.erase(iter);
		}

		else
			++iter;
	}

	list<CDx11GameObject*>::iterator	iter1;
	list<CDx11GameObject*>::iterator	iter1End = m_ChildList.end();

	for (iter1 = m_ChildList.begin(); iter1 != iter1End;)
	{
		if (!(*iter1)->GetEnable())
		{
			++iter1;
			continue;
		}

		(*iter1)->Input(fTime);

		if (!(*iter1)->GetAlive())
		{
			SAFE_RELEASE((*iter1));
			iter1 = m_ChildList.erase(iter1);
		}

		else
			++iter1;
	}
}

int CDx11GameObject::Update(float fTime)
{
	list<class CDx11Component*>::iterator	iter;
	list<class CDx11Component*>::iterator	iterEnd = m_ComponentList.end();

	for (iter = m_ComponentList.begin(); iter != iterEnd;)
	{
		if (!(*iter)->GetEnable())
		{
			++iter;
			continue;
		}

		else if ((*iter)->GetComponentType() == CT_TRANSFORM)
		{
			++iter;
			continue;
		}

		(*iter)->Update(fTime);

		if (!(*iter)->GetAlive())
		{
			SAFE_RELEASE((*iter));
			iter = m_ComponentList.erase(iter);
		}

		else
			++iter;
	}

	m_pTransform->Update(fTime);


	list<CDx11GameObject*>::iterator	iter1;
	list<CDx11GameObject*>::iterator	iter1End = m_ChildList.end();

	for (iter1 = m_ChildList.begin(); iter1 != iter1End;)
	{
		if (!(*iter1)->GetEnable())
		{
			++iter1;
			continue;
		}

		(*iter1)->Update(fTime);

		if (!(*iter1)->GetAlive())
		{
			SAFE_RELEASE((*iter1));
			iter1 = m_ChildList.erase(iter1);
		}

		else
			++iter1;
	}
	
	return 0;
}

int CDx11GameObject::LateUpdate(float fTime)
{
	list<class CDx11Component*>::iterator	iter;
	list<class CDx11Component*>::iterator	iterEnd = m_ComponentList.end();

	for (iter = m_ComponentList.begin(); iter != iterEnd;)
	{
		if (!(*iter)->GetEnable())
		{
			++iter;
			continue;
		}

		else if ((*iter)->GetComponentType() == CT_TRANSFORM)
		{
			++iter;
			continue;
		}

		(*iter)->LateUpdate(fTime);

		if (!(*iter)->GetAlive())
		{
			SAFE_RELEASE((*iter));
			iter = m_ComponentList.erase(iter);
		}

		else
			++iter;
	}

	m_pTransform->LateUpdate(fTime);

	list<CDx11GameObject*>::iterator	iter1;
	list<CDx11GameObject*>::iterator	iter1End = m_ChildList.end();

	for (iter1 = m_ChildList.begin(); iter1 != iter1End;)
	{
		if (!(*iter1)->GetEnable())
		{
			++iter1;
			continue;
		}

		(*iter1)->LateUpdate(fTime);

		if (!(*iter1)->GetAlive())
		{
			SAFE_RELEASE((*iter1));
			iter1 = m_ChildList.erase(iter1);
		}

		else
			++iter1;
	}

	return 0;
}

void CDx11GameObject::Render(float fTime)
{
	list<class CDx11Component*>::iterator	iter;
	list<class CDx11Component*>::iterator	iterEnd = m_ComponentList.end();

	for (iter = m_ComponentList.begin(); iter != iterEnd;)
	{
		if (!(*iter)->GetEnable())
		{
			++iter;
			continue;
		}

		else if ((*iter)->GetComponentType() == CT_TRANSFORM)
		{
			++iter;
			continue;
		}

		(*iter)->Render(fTime);

		if (!(*iter)->GetAlive())
		{
			SAFE_RELEASE((*iter));
			iter = m_ComponentList.erase(iter);
		}

		else
			++iter;
	}
	

	m_pTransform->Render(fTime);


	//list<CDx11GameObject*>::iterator	iter1;
	//list<CDx11GameObject*>::iterator	iter1End = m_ChildList.end();

	//for (iter1 = m_ChildList.begin(); iter1 != iter1End;)
	//{
	//	if (!(*iter1)->GetEnable())
	//	{
	//		++iter1;
	//		continue;
	//	}

	//	(*iter1)->Render(fTime);

	//	if (!(*iter1)->GetAlive())
	//	{
	//		SAFE_RELEASE((*iter1));
	//		iter1 = m_ChildList.erase(iter1);
	//	}

	//	else
	//		++iter1;
	//}
}

CDx11GameObject * CDx11GameObject::Clone() const
{
	//this를 역참조하여 복사생성자에 값을 넣고 new로 별도 할당
	return new CDx11GameObject(*this);
}

void CDx11GameObject::OnCollisionEnter(CDx11Collider * pColl, float fTime)
{
	list<class CDx11Component*>::iterator	iter;
	list<class CDx11Component*>::iterator	iterEnd = m_ComponentList.end();

	for (iter = m_ComponentList.begin(); iter != iterEnd; ++iter)
	{
		if (!(*iter)->GetEnable())
			continue;

		(*iter)->OnCollisionEnter(pColl, fTime);
	}
}

void CDx11GameObject::OnCollision(CDx11Collider * pColl, float fTime)
{
	list<class CDx11Component*>::iterator	iter;
	list<class CDx11Component*>::iterator	iterEnd = m_ComponentList.end();

	for (iter = m_ComponentList.begin(); iter != iterEnd; ++iter)
	{
		if (!(*iter)->GetEnable())
			continue;

		(*iter)->OnCollision(pColl, fTime);
	}
}

void CDx11GameObject::OnCollisionExit(CDx11Collider * pColl, float fTime)
{
	list<class CDx11Component*>::iterator	iter;
	list<class CDx11Component*>::iterator	iterEnd = m_ComponentList.end();

	for (iter = m_ComponentList.begin(); iter != iterEnd; ++iter)
	{
		if (!(*iter)->GetEnable())
			continue;

		(*iter)->OnCollisionExit(pColl, fTime);
	}
}

void CDx11GameObject::AddCollider()
{
	DX11_GET_SINGLE(CDx11CollisionManager)->AddGameObject(this);

	list<CDx11GameObject*>::iterator	iter;
	list<CDx11GameObject*>::iterator	iterEnd = m_ChildList.end();

	for (iter = m_ChildList.begin(); iter != iterEnd; ++iter)
	{
		(*iter)->AddCollider();
	}
}

CDx11Component * CDx11GameObject::FindComponentFromTag(const string & strTag)
{
	list<CDx11Component*>::iterator iter;
	list<CDx11Component*>::iterator iterEnd = m_ComponentList.end();

	for (iter = m_ComponentList.begin(); iter != iterEnd; ++iter)
	{
		//태그로 컴포넌트를 찾는다.
		if ((*iter)->GetTag() == strTag)
		{
			//찾았으면 참조카운트를 1 증가시키고 해당 iterator를 리턴
			(*iter)->AddRef();
			return *iter;
		}
	}

	return NULL;
}

CDx11Component * CDx11GameObject::FindComponentFromTypeName(const string & strTypeName)
{
	list<CDx11Component*>::iterator iter;
	list<CDx11Component*>::iterator iterEnd = m_ComponentList.end();

	for (iter = m_ComponentList.begin(); iter != iterEnd; ++iter)
	{
		//타입네임으로 컴포넌트를 찾는다.
		if ((*iter)->GetTypeName() == strTypeName)
		{
			//찾았으면 참조카운트를 1 증가시키고 해당 iterator를 리턴
			(*iter)->AddRef();
			return *iter;
		}
	}
	return NULL;
}

CDx11Component * CDx11GameObject::FindComponentFromType(COMPONENT_TYPE eType)
{
	list<CDx11Component*>::iterator iter;
	list<CDx11Component*>::iterator iterEnd = m_ComponentList.end();

	for (iter = m_ComponentList.begin(); iter != iterEnd; ++iter)
	{
		//타입네임으로 컴포넌트를 찾는다.
		if ((*iter)->GetComponentType() == eType)
		{
			//찾았으면 참조카운트를 1 증가시키고 해당 iterator를 리턴
			(*iter)->AddRef();
			return *iter;
		}
	}
	return NULL;
}

list<CDx11Component*>* CDx11GameObject::FindComponentsFromTag(const string & strTag)
{
//	같은 이름을 가진 여러개의 컴포넌트를 한번에 찾는 함수이다.
	list<CDx11Component*>::iterator iter;
	list<CDx11Component*>::iterator iterEnd = m_FindList.end();

//	우선 FindList를 비워준다.
	for (iter = m_FindList.begin(); iter != iterEnd; ++iter)
	{
		(*iter)->Release();
	}
	m_FindList.clear();

	iterEnd = m_ComponentList.end();

	for (iter = m_ComponentList.begin(); iter != iterEnd; ++iter)
	{
		if ((*iter)->GetTag() == strTag)
		{
			(*iter)->AddRef();
			m_FindList.push_back(*iter);
		}
	}

	// 직접 리스트를 넘겨주면 너무 느리므로 포인터만 넘겨준다.
	return &m_FindList;
}

list<CDx11Component*>* CDx11GameObject::FindComponentsFromTypeName(const string & strTypeName)
{
	//	같은 이름을 가진 여러개의 컴포넌트를 한번에 찾는 함수이다.
	list<CDx11Component*>::iterator iter;
	list<CDx11Component*>::iterator iterEnd = m_FindList.end();

	//	우선 FindList를 비워준다.
	for (iter = m_FindList.begin(); iter != iterEnd; ++iter)
	{
		(*iter)->Release();
	}
	m_FindList.clear();

	iterEnd = m_ComponentList.end();

	for (iter = m_ComponentList.begin(); iter != iterEnd; ++iter)
	{
		if ((*iter)->GetTypeName() == strTypeName)
		{
			(*iter)->AddRef();
			m_FindList.push_back(*iter);
		}
	}

	// 직접 리스트를 넘겨주면 너무 느리므로 포인터만 넘겨준다.
	return &m_FindList;
}

list<CDx11Component*>* CDx11GameObject::FindComponentsFromType(COMPONENT_TYPE eType)
{
	//	같은 이름을 가진 여러개의 컴포넌트를 한번에 찾는 함수이다.
	list<CDx11Component*>::iterator iter;
	list<CDx11Component*>::iterator iterEnd = m_FindList.end();

	//	우선 FindList를 비워준다.
	for (iter = m_FindList.begin(); iter != iterEnd; ++iter)
	{
		(*iter)->Release();
	}
	m_FindList.clear();

	iterEnd = m_ComponentList.end();

	for (iter = m_ComponentList.begin(); iter != iterEnd; ++iter)
	{
		if ((*iter)->GetComponentType() == eType)
		{
			(*iter)->AddRef();
			m_FindList.push_back(*iter);
		}
	}

	// 직접 리스트를 넘겨주면 너무 느리므로 포인터만 넘겨준다.
	return &m_FindList;
}

CDx11GameObject * CDx11GameObject::FindChildFromTag(const string & strTag)
{
	list<CDx11GameObject*>::iterator iter;
	list<CDx11GameObject*>::iterator iterEnd = m_ChildList.end();

	for (iter = m_ChildList.begin(); iter != iterEnd; ++iter)
	{
		//태그로 컴포넌트를 찾는다.
		if ((*iter)->GetTag() == strTag)
		{
			//찾았으면 참조카운트를 1 증가시키고 해당 iterator를 리턴
			(*iter)->AddRef();
			return *iter;
		}
	}
	return NULL;
}

CDx11GameObject * CDx11GameObject::FindChildFromTypeName(const string & strTypeName)
{
	list<CDx11GameObject*>::iterator iter;
	list<CDx11GameObject*>::iterator iterEnd = m_ChildList.end();

	for (iter = m_ChildList.begin(); iter != iterEnd; ++iter)
	{
		//타입네임으로 컴포넌트를 찾는다.
		if ((*iter)->GetTypeName() == strTypeName)
		{
			//찾았으면 참조카운트를 1 증가시키고 해당 iterator를 리턴
			(*iter)->AddRef();
			return *iter;
		}
	}
	return NULL;
}

void CDx11GameObject::AddRenderObject()
{
	// Layer에서 RenderManager에 오브젝트를 추가하지 않고, 
	// GameObject에서 직접적으로 추가해줌
	// 따라서 Child에 대해서도 처리를 해 줄 수 있따.

	DX11_GET_SINGLE(CDx11RenderManager)->AddRenderObject(this);

	list<CDx11GameObject*>::iterator	iter;
	list<CDx11GameObject*>::iterator	iterEnd = m_ChildList.end();

	for (iter = m_ChildList.begin(); iter != iterEnd; ++iter)
	{
		(*iter)->AddRenderObject();
	}
}

void CDx11GameObject::AddChild(CDx11GameObject * pChild)
{
//	차일드는 부모와 같은 정보를 같는다.
	pChild->AddRef();
	pChild->SetScene(m_pScene);
	pChild->SetLayer(m_pLayer);
	pChild->SetParent(this);
	m_ChildList.push_back(pChild);
}

void CDx11GameObject::AddComponent(CDx11Component * pComponent)
{
//	참조 카운트 1 증가
	pComponent->AddRef();
//	씬과 레이어, 게임오브젝트를 셋
	pComponent->SetScene(m_pScene);
	pComponent->SetLayer(m_pLayer);
	pComponent->SetGameObject(this);
	pComponent->SetTransform(m_pTransform);

//	컴포넌트 리스트에 추가
	m_ComponentList.push_back(pComponent);
}

