#include "Dx11CollisionManager.h"
#include "../Component/Dx11Collider.h"
#include "../GameObject/Dx11GameObject.h"

DX11_USING

DX11_DEFINITION_SINGLE(CDx11CollisionManager)

CDx11CollisionManager::CDx11CollisionManager()
{
	// 충돌체 렌더링 부울변수
	m_bColliderRender = false;

#if defined(DEBUG) | defined(_DEBUG)
	m_bColliderRender = true;
#endif


}


CDx11CollisionManager::~CDx11CollisionManager()
{
}

bool CDx11CollisionManager::Init()
{
	m_ObjList.reserve(1000);

	return true;
}

void CDx11CollisionManager::AddGameObject(CDx11GameObject * pObj)
{
//	충돌 컴포넌트가 없으면 제외시킨다.
	CDx11Component* pColl = pObj->FindComponentFromType(CT_COLLIDER);
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

	// 버블 정렬 방식으로 모든 오브젝트들의 충돌 여부를 모두 검사한다.

	vector<CDx11GameObject*>::iterator	iter1;
	vector<CDx11GameObject*>::iterator	iter1End = m_ObjList.end() - 1;

	for (iter1 = m_ObjList.begin(); iter1 != iter1End; ++iter1)
	{
		vector<CDx11GameObject*>::iterator	iter2;
		vector<CDx11GameObject*>::iterator	iter2End = m_ObjList.end();

		for (iter2 = iter1 + 1; iter2 != iter2End; ++iter2)
		{
		//  두 오브젝트씩 검사
			CheckCollision(*iter1, *iter2, fTime);
		}
	}

	m_ObjList.clear();
	m_ObjList.reserve(1000);
}

void CDx11CollisionManager::SetColliderRender(bool bEnable)
{
	m_bColliderRender = bEnable;

}

void CDx11CollisionManager::ColliderRenderEnable()
{
	m_bColliderRender = !m_bColliderRender;
}

bool CDx11CollisionManager::GetColliderRender()
{
	return m_bColliderRender;
}

bool CDx11CollisionManager::CheckCollision(CDx11GameObject * pObj1, CDx11GameObject * pObj2, float fTime)
{
	// 각 오브젝트가 가지고 있는 모든 충돌 컴포넌트를 받아온다.

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
			CDx11Collider* pC1 = (CDx11Collider*)*iter1;
			CDx11Collider* pC2 = (CDx11Collider*)*iter2;
			
			// 충돌이 됬을 경우
			if (pC1->Collision(pC2))
			{
				// 충돌체가 없다면 추가해주고,
				// 충돌 시작함수를 호출한다.
				if (!pC1->CheckCollider(pC2))
				{
					pC1->AddCollider(pC2);
					pC2->AddCollider(pC1);

					pObj1->OnCollisionEnter(pC2, fTime);
					pObj2->OnCollisionEnter(pC1, fTime);
				}
				// 충돌체가 있다면 충돌 중 함수를 호출한다.
				else
				{
					pObj1->OnCollision(pC2, fTime);
					pObj2->OnCollision(pC1, fTime);
				}
			}
			// 충돌이 안됬을 경우
			else
			{
				// 있다면 지워준다.
				if (pC1->CheckCollider(pC2))
				{
					pC1->EraseCollider(pC2);
					pC2->EraseCollider(pC1);

					pObj1->OnCollisionExit(pC2, fTime);
					pObj2->OnCollisionExit(pC1, fTime);
				}
			}

		}
	}

	return true;
}
