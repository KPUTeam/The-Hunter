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

//	���� ����ó�����ϴ� ���������
CDx11GameObject::CDx11GameObject(const CDx11GameObject & obj)
{
//	�ܼ��� �� ����
	*this = obj;

//	���� "���� ����"�� �Ǿ� �ִ� ������Ʈ ����Ʈ�� ���
	m_ComponentList.clear();

	list<class CDx11Component*>::const_iterator iter;
	list<class CDx11Component*>::const_iterator iterEnd = obj.m_ComponentList.end();

//	������Ʈ�� ��� ���� �����Ѵ�.
	for (iter = obj.m_ComponentList.begin(); iter != iterEnd; ++iter)
	{
		CDx11Component* pCom = (*iter)->Clone();
		// Ʈ�������� ���� ���ϰ� �����ص�
		if (pCom->GetComponentType() == CT_TRANSFORM)
			m_pTransform = (CDx11Transform*)pCom;

		pCom->SetTransform(m_pTransform);
		pCom->SetGameObject(this);
		pCom->SetLayer(m_pLayer);
		pCom->SetScene(m_pScene);
		m_ComponentList.push_back(pCom);
	}

	m_pTransform->AddRef();

// ���ϵ� ����
	m_ChildList.clear();

	list<CDx11GameObject*>::const_iterator	iter1;
	list<CDx11GameObject*>::const_iterator	iter1End = obj.m_ChildList.end();

	for (iter1 = obj.m_ChildList.begin(); iter1 != iter1End; ++iter1)
	{
	//	���ϵ� ����Ʈ�� ��ȸ�ϸ鼭 ��� ���ϵ带 �������ش�.
		CDx11GameObject*	pChild = (*iter1)->Clone();

	//	�θ� ���� �Ȱ��� ����
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
	//	�븻�̸� �׳� �Ҵ�����
		pObj = new CDx11GameObject;
		if (!pObj->Init())
		{
			SAFE_RELEASE(pObj);
			return NULL;
		}
		break;
	case OT_PROTOTYPE:
	//	������Ÿ���� ��� �޸� �Ҵ����ְ�,
		pObj = new CDx11GameObject;
		if (!pObj->Init())
		{
			SAFE_RELEASE(pObj);
			return NULL;
		}
	//	������Ʈ �Ŵ����� AddPrototype�Լ��� ȣ���Ͽ� ������� ������Ÿ���� �߰�����.
		if (DX11_GET_SINGLE(CDx11GameObjectManager)->AddPrototype(pObj, strTag))
			pObj->Release();
		break;
	case OT_CLONE:
	//	Ŭ���� ��� ������Ʈ �Ŵ������� CreateObject �Լ��� ȣ��
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
//	�θ� ������ NULL�� ����
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
	// Ʈ�������� ��� ���ӿ�����Ʈ�� �ݵ�� ������ �־���Ѵ�.
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
	//this�� �������Ͽ� ��������ڿ� ���� �ְ� new�� ���� �Ҵ�
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
		//�±׷� ������Ʈ�� ã�´�.
		if ((*iter)->GetTag() == strTag)
		{
			//ã������ ����ī��Ʈ�� 1 ������Ű�� �ش� iterator�� ����
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
		//Ÿ�Գ������� ������Ʈ�� ã�´�.
		if ((*iter)->GetTypeName() == strTypeName)
		{
			//ã������ ����ī��Ʈ�� 1 ������Ű�� �ش� iterator�� ����
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
		//Ÿ�Գ������� ������Ʈ�� ã�´�.
		if ((*iter)->GetComponentType() == eType)
		{
			//ã������ ����ī��Ʈ�� 1 ������Ű�� �ش� iterator�� ����
			(*iter)->AddRef();
			return *iter;
		}
	}
	return NULL;
}

list<CDx11Component*>* CDx11GameObject::FindComponentsFromTag(const string & strTag)
{
//	���� �̸��� ���� �������� ������Ʈ�� �ѹ��� ã�� �Լ��̴�.
	list<CDx11Component*>::iterator iter;
	list<CDx11Component*>::iterator iterEnd = m_FindList.end();

//	�켱 FindList�� ����ش�.
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

	// ���� ����Ʈ�� �Ѱ��ָ� �ʹ� �����Ƿ� �����͸� �Ѱ��ش�.
	return &m_FindList;
}

list<CDx11Component*>* CDx11GameObject::FindComponentsFromTypeName(const string & strTypeName)
{
	//	���� �̸��� ���� �������� ������Ʈ�� �ѹ��� ã�� �Լ��̴�.
	list<CDx11Component*>::iterator iter;
	list<CDx11Component*>::iterator iterEnd = m_FindList.end();

	//	�켱 FindList�� ����ش�.
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

	// ���� ����Ʈ�� �Ѱ��ָ� �ʹ� �����Ƿ� �����͸� �Ѱ��ش�.
	return &m_FindList;
}

list<CDx11Component*>* CDx11GameObject::FindComponentsFromType(COMPONENT_TYPE eType)
{
	//	���� �̸��� ���� �������� ������Ʈ�� �ѹ��� ã�� �Լ��̴�.
	list<CDx11Component*>::iterator iter;
	list<CDx11Component*>::iterator iterEnd = m_FindList.end();

	//	�켱 FindList�� ����ش�.
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

	// ���� ����Ʈ�� �Ѱ��ָ� �ʹ� �����Ƿ� �����͸� �Ѱ��ش�.
	return &m_FindList;
}

CDx11GameObject * CDx11GameObject::FindChildFromTag(const string & strTag)
{
	list<CDx11GameObject*>::iterator iter;
	list<CDx11GameObject*>::iterator iterEnd = m_ChildList.end();

	for (iter = m_ChildList.begin(); iter != iterEnd; ++iter)
	{
		//�±׷� ������Ʈ�� ã�´�.
		if ((*iter)->GetTag() == strTag)
		{
			//ã������ ����ī��Ʈ�� 1 ������Ű�� �ش� iterator�� ����
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
		//Ÿ�Գ������� ������Ʈ�� ã�´�.
		if ((*iter)->GetTypeName() == strTypeName)
		{
			//ã������ ����ī��Ʈ�� 1 ������Ű�� �ش� iterator�� ����
			(*iter)->AddRef();
			return *iter;
		}
	}
	return NULL;
}

void CDx11GameObject::AddRenderObject()
{
	// Layer���� RenderManager�� ������Ʈ�� �߰����� �ʰ�, 
	// GameObject���� ���������� �߰�����
	// ���� Child�� ���ؼ��� ó���� �� �� �� �ֵ�.

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
//	���ϵ�� �θ�� ���� ������ ���´�.
	pChild->AddRef();
	pChild->SetScene(m_pScene);
	pChild->SetLayer(m_pLayer);
	pChild->SetParent(this);
	m_ChildList.push_back(pChild);
}

void CDx11GameObject::AddComponent(CDx11Component * pComponent)
{
//	���� ī��Ʈ 1 ����
	pComponent->AddRef();
//	���� ���̾�, ���ӿ�����Ʈ�� ��
	pComponent->SetScene(m_pScene);
	pComponent->SetLayer(m_pLayer);
	pComponent->SetGameObject(this);
	pComponent->SetTransform(m_pTransform);

//	������Ʈ ����Ʈ�� �߰�
	m_ComponentList.push_back(pComponent);
}

