#include "Dx11GameObject.h"
#include "../Component/Dx11Transform.h"
#include "../Scene/Dx11Scene.h"
#include "../Scene/Dx11Layer.h"
#include "Dx11PrototypeManager.h"
#include "../Core/Dx11CollisionManager.h"
#include "../Rendering/Dx11RenderManager.h"
#include "../Dx11Core.h"
#include "../Component/Dx11Component.h"

DX11_USING

CDx11GameObject::CDx11GameObject() :
	m_pTransform(NULL),
	m_pScene(NULL),
	m_pLayer(NULL),
	m_pParent(NULL),
	m_bGravity(false)
{
	SetTypeName<CDx11GameObject>();
}

CDx11GameObject::CDx11GameObject(const CDx11GameObject & obj)
{
	*this = obj;
	m_iRef = 1;

	m_ComponentList.clear();

	list<CDx11Component*>::const_iterator	iter;
	list<CDx11Component*>::const_iterator	iterEnd = obj.m_ComponentList.end();

	for (iter = obj.m_ComponentList.begin(); iter != iterEnd; ++iter)
	{
		CDx11Component*	pCom = (*iter)->Clone();

		if (pCom->GetComponentType() == CT_TRANSFORM)
		{
			m_pTransform = (CDx11Transform*)pCom;
			m_pTransform->AddRef();
		}

		m_ComponentList.push_back(pCom);
	}

	list<CDx11Component*>::iterator	iter1;
	list<CDx11Component*>::iterator	iter1End = m_ComponentList.end();

	for (iter1 = m_ComponentList.begin(); iter1 != iter1End; ++iter1)
	{
		(*iter1)->SetGameObject(this);
		(*iter1)->SetTransform(m_pTransform);
	}

	m_pScene = NULL;
	m_pLayer = NULL;

	m_pParent = NULL;

	m_ChildList.clear();

	list<CDx11GameObject*>::const_iterator	iter2;
	list<CDx11GameObject*>::const_iterator	iter2End = obj.m_ChildList.end();

	for (iter2 = obj.m_ChildList.begin(); iter2 != iter2End; ++iter2)
	{
		CDx11GameObject*	pChild = (*iter2)->Clone();

		AddChild(pChild);
		SAFE_RELEASE(pChild);
	}
}

CDx11GameObject::~CDx11GameObject()
{
	Safe_Release_VecList(m_ChildList);
	SAFE_RELEASE(m_pTransform);
	Safe_Release_VecList(m_ComponentList);
}

CDx11GameObject * CDx11GameObject::Create(const string & strTag,
	OBJECT_CREATE_TYPE eOct)
{
	CDx11GameObject*	pObject = NULL;

	if (eOct != OCT_CLONE)
	{
		pObject = new CDx11GameObject;

		pObject->SetTag(strTag);

		if (!pObject->Init())
		{
			SAFE_RELEASE(pObject);
			return NULL;
		}
	}

	switch (eOct)
	{
	case OCT_PROTOTYPE:
		DX11_GET_SINGLE(CDx11PrototypeManager)->AddPrototype(strTag, pObject);
		break;

	case OCT_CLONE:
		pObject = DX11_GET_SINGLE(CDx11PrototypeManager)->CloneObject(strTag);
		pObject->SetTag(strTag);
		break;
	}

	return pObject;
}

CDx11GameObject * CDx11GameObject::Create(CDx11Scene * pScene,
	CDx11Layer * pLayer, const string & strTag,
	OBJECT_CREATE_TYPE eOct)
{
	CDx11GameObject*	pObject = NULL;

	if (eOct != OCT_CLONE)
	{
		pObject = new CDx11GameObject;

		pObject->SetTag(strTag);
		pObject->SetScene(pScene);
		pObject->SetLayer(pLayer);

		if (!pObject->Init())
		{
			SAFE_RELEASE(pObject);
			return NULL;
		}
	}

	switch (eOct)
	{
	case OCT_PROTOTYPE:
		DX11_GET_SINGLE(CDx11PrototypeManager)->AddPrototype(strTag, pObject);
		break;

	case OCT_CLONE:
		pObject = DX11_GET_SINGLE(CDx11PrototypeManager)->CloneObject(strTag);
		pObject->SetTag(strTag);
		pObject->SetScene(pScene);
		pObject->SetLayer(pLayer);
		break;
	}

	return pObject;
}

bool CDx11GameObject::IsCollision()
{
	return m_bCollide;
}

void CDx11GameObject::SetCollision(bool bColl)
{
	m_bCollide = bColl;
}

const list<class CDx11Component*>* CDx11GameObject::GetFindList()
{
	return &m_FindComponentList;
}

bool CDx11GameObject::GetGravity() const
{
	return m_bGravity;
}

float CDx11GameObject::GetGravityTime() const
{
	return m_fGravityTime;
}

void CDx11GameObject::ClearGravityTime()
{
	m_fGravityTime = 0.f;
}

void CDx11GameObject::EnableGravity(bool bGravity)
{
	m_bGravity = bGravity;
}

bool CDx11GameObject::Init()
{
	m_pTransform = new CDx11Transform;

	if (!m_pTransform->Init())
	{
		SAFE_RELEASE(m_pTransform);
		return false;
	}

	AddComponent(m_pTransform);

	return true;
}

int CDx11GameObject::Update(float fTime)
{
	list<CDx11Component*>::iterator	iter;
	list<CDx11Component*>::iterator	iterEnd = m_ComponentList.end();

	for (iter = m_ComponentList.begin(); iter != iterEnd; )
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

	// Child
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

//void CDx11GameObject::UpdateMatrix()
//{
//	m_pTransform->UpdateMatrix();
//
//	// Child
//	list<CDx11GameObject*>::iterator	iter1;
//	list<CDx11GameObject*>::iterator	iter1End = m_ChildList.end();
//
//	for (iter1 = m_ChildList.begin(); iter1 != iter1End;)
//	{
//		if (!(*iter1)->GetEnable())
//		{
//			++iter1;
//			continue;
//		}
//
//		(*iter1)->UpdateMatrix();
//
//		if (!(*iter1)->GetAlive())
//		{
//			SAFE_RELEASE((*iter1));
//			iter1 = m_ChildList.erase(iter1);
//		}
//
//		else
//			++iter1;
//	}
//}

int CDx11GameObject::LateUpdate(float fTime)
{
	// 중력을 적용한다.
	if (m_bGravity)
	{
		// 중력가속도를 구해준다.(등가속도)
		m_fGravityTime += fTime;
		float	fSpeed = -DX11_GET_SINGLE(CDx11Core)->GetGravity() *
			(m_fGravityTime * m_fGravityTime);

		m_pTransform->MoveWorld(VEC3Y * fSpeed);
	}

	list<CDx11Component*>::iterator	iter;
	list<CDx11Component*>::iterator	iterEnd = m_ComponentList.end();

	for (iter = m_ComponentList.begin(); iter != iterEnd; )
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

	// Child
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
	list<CDx11Component*>::iterator	iter;
	list<CDx11Component*>::iterator	iterEnd = m_ComponentList.end();

	for (iter = m_ComponentList.begin(); iter != iterEnd; )
	{
		if (!(*iter)->GetEnable())
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

	// Child
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

CDx11GameObject * CDx11GameObject::Clone()
{
	return new CDx11GameObject(*this);
}

void CDx11GameObject::AddRenderObject()
{
	DX11_GET_SINGLE(CDx11RenderManager)->AddObject(this);

	list<CDx11GameObject*>::iterator	iter1;
	list<CDx11GameObject*>::iterator	iter1End = m_ChildList.end();

	for (iter1 = m_ChildList.begin(); iter1 != iter1End; ++iter1)
	{
		(*iter1)->AddRenderObject();
	}
}

void CDx11GameObject::OnCollisionEnter(class CDx11Collider* pMy, class CDx11Collider* pYour, float fTime)
{
	list<CDx11Component*>::iterator	iter;
	list<CDx11Component*>::iterator	iterEnd = m_ComponentList.end();

	for (iter = m_ComponentList.begin(); iter != iterEnd; ++iter)
	{
		if((*iter)->GetComponentType() == CT_SCRIPT)
			(*iter)->OnCollisionEnter(pMy, pYour, fTime);
	}
}

void CDx11GameObject::OnCollision(class CDx11Collider* pMy, class CDx11Collider* pYour, float fTime)
{
	list<CDx11Component*>::iterator	iter;
	list<CDx11Component*>::iterator	iterEnd = m_ComponentList.end();

	for (iter = m_ComponentList.begin(); iter != iterEnd; ++iter)
	{
		(*iter)->OnCollision(pMy, pYour, fTime);
	}
}

void CDx11GameObject::OnCollisionExit(class CDx11Collider* pMy, class CDx11Collider* pYour, float fTime)
{
	list<CDx11Component*>::iterator	iter;
	list<CDx11Component*>::iterator	iterEnd = m_ComponentList.end();

	for (iter = m_ComponentList.begin(); iter != iterEnd; ++iter)
	{
		(*iter)->OnCollisionExit(pMy, pYour, fTime);
	}
}

void CDx11GameObject::Destroy()
{
	list<CDx11Component*>::iterator	iter;
	list<CDx11Component*>::iterator	iterEnd = m_ComponentList.end();

	for (iter = m_ComponentList.begin(); iter != iterEnd; ++iter)
	{
		(*iter)->Destroy();
	}

	m_ComponentList.clear();

	delete	this;
}

void CDx11GameObject::AddCollision()
{
	DX11_GET_SINGLE(CDx11CollisionManager)->AddCollisionObject(this);

	list<CDx11GameObject*>::iterator	iter;
	list<CDx11GameObject*>::iterator	iterEnd = m_ChildList.end();

	for (iter = m_ChildList.begin(); iter != iterEnd; ++iter)
	{
		(*iter)->AddCollision();
	}
}

void CDx11GameObject::SetParent(CDx11GameObject * pParent)
{
	m_pParent = pParent;
}

void CDx11GameObject::AddChild(CDx11GameObject * pChild)
{
	pChild->SetParent(this);
	pChild->SetScene(m_pScene);
	pChild->SetLayer(m_pLayer);
	pChild->AddRef();

	CDx11Transform*	pTransform = pChild->GetTransform();

	/*pTransform->ComputeWorldPos();*/

	pTransform->MoveWorld(Vec3(0.f, 0.f, 0.f));
	pTransform->RotateWorld(Vec3(0.f, 0.f, 0.f));


	SAFE_RELEASE(pTransform);

	m_ChildList.push_back(pChild);
}

void CDx11GameObject::DeleteChild(CDx11GameObject * pChild)
{
	list<CDx11GameObject*>::iterator	iter;
	list<CDx11GameObject*>::iterator	iterEnd = m_ChildList.end();

	for (iter = m_ChildList.begin(); iter != iterEnd; ++iter)
	{
		if (*iter == pChild)
		{
			pChild->SetParent(NULL);
			pChild->Release();
			m_ChildList.erase(iter);
			break;
		}
	}
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

void CDx11GameObject::AddComponent(CDx11Component * pComponent)
{
	pComponent->AddRef();
	pComponent->SetScene(m_pScene);
	pComponent->SetLayer(m_pLayer);
	pComponent->SetGameObject(this);
	pComponent->SetTransform(m_pTransform);
	m_ComponentList.push_back(pComponent);
}

CDx11Transform * CDx11GameObject::GetTransform()	const
{
	m_pTransform->AddRef();

	return m_pTransform;
}

CDx11Scene * CDx11GameObject::GetScene()	const
{
	m_pScene->AddRef();
	return m_pScene;
}

CDx11Layer * CDx11GameObject::GetLayer()	const
{
	m_pLayer->AddRef();
	return m_pLayer;
}

void CDx11GameObject::SetScene(CDx11Scene * pScene)
{
	list<CDx11Component*>::iterator	iter;
	list<CDx11Component*>::iterator	iterEnd = m_ComponentList.end();

	for (iter = m_ComponentList.begin(); iter != iterEnd; ++iter)
	{
		(*iter)->SetScene(pScene);
	}
	m_pScene = pScene;

	list<CDx11GameObject*>::iterator	iter1;
	list<CDx11GameObject*>::iterator	iter1End = m_ChildList.end();

	for (iter1 = m_ChildList.begin(); iter1 != iter1End; ++iter1)
	{
		(*iter1)->SetScene(pScene);
	}
}

void CDx11GameObject::SetLayer(CDx11Layer * pLayer)
{
	list<CDx11Component*>::iterator	iter;
	list<CDx11Component*>::iterator	iterEnd = m_ComponentList.end();

	for (iter = m_ComponentList.begin(); iter != iterEnd; ++iter)
	{
		(*iter)->SetLayer(pLayer);
	}
	m_pLayer = pLayer;

	list<CDx11GameObject*>::iterator	iter1;
	list<CDx11GameObject*>::iterator	iter1End = m_ChildList.end();

	for (iter1 = m_ChildList.begin(); iter1 != iter1End; ++iter1)
	{
		(*iter1)->SetLayer(pLayer);
	}
}

void CDx11GameObject::DeleteComponent(const string & strTag)
{
	list<CDx11Component*>::iterator	iter;
	list<CDx11Component*>::iterator	iterEnd = m_ComponentList.end();

	for (iter = m_ComponentList.begin(); iter != iterEnd; ++iter)
	{
		if ((*iter)->GetTag() == strTag)
		{
			SAFE_RELEASE((*iter));
			iter = m_ComponentList.erase(iter);
			break;
		}
	}
}

CDx11Component * CDx11GameObject::FindComponentFromTag(const string & strTag)	const
{
	list<CDx11Component*>::const_iterator	iter;
	list<CDx11Component*>::const_iterator	iterEnd = m_ComponentList.end();

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

CDx11Component * CDx11GameObject::FindComponentFromTypeName(const string & strTypeName)	const
{
	list<CDx11Component*>::const_iterator	iter;
	list<CDx11Component*>::const_iterator	iterEnd = m_ComponentList.end();

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

CDx11Component * CDx11GameObject::FindComponentFromType(COMPONENT_TYPE eType)	const
{
	list<CDx11Component*>::const_iterator	iter;
	list<CDx11Component*>::const_iterator	iterEnd = m_ComponentList.end();

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

const list<CDx11Component*>* CDx11GameObject::FindComponentsFromTag(const string & strTag)
{
	return NULL;
}

const list<CDx11Component*>* CDx11GameObject::FindComponentsFromTypeName(const string & strTypeName)
{
	m_FindComponentList.clear();

	list<CDx11Component*>::iterator	iter;
	list<CDx11Component*>::iterator	iterEnd = m_ComponentList.end();

	for (iter = m_ComponentList.begin(); iter != iterEnd; ++iter)
	{
		if ((*iter)->GetTypeName() == strTypeName)
		{
			m_FindComponentList.push_back(*iter);
		}
	}

	return &m_FindComponentList;
}

const list<CDx11Component*>* CDx11GameObject::FindComponentsFromType(COMPONENT_TYPE eType)
{
	m_FindComponentList.clear();

	list<CDx11Component*>::iterator	iter;
	list<CDx11Component*>::iterator	iterEnd = m_ComponentList.end();

	for (iter = m_ComponentList.begin(); iter != iterEnd; ++iter)
	{
		if ((*iter)->GetComponentType() == eType)
		{
			m_FindComponentList.push_back(*iter);
		}
	}

	return &m_FindComponentList;
}

list<CDx11Component*>* CDx11GameObject::FindComponentsFromType_NonConst(COMPONENT_TYPE eType)
{
	m_FindComponentList.clear();

	list<CDx11Component*>::iterator	iter;
	list<CDx11Component*>::iterator	iterEnd = m_ComponentList.end();

	for (iter = m_ComponentList.begin(); iter != iterEnd; ++iter)
	{
		if ((*iter)->GetComponentType() == eType)
		{
			m_FindComponentList.push_back(*iter);
		}
	}

	return &m_FindComponentList;
}

CDx11GameObject * CDx11GameObject::FindChildFromTag(const string & strTag)	const
{
	list<CDx11GameObject*>::const_iterator	iter;
	list<CDx11GameObject*>::const_iterator	iterEnd = m_ChildList.end();

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

CDx11GameObject * CDx11GameObject::FindChildFromComponentTypeName(const string & strName)	const
{
	list<CDx11GameObject*>::const_iterator	iter;
	list<CDx11GameObject*>::const_iterator	iterEnd = m_ChildList.end();

	for (iter = m_ChildList.begin(); iter != iterEnd; ++iter)
	{
		CDx11Component*	pCom = (*iter)->FindComponentFromTypeName(strName);
		if (pCom)
		{
			SAFE_RELEASE(pCom);
			(*iter)->AddRef();
			return *iter;
		}
	}

	return NULL;
}
