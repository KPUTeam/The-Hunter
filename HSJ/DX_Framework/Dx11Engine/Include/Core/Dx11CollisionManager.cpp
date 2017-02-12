#include "Dx11CollisionManager.h"
#include "../Component/Dx11Collider.h"
#include "../GameObject/Dx11GameObject.h"

DX11_USING

DX11_DEFINITION_SINGLE(CDx11CollisionManager)

CDx11CollisionManager::CDx11CollisionManager()
{
}

CDx11CollisionManager::~CDx11CollisionManager()
{
}

bool CDx11CollisionManager::Init()
{
	m_ObjList.reserve(200);

	return true;
}

void CDx11CollisionManager::AddGameObject(CDx11GameObject * pObj)
{
	CDx11Component*	pColl = pObj->FindComponentFromType(CT_COLLIDER);

	if (!pColl)
		return;

	m_ObjList.push_back(pObj);

	SAFE_RELEASE(pColl);
}

void CDx11CollisionManager::Collision(float fTime)
{
	if (m_ObjList.size() < 2)
	{
		m_ObjList.clear();
		return;
	}

	vector<CDx11GameObject*>::iterator	iter;
	vector<CDx11GameObject*>::iterator	iterEnd = m_ObjList.end() - 1;

	for (iter = m_ObjList.begin(); iter != iterEnd; ++iter)
	{
		vector<CDx11GameObject*>::iterator	iter1;
		vector<CDx11GameObject*>::iterator	iter1End = m_ObjList.end();
		
		for (iter1 = iter + 1; iter1 != iter1End; ++iter1)
		{
			Collision(*iter, *iter1);
		}
	}

	m_ObjList.clear();
}

bool CDx11CollisionManager::Collision(CDx11GameObject * pObj1, 
	CDx11GameObject * pObj2)
{
	list<CDx11Component*>*	pColl1 = pObj1->FindComponentsFromType(CT_COLLIDER);
	list<CDx11Component*>*	pColl2 = pObj2->FindComponentsFromType(CT_COLLIDER);

	list<CDx11Component*>::iterator	iter1;
	list<CDx11Component*>::iterator	iter1End = pColl1->end();

	list<CDx11Component*>::iterator	iter2;
	list<CDx11Component*>::iterator	iter2End = pColl2->end();

	for (iter1 = pColl1->begin(); iter1 != iter1End; ++iter1)
	{
		for (iter2 = pColl2->begin(); iter2 != iter2End; ++iter2)
		{
			CDx11Collider*	pColl1 = (CDx11Collider*)*iter1;
			CDx11Collider*	pColl2 = (CDx11Collider*)*iter2;

			if (pColl1->Collision(pColl2))
			{
				MessageBox(0, L"Coll", L"Coll", MB_OK);
			}
		}
	}

	return true;
}
