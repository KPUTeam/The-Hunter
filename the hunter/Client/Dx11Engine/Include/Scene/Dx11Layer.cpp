#include "Dx11Layer.h"
#include "../GameObject/Dx11GameObject.h"
#include "../Scene/Dx11Scene.h"
#include "../Core/Dx11CollisionManager.h"
#include "../Rendering/Dx11RenderManager.h"

DX11_USING

CDx11Layer::CDx11Layer()
{
	SetTypeName<CDx11Layer>();
}

CDx11Layer::~CDx11Layer()
{
	Safe_Release_VecList(m_ObjList);
}

bool CDx11Layer::Init()
{
	return true;
}

int CDx11Layer::Update(float fTime)
{
	list<CDx11GameObject*>::iterator	iter;
	list<CDx11GameObject*>::iterator	iterEnd = m_ObjList.end();

	for (iter = m_ObjList.begin(); iter != iterEnd; )
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
		}

		else
			++iter;
	}

	return 0;
}

//void CDx11Layer::UpdateMatrix()
//{
//	list<CDx11GameObject*>::iterator	iter;
//	list<CDx11GameObject*>::iterator	iterEnd = m_ObjList.end();
//
//	for (iter = m_ObjList.begin(); iter != iterEnd; )
//	{
//		if (!(*iter)->GetEnable())
//		{
//			++iter;
//			continue;
//		}
//
//		(*iter)->UpdateMatrix();
//
//		if (!(*iter)->GetAlive())
//		{
//			SAFE_RELEASE((*iter));
//			iter = m_ObjList.erase(iter);
//		}
//
//		else
//			++iter;
//	}
//}

int CDx11Layer::LateUpdate(float fTime)
{
	list<CDx11GameObject*>::iterator	iter;
	list<CDx11GameObject*>::iterator	iterEnd = m_ObjList.end();

	for (iter = m_ObjList.begin(); iter != iterEnd; )
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

void CDx11Layer::AddCollision()
{
	list<CDx11GameObject*>::iterator	iter;
	list<CDx11GameObject*>::iterator	iterEnd = m_ObjList.end();

	for (iter = m_ObjList.begin(); iter != iterEnd; ++iter)
	{
		if (!(*iter)->GetEnable())
			continue;

		if (!(*iter)->IsCollision())
			continue;

		//DX11_GET_SINGLE(CDx11CollisionManager)->AddCollisionObject(*iter);
		(*iter)->AddCollision();
	}
}

void CDx11Layer::Render(float fTime)
{
	list<CDx11GameObject*>::iterator	iter;
	list<CDx11GameObject*>::iterator	iterEnd = m_ObjList.end();

	for (iter = m_ObjList.begin(); iter != iterEnd; ++iter)
	{
		if (!(*iter)->GetEnable())
		{
			continue;
		}

		(*iter)->AddRenderObject();

		//DX11_GET_SINGLE(CDx11RenderManager)->AddObject(*iter);
		//(*iter)->Render(fTime);
	}

}

void CDx11Layer::AddObject(CDx11GameObject * pGameObject)
{
	pGameObject->AddRef();
	pGameObject->SetScene(m_pScene);
	pGameObject->SetLayer(this);

	m_ObjList.push_back(pGameObject);
}

void CDx11Layer::SetScene(CDx11Scene * pScene)
{
	m_pScene = pScene;
}

CDx11Scene * CDx11Layer::GetScene()
{
	m_pScene->AddRef();
	return m_pScene;
}

CDx11GameObject* CDx11Layer::FindObjectFromTag(const string& strTag)
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

void CDx11Layer::Destroy()
{
	list<CDx11GameObject*>::iterator	iter;
	list<CDx11GameObject*>::iterator	iterEnd = m_ObjList.end();

	for (iter = m_ObjList.begin(); iter != iterEnd; ++iter)
	{
		(*iter)->Destroy();
	}

	m_ObjList.clear();
}
