#include "Dx11Layer.h"
#include "../GameObject/Dx11GameObject.h"
#include "../Rendering/Dx11RenderManager.h"
#include "../Core/Dx11CollisionManager.h"

DX11_USING

CDx11Layer::CDx11Layer()
{
	SetTypeName<CDx11Layer>();
}

CDx11Layer::~CDx11Layer()
{
	Safe_Release_VecList(m_ObjList);
	Safe_Release_VecList(m_FindList);
}


CDx11GameObject * CDx11Layer::FindGameObjectFromTag(const string & strTag)
{
	list<CDx11GameObject*>::iterator	iter;
	list<CDx11GameObject*>::iterator	iterEnd = m_ObjList.end();

	for (iter = m_ObjList.begin(); iter != iterEnd; ++iter)
	{
		if ((*iter)->GetTag() == strTag)
		{
			(*iter)->AddRef();
			return *iter;
		}
	}
	return NULL;
}

list<CDx11GameObject*>* CDx11Layer::FindGameObjectsFromTag(const string & strTag)
{
	Safe_Release_VecList(m_FindList);

	list<CDx11GameObject*>::iterator	iter;
	list<CDx11GameObject*>::iterator	iterEnd = m_ObjList.end();

	for (iter = m_ObjList.begin(); iter != iterEnd; ++iter)
	{
		if ((*iter)->GetTag() == strTag)
		{
			(*iter)->AddRef();
			m_FindList.push_back(*iter);
		}
	}

	return &m_FindList;
}

void CDx11Layer::SetScene(CDx11Scene * pScene)
{
	m_pScene = pScene;
}

void CDx11Layer::AddObject(CDx11GameObject * pObj)
{
	pObj->AddRef();
	pObj->SetScene(m_pScene);
	pObj->SetLayer(this);
	m_ObjList.push_back(pObj);
}

bool CDx11Layer::Init()
{
	return true;
}

void CDx11Layer::Input(float fTime)
{
	list<CDx11GameObject*>::iterator	iter;
	list<CDx11GameObject*>::iterator	iterEnd = m_ObjList.end();

	for (iter = m_ObjList.begin(); iter != iterEnd;)
	{
		if (!(*iter)->GetEnable())
		{
			++iter;
			continue;
		}

		(*iter)->Input(fTime);

		if (!(*iter)->GetAlive())
		{
			SAFE_RELEASE((*iter));
			iter = m_ObjList.erase(iter);
		}

		else
			++iter;
	}

}

int CDx11Layer::Update(float fTime)
{
	list<CDx11GameObject*>::iterator	iter;
	list<CDx11GameObject*>::iterator	iterEnd = m_ObjList.end();

	for (iter = m_ObjList.begin(); iter != iterEnd;)
	{
		if (!(*iter)->GetEnable())
		{
			++iter;
			continue;
		}

		(*iter)->Update(fTime);

		if (!(*iter)->GetAlive())
		{
			SAFE_RELEASE((*iter));
			iter = m_ObjList.erase(iter);
//			list에서는 erase()함수가 호출되면 해당 *iter를 지우고 
//			다음 iter를 가리키게된다.
		}
		else
			++iter;
	}

	return 0;
}

int CDx11Layer::LateUpdate(float fTime)
{
	list<CDx11GameObject*>::iterator	iter;
	list<CDx11GameObject*>::iterator	iterEnd = m_ObjList.end();

	for (iter = m_ObjList.begin(); iter != iterEnd;)
	{
		if (!(*iter)->GetEnable())
		{
			++iter;
			continue;
		}

		(*iter)->LateUpdate(fTime);

		if (!(*iter)->GetAlive())
		{
			SAFE_RELEASE((*iter));
			iter = m_ObjList.erase(iter);
		}
		else
			++iter;
	}
	return 0;
}

void CDx11Layer::Collision(float fTime)
{
	list<CDx11GameObject*>::iterator	iter;
	list<CDx11GameObject*>::iterator	iterEnd = m_ObjList.end();

	for (iter = m_ObjList.begin(); iter != iterEnd; ++iter)
	{
//		DX11_GET_SINGLE(CDx11CollisionManager)->AddGameObject(*iter);
		(*iter)->AddCollider();
	}
}

void CDx11Layer::Render(float fTime)
{
	list<CDx11GameObject*>::iterator	iter;
	list<CDx11GameObject*>::iterator	iterEnd = m_ObjList.end();

	for (iter = m_ObjList.begin(); iter != iterEnd;)
	{
		if (!(*iter)->GetEnable())
		{
			++iter;
			continue;
		}

		if (!(*iter)->GetAlive())
		{
			SAFE_RELEASE((*iter));
			iter = m_ObjList.erase(iter);
		}
		else
		{
			// 그려져야 하는 모든 오브젝트는 AddRenderObject
			(*iter)->AddRenderObject();
//			DX11_GET_SINGLE(CDx11RenderManager)->AddRenderObject(*iter);
//			DX11_GET_SINGLE(CDx11RenderManager)->Render(fTime);
			++iter;
		}
	}
}
