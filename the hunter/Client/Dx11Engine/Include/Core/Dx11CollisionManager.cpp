#include "Dx11CollisionManager.h"
#include "../GameObject/Dx11GameObject.h"
#include "../Component/Dx11Collider.h"
#include "Dx11Input.h"

DX11_USING

DX11_SINGLE_DEFINITION(CDx11CollisionManager)

#define MaxPlayer	 10
#define MaxMonster 300
#define MaxObj			100
#define MaxBullet		100
#define MaxStatic		500

CDx11CollisionManager::CDx11CollisionManager()
{
	m_bColliderRender = false;

#if defined(DEBUG) | defined(_DEBUG1)
	m_bColliderRender = true;
#endif

}

CDx11CollisionManager::~CDx11CollisionManager()
{
	Safe_Release_VecList(m_PlayerList);
	Safe_Release_VecList(m_MonsterList);
	Safe_Release_VecList(m_ObjList);
	Safe_Release_VecList(m_BulletList);
	Safe_Release_VecList(m_StaticList);
}

bool CDx11CollisionManager::Init()
{
	m_PlayerList.reserve(MaxPlayer);
	m_MonsterList.reserve(MaxMonster);
	m_ObjList.reserve(MaxObj);
	m_BulletList.reserve(MaxObj);
	m_StaticList.reserve(MaxStatic);

	return true;
}

void CDx11CollisionManager::Input(float fTime)
{
	if (DX11_GET_SINGLE(CDx11Input)->KeyDown(VISIBILLTY_COLLIDER))
	{
		ColliderRenderEnable();
	}
}

void CDx11CollisionManager::AddCollisionObject(CDx11GameObject * pObj)
{
	// ���� ������Ʈ�� Ÿ���� �˻����� �˸��� ����Ʈ�� �־��ش�.
	// �浹ü�� �ϳ��� ã�Ƶ� ���ӿ�����Ʈ�� �ִ´�.

	CDx11Component*	pColl = pObj->FindComponentFromType(CT_COLLIDER);

	if (!pColl)
		return;

	string strTag = pColl->GetTag();

	if (strTag == MONSTER_VIEW)
		m_MonsterList.push_back(pObj);

	else if (strTag == MONSTER_BODY)
		m_MonsterList.push_back(pObj);

	else if (strTag == MONSTER_ATTACK_BOX)
		m_MonsterList.push_back(pObj);

	else if (strTag == PLAYER_BODY)
		m_PlayerList.push_back(pObj);

	else if (strTag == PLAYER_VIEW)
		m_PlayerList.push_back(pObj);

	else if (strTag == BULLET_BODY)
		m_BulletList.push_back(pObj);

	else if (strTag == STATIC_MESH)
		m_StaticList.push_back(pObj);

	else
		m_ObjList.push_back(pObj);

	SAFE_RELEASE(pColl);
}

void CDx11CollisionManager::Collision(float fTime)
{
	// �÷��̾�� ����ƽ �Ž�
	vector<CDx11GameObject*>::iterator	iterPlayer;
	vector<CDx11GameObject*>::iterator	iterPlayerEnd = m_PlayerList.end();

	vector<CDx11GameObject*>::iterator	iterStatic;
	vector<CDx11GameObject*>::iterator	iterStaticEnd = m_StaticList.end();

	if (m_PlayerList.size() != 0 && m_StaticList.size() != 0)
		for (iterPlayer = m_PlayerList.begin(); iterPlayer != iterPlayerEnd; ++iterPlayer)
		{
			for (iterStatic = m_StaticList.begin(); iterStatic != iterStaticEnd; ++iterStatic)
			{
				Collision(fTime, *iterPlayer, *iterStatic);
			}
		}

	// �Ѿ˰� ����ƽ �Ž�
	vector<CDx11GameObject*>::iterator	iterBullet;
	vector<CDx11GameObject*>::iterator	iterBulletEnd = m_BulletList.end();

	if (m_BulletList.size() != 0 && m_StaticList.size() != 0)
		for (iterBullet = m_BulletList.begin(); iterBullet != iterBulletEnd; ++iterBullet)
		{
			for (iterStatic = m_StaticList.begin(); iterStatic != iterStaticEnd; ++iterStatic)
			{
				Collision(fTime, *iterBullet, *iterStatic);
			}
		}

	// �Ѿ˰� �÷��̾�
	if (m_PlayerList.size() != 0 && m_BulletList.size() != 0)
		for (iterPlayer = m_PlayerList.begin(); iterPlayer != iterPlayerEnd; ++iterPlayer)
		{
			for (iterBullet = m_BulletList.begin(); iterBullet != iterBulletEnd; ++iterBullet)
			{
				Collision(fTime, *iterPlayer, *iterBullet);
			}
		}

	// �Ѿ˰� ����
	vector<CDx11GameObject*>::iterator	iterMonster;
	vector<CDx11GameObject*>::iterator	iterMonsterEnd = m_MonsterList.end();

	if (m_BulletList.size() != 0 && m_MonsterList.size() != 0)
		for (iterBullet = m_BulletList.begin(); iterBullet != iterBulletEnd; ++iterBullet)
		{
			for (iterMonster = m_MonsterList.begin(); iterMonster != iterMonsterEnd; ++iterMonster)
			{
				Collision(fTime, *iterBullet, *iterMonster);
			}
		}

	// �÷��̾�� ����
	if (m_PlayerList.size() != 0 && m_MonsterList.size() != 0)
		for (iterPlayer = m_PlayerList.begin(); iterPlayer != iterPlayerEnd; ++iterPlayer)
		{
			for (iterMonster = m_MonsterList.begin(); iterMonster != iterMonsterEnd; ++iterMonster)
			{
				Collision(fTime, *iterPlayer, *iterMonster);
			}
		}


	//// �÷��̾�� �Ѿ�
	//vector<CDx11GameObject*>::iterator	iterBullet;
	//vector<CDx11GameObject*>::iterator	iterBulletEnd = m_BulletList.end();

	//if (m_PlayerList.size() != 0 && m_BulletList.size() != 0)
	//for (iterPlayer = m_PlayerList.begin(); iterPlayer != iterPlayerEnd; ++iterPlayer)
	//{
	//	for (iterBullet = m_BulletList.begin(); iterBullet != iterBulletEnd; ++iterBullet)
	//	{
	//		Collision(fTime, *iterPlayer, *iterBullet);
	//	}
	//}

	//if (m_MonsterList.size() != 0 && m_BulletList.size() != 0)
	//for (iterMonster = m_MonsterList.begin(); iterMonster != iterMonsterEnd; ++iterMonster)
	//{
	//	for (iterBullet = m_BulletList.begin(); iterBullet != iterBulletEnd; ++iterBullet)
	//	{
	//		Collision(fTime, *iterMonster, *iterBullet);
	//	}
	//}

	//// ������Ʈ ������ �浹
	//// ���� ���� ����� ����Ѵ�.
	//if (m_ObjList.size() > 1)
	//{
	//	vector<CDx11GameObject*>::iterator	iter1;
	//	vector<CDx11GameObject*>::iterator	iter1End = m_ObjList.end() - 1;

	//	vector<CDx11GameObject*>::iterator	iter2;
	//	vector<CDx11GameObject*>::iterator	iter2End = m_ObjList.end();

	//	for (iter1 = m_ObjList.begin(); iter1 != iter1End; ++iter1)
	//	{
	//		for (iter2 = iter1 + 1; iter2 != iter2End; ++iter2)
	//		{
	//			//  �� ������Ʈ�� �˻�
	//			// Collision(fTime, *iter1, *iter2);
	//		}
	//	}
	//}

	// ������
	m_BulletList.clear();
	m_BulletList.reserve(MaxBullet);

	m_PlayerList.clear();
	m_PlayerList.reserve(MaxPlayer);

	m_MonsterList.clear();
	m_MonsterList.reserve(MaxMonster);

	m_ObjList.clear();
	m_ObjList.reserve(MaxObj);

	m_StaticList.clear();
	m_StaticList.reserve(MaxStatic);
}

void CDx11CollisionManager::SetColliderRenderEnable(bool bEnable)
{
#if defined(DEBUG) | defined(_DEBUG1)
	m_bColliderRender = bEnable;
#endif
}

void CDx11CollisionManager::ColliderRenderEnable()
{
	m_bColliderRender = !m_bColliderRender;
}

bool CDx11CollisionManager::GetColliderRender()
{
	return m_bColliderRender;
}

void CDx11CollisionManager::Collision(float fTime, CDx11GameObject* pObj1, CDx11GameObject* pObj2)
{
	list<CDx11Component*>*	pCollList1 = pObj1->FindComponentsFromType_NonConst(CT_COLLIDER);
	list<CDx11Component*>::iterator	iter1;
	list<CDx11Component*>::iterator	iter1End = pCollList1->end();

	list<CDx11Component*>*	pCollList2 = pObj2->FindComponentsFromType_NonConst(CT_COLLIDER);

	list<CDx11Component*>::iterator	iter2;
	list<CDx11Component*>::iterator	iter2End = pCollList2->end();

	for (iter1 = pCollList1->begin(); iter1 != iter1End; ++iter1)
	{
		for (iter2 = pCollList2->begin(); iter2 != iter2End; ++iter2)
		{
			CDx11Collider*	pColl1 = (CDx11Collider*) (*iter1);
			CDx11Collider*	pColl2 = (CDx11Collider*) (*iter2);

			// �浹?
			if (pColl1->Collision(pColl2) || pColl2->Collision(pColl1))
			{
				pColl1->ColorEnable();
				pColl2->ColorEnable();

				// ���� ��Ͽ� �ֳ�? ������,
				if (!pColl1->CollsionInCollider(pColl2))
				{
					pColl1->AddCollision(pColl2);
					pObj1->OnCollisionEnter(pColl1, pColl2, fTime);

					pColl2->AddCollision(pColl1);
					pObj2->OnCollisionEnter(pColl2, pColl1, fTime);
				}

				// ������,
				else
				{
					pObj1->OnCollision(pColl1, pColl2, fTime);
					pObj2->OnCollision(pColl2, pColl1, fTime);
				}
			}

			// �浹�� ���� �ʾ��� ��,
			else
			{
				// ���� ��Ͽ� �־�����
				if (pColl1->CollsionInCollider(pColl2))
				{
					pColl1->EraseCollision(pColl2);
					pObj1->OnCollisionExit(pColl1, pColl2, fTime);

					pColl2->EraseCollision(pColl1);
					pObj2->OnCollisionExit(pColl2, pColl1, fTime);
				}
			}
		}
	}
}