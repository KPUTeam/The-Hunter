#include "Dx11GameObject.h"
#include "../Scene/Dx11Scene.h"
#include "../Scene/Dx11layer.h"
#include "../Component/Dx11Transform.h"
#include "../Component/Dx11Renderer.h"
#include "Dx11GameObjectManager.h"

DX11_USING

CDx11GameObject::CDx11GameObject() :
	m_pTransform(NULL),
	m_pParent(NULL)
{
	SetTypeName<CDx11GameObject>();
}

CDx11GameObject::CDx11GameObject(const CDx11GameObject & obj)
{
	*this = obj;

	m_ComponentList.clear();

	list<class CDx11Component*>::const_iterator	iter;
	list<class CDx11Component*>::const_iterator	iterEnd = obj.m_ComponentList.end();

	for (iter = obj.m_ComponentList.begin(); iter != iterEnd; ++iter)
	{
		CDx11Component*	pCom = (*iter)->Clone();

		if (pCom->GetComponentType() == CT_TRANSFORM)
			m_pTransform = (CDx11Transform*)pCom;

		pCom->SetTransform(m_pTransform);
		pCom->SetGameObject(this);
		m_ComponentList.push_back(pCom);
	}

	m_pTransform->AddRef();

	m_ChildList.clear();

	list<CDx11GameObject*>::const_iterator	iter1;
	list<CDx11GameObject*>::const_iterator	iter1End = obj.m_ChildList.end();

	for (iter1 = obj.m_ChildList.begin(); iter1 != iter1End; ++iter1)
	{
		CDx11GameObject*	pChild = (*iter1)->Clone();

		pChild->SetParent(this);

		m_ChildList.push_back(pChild);
	}
}

CDx11GameObject::~CDx11GameObject()
{
	Safe_Release_VecList(m_ChildList);
	SAFE_RELEASE(m_pTransform);
	Safe_Release_VecList(m_ComponentList);
}

CDx11GameObject * CDx11GameObject::Create(const string & strTag,
	OBJECT_TYPE ot)
{
	CDx11GameObject*	pObj = NULL;

	switch (ot)
	{
	case OT_NORMAL:
		pObj = new CDx11GameObject;

		if (!pObj->Init())
		{
			SAFE_RELEASE(pObj);
			return NULL;
		}
		break;
	case OT_PROTOTYPE:
		pObj = new CDx11GameObject;

		if (!pObj->Init())
		{
			SAFE_RELEASE(pObj);
			return NULL;
		}

		/*if (DX11_GET_SINGLE(CDx11GameObjectManager)->AddPrototype(pObj, strTag))
			pObj->Release();*/
		DX11_GET_SINGLE(CDx11GameObjectManager)->AddPrototype(pObj, strTag);
		break;

	case OT_CLONE:
		pObj = DX11_GET_SINGLE(CDx11GameObjectManager)->CreateObject(strTag);
		break;
	}

	pObj->SetTag(strTag);

	return pObj;
}

void CDx11GameObject::SetScene(CDx11Scene * pScene)
{
	m_pScene = pScene;

	list<CDx11Component*>::iterator	iter;
	list<CDx11Component*>::iterator	iterEnd = m_ComponentList.end();

	for (iter = m_ComponentList.begin(); iter != iterEnd; ++iter)
	{
		(*iter)->SetScene(pScene);
	}

	list<CDx11GameObject*>::iterator	iter1;
	list<CDx11GameObject*>::iterator	iter1End = m_ChildList.end();

	for (iter1 = m_ChildList.begin(); iter1 != iter1End; ++iter1)
	{
		(*iter1)->SetScene(pScene);
	}
}

void CDx11GameObject::SetLayer(CDx11Layer * pLayer)
{
	m_pLayer = pLayer;

	list<CDx11Component*>::iterator	iter;
	list<CDx11Component*>::iterator	iterEnd = m_ComponentList.end();

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
	// Transform을 생성한다.
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

		/*else if ((*iter)->GetComponentType() == CT_TRANSFORM)
		{
		++iter;
		continue;
		}*/

		(*iter)->Render(fTime);

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

		(*iter1)->Render(fTime);

		if (!(*iter1)->GetAlive())
		{
			SAFE_RELEASE((*iter1));
			iter1 = m_ChildList.erase(iter1);
		}

		else
			++iter1;
	}
}

CDx11GameObject * CDx11GameObject::Clone() const
{
	return new CDx11GameObject(*this);
}

CDx11Component * CDx11GameObject::FindComponentFromTag(const string & strTag)
{
	list<CDx11Component*>::iterator	iter;
	list<CDx11Component*>::iterator	iterEnd = m_ComponentList.end();

	for (iter = m_ComponentList.begin(); iter != iterEnd; ++iter)
	{
		if ((*iter)->GetTag() == strTag)
		{
			(*iter)->AddRef();
			return *iter;
		}
	}

	return NULL;
}

CDx11Component * CDx11GameObject::FindComponentFromTypeName(const string & strTypeName)
{
	list<CDx11Component*>::iterator	iter;
	list<CDx11Component*>::iterator	iterEnd = m_ComponentList.end();

	for (iter = m_ComponentList.begin(); iter != iterEnd; ++iter)
	{
		if ((*iter)->GetTypeName() == strTypeName)
		{
			(*iter)->AddRef();
			return *iter;
		}
	}

	return NULL;
}

CDx11Component * CDx11GameObject::FindComponentFromType(COMPONENT_TYPE eType)
{
	list<CDx11Component*>::iterator	iter;
	list<CDx11Component*>::iterator	iterEnd = m_ComponentList.end();

	for (iter = m_ComponentList.begin(); iter != iterEnd; ++iter)
	{
		if ((*iter)->GetComponentType() == eType)
		{
			(*iter)->AddRef();
			return *iter;
		}
	}

	return NULL;
}

list<CDx11Component*>* CDx11GameObject::FindComponentsFromTag(const string & strTag)
{
	list<CDx11Component*>::iterator	iter;
	list<CDx11Component*>::iterator	iterEnd = m_FindList.end();

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

	return &m_FindList;
}

list<CDx11Component*>* CDx11GameObject::FindComponentsFromTypeName(const string & strTypeName)
{
	list<CDx11Component*>::iterator	iter;
	list<CDx11Component*>::iterator	iterEnd = m_FindList.end();

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

	return &m_FindList;
}

list<CDx11Component*>* CDx11GameObject::FindComponentsFromType(COMPONENT_TYPE eType)
{
	list<CDx11Component*>::iterator	iter;
	list<CDx11Component*>::iterator	iterEnd = m_FindList.end();

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

	return &m_FindList;
}

CDx11GameObject * CDx11GameObject::FindChildFromTag(const string & strTag)
{
	list<CDx11GameObject*>::iterator	iter;
	list<CDx11GameObject*>::iterator	iterEnd = m_ChildList.end();

	for (iter = m_ChildList.begin(); iter != iterEnd; ++iter)
	{
		if ((*iter)->GetTag() == strTag)
		{
			(*iter)->AddRef();
			return *iter;
		}
	}

	return NULL;
}

CDx11GameObject * CDx11GameObject::FindChildFromTypeName(const string & strTypeName)
{
	list<CDx11GameObject*>::iterator	iter;
	list<CDx11GameObject*>::iterator	iterEnd = m_ChildList.end();

	for (iter = m_ChildList.begin(); iter != iterEnd; ++iter)
	{
		if ((*iter)->GetTypeName() == strTypeName)
		{
			(*iter)->AddRef();
			return *iter;
		}
	}

	return NULL;
}

void CDx11GameObject::AddChild(CDx11GameObject * pChild)
{
	pChild->AddRef();
	pChild->SetScene(m_pScene);
	pChild->SetLayer(m_pLayer);
	pChild->SetParent(this);
	m_ChildList.push_back(pChild);
}

void CDx11GameObject::AddComponent(CDx11Component * pComponent)
{
	pComponent->AddRef();
	pComponent->SetScene(m_pScene);
	pComponent->SetLayer(m_pLayer);
	pComponent->SetGameObject(this);
	pComponent->SetTransform(m_pTransform);
	m_ComponentList.push_back(pComponent);
}

void CDx11GameObject::UpdateChildTransform()
{
	//list<CDx11GameObject*>::iterator	iter;
	//list<CDx11GameObject*>::iterator	iterEnd = m_ChildList.end();

	//for (iter = m_ChildList.begin(); iter != iterEnd; ++iter)
	//{
	//	CDx11Transform*	pTransform = (*iter)->GetTransform();

	//	pTransform->EnableUpdate();

	//	SAFE_RELEASE(pTransform);
	//}
}
