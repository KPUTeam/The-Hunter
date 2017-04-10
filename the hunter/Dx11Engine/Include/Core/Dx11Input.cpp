#include "Dx11Input.h"
#include "../GameObject/Dx11GameObject.h"
#include "../Component/Dx11Mouse.h"
#include "../Component/Dx11ColliderPoint.h"
#include "Dx11CollisionManager.h"
#include "../Component/Dx11Transform.h"

DX11_USING

DX11_DEFINITION_SINGLE(CDx11Input)

CDx11Input::CDx11Input()	:
	m_pCreateKey(NULL),
	m_pMouseObj(NULL),
	m_pMouse(NULL),
	m_bActivated(true)
{
}


CDx11Input::~CDx11Input()
{
	SAFE_RELEASE(m_pMouse);
	SAFE_RELEASE(m_pMouseObj);
	Safe_Delete_Map(m_mapKey);
}

POINT CDx11Input::GetMousePoint() const
{
	return m_ptMouse;
}

CDx11GameObject * CDx11Input::GetMouseObj() const
{
	return m_pMouseObj;
}

CDx11Mouse * CDx11Input::GetMouse() const
{
	m_pMouse->AddRef();
	return m_pMouse;
}

bool CDx11Input::Init(HWND hWnd)
{
	// 마우스 설정
	m_pMouseObj = CDx11GameObject::Create("Mouse");
	m_pMouse = m_pMouseObj->AddComponent<CDx11Mouse>("Mouse");
	m_pMouse->SetWindowHandle(hWnd);
	m_hWnd = hWnd;
	CreateKey("MouseLButton", VK_LBUTTON);
	CreateKey("MouseRButton", VK_RBUTTON);

	CDx11ColliderPoint*	pColl = m_pMouseObj->AddComponent<CDx11ColliderPoint>("MouseCollider");
	SAFE_RELEASE(pColl);

	// 키 설정
	CreateKey("MoveFront",	'W');
	CreateKey("MoveBack",	'S');
	CreateKey("MoveLeft",	'A');
	CreateKey("MoveRight",  'D');
	CreateKey("RotYFront", 'T');
	CreateKey("RotYBack",  'Y');
	CreateKey("RotXFront", 'Z');
	CreateKey("RotXBack",  'X');
	CreateKey("Fire", VK_SPACE);
	CreateKey('1', "Skill1");

	return true;
}

int CDx11Input::Update(float fTime)
{
	unordered_map<string, PKEYINFO>::iterator	iter;
	unordered_map<string, PKEYINFO>::iterator	iterEnd = m_mapKey.end();

	// 윈도우 창이 활성화되있지 않다면,
	// 모든 키들의 상태를 false로 만들고 return
	if (!m_bActivated)
	{
		for (iter = m_mapKey.begin(); iter != iterEnd; ++iter)
		{
			iter->second->bDown = false;
			iter->second->bPush = false;
			iter->second->bUp = false;
		}
		return 0;
	}

	for (iter = m_mapKey.begin(); iter != iterEnd; ++iter)
	{
		UINT	iKey = 0;
		vector<UINT>::iterator	iter1;
		vector<UINT>::iterator	iter1End = iter->second->vecKey.end();

		for (iter1 = iter->second->vecKey.begin(); iter1 != iter1End; ++iter1)
		{
			if (GetAsyncKeyState(*iter1) & 0x8000)
				++iKey;
		}

		if (iKey == iter->second->vecKey.size())
		{
			if (!iter->second->bDown && !iter->second->bPush)
				iter->second->bDown = true;

			else if (iter->second->bDown && !iter->second->bPush)
			{
				iter->second->bPush = true;
				iter->second->bDown = false;
			}
		}

		else
		{
			if (iter->second->bDown || iter->second->bPush)
			{
				iter->second->bUp = true;
				iter->second->bDown = false;
				iter->second->bPush = false;
			}

			else
				iter->second->bUp = false;
		}

		// 마우스의 버튼 상태값 갱신
		if (iter->first == "MouseLButton")
		{
			m_pMouse->SetMouseLeftState(iter->second->bDown, iter->second->bPush, iter->second->bUp);
		}

		if (iter->first == "MouseRButton")
		{
			m_pMouse->SetMouseRightState(iter->second->bDown, iter->second->bPush, iter->second->bUp);
		}

	}

	DX11_GET_SINGLE(CDx11CollisionManager)->AddGameObject(m_pMouseObj);

	// 마우스의 위치를 받아온다.
	m_ptMouse = m_pMouse->GetMousePos();

	// 마우스 오브젝트의 트랜스폼 상의 좌표를 얻어온 POINT와 일치시킨다.
	CDx11Transform*	pTr = m_pMouseObj->GetTransform();
	pTr->SetWorldPos(m_ptMouse.x, m_ptMouse.y, 0.f);
	SAFE_RELEASE(pTr);

	m_pMouseObj->Update(fTime);
	m_pMouseObj->LateUpdate(fTime);

	return 0;
}

bool CDx11Input::KeyDown(const string & strKey)
{
	PKEYINFO	pKey = FindKey(strKey);

	if (!pKey)
		return false;

	return pKey->bDown;
}

bool CDx11Input::KeyPush(const string & strKey)
{
	PKEYINFO	pKey = FindKey(strKey);

	if (!pKey)
		return false;

	return pKey->bPush;
}

bool CDx11Input::KeyUp(const string & strKey)
{
	PKEYINFO	pKey = FindKey(strKey);

	if (!pKey)
		return false;

	return pKey->bUp;
}

void CDx11Input::Wheel(short sWheel)
{
	m_sWheel = sWheel / 120;
	m_pMouse->SetWheel(m_sWheel);
}

void CDx11Input::Clear()
{
	m_sWheel = 0;
	m_pMouse->Clear();
}

void CDx11Input::SetActivated(bool bActivated)
{
	m_bActivated = bActivated;
}

PKEYINFO CDx11Input::FindKey(const string & strKey)
{
	unordered_map<string, PKEYINFO>::iterator	iter = m_mapKey.find(strKey);

	if (iter == m_mapKey.end())
		return NULL;

	return iter->second;
}
