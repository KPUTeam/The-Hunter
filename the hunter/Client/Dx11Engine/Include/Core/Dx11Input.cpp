#include "Dx11Input.h"
#include "../GameObject/Dx11GameObject.h"
#include "../Component/Dx11ColliderPoint.h"
#include "../Component/Dx11ColliderRay.h"
#include "../Component/Dx11Mouse.h"
#include "Dx11Device.h"

DX11_USING

DX11_SINGLE_DEFINITION(CDx11Input)

CDx11Input::CDx11Input() :
	m_pCreateKey(NULL),
	m_pMouseObj(NULL),
	m_pMouse(NULL),
	m_bInput(true)
{
}

CDx11Input::~CDx11Input()
{
	SAFE_RELEASE(m_pMouse);
	SAFE_RELEASE(m_pMouseObj);
	Safe_Delete_Map(m_mapKey);
}

bool CDx11Input::Init(HWND hWnd)
{
	m_hWnd = hWnd;

	//CreateKey('W', "MoveUp");
	//CreateKey('S', "MoveDown");
	//CreateKey("MoveLeft", 'A');
	//CreateKey("MoveRight", 'D');
	//CreateKey(VK_SPACE, "Fire");
	//CreateKey(VK_CONTROL, '1', "Skill1");

	CreateKey(VK_F1, VISIBILLTY_COLLIDER);
	CreateKey(VK_LBUTTON, "MouseLButton");
	CreateKey(VK_RBUTTON, "MouseRButton");

	m_pMouseObj = CDx11GameObject::Create("Mouse");

	m_pMouse = m_pMouseObj->AddComponent<CDx11Mouse>("Mouse");

	m_pMouse->SetWindowHandle(m_hWnd);

	return true;
}

const PKEYINFO CDx11Input::FindKey(const string & strName)
{
	unordered_map<string, PKEYINFO>::iterator	iter = m_mapKey.find(strName);

	if (iter == m_mapKey.end())
		return NULL;

	return iter->second;
}

void CDx11Input::Update(float fTime)
{
	unordered_map<string, PKEYINFO>::iterator	iter;
	unordered_map<string, PKEYINFO>::iterator	iterEnd = m_mapKey.end();

	vector<DWORD>::iterator	iter1;
	vector<DWORD>::iterator	iter1End;

	if (!m_bInput)
	{
		for (iter = m_mapKey.begin(); iter != iterEnd; ++iter)
		{
			iter->second->bDown = false;
			iter->second->bUp = false;
			iter->second->bStay = false;
		}
		return;
	}

	for (iter = m_mapKey.begin(); iter != iterEnd; ++iter)
	{
		iter1End = iter->second->vecKey.end();
		UINT	iInputCount = 0;

		for (iter1 = iter->second->vecKey.begin(); iter1 != iter1End; ++iter1)
		{
			if (GetAsyncKeyState(*iter1) & 0x8000)
				++iInputCount;
		}

		if (iInputCount == iter->second->vecKey.size())
		{
			if (!iter->second->bDown && !iter->second->bStay)
			{
				iter->second->bDown = true;
			}

			else if (iter->second->bDown && !iter->second->bStay)
			{
				iter->second->bDown = false;
				iter->second->bStay = true;
			}
		}

		else
		{
			if (iter->second->bDown || iter->second->bStay)
			{
				iter->second->bUp = true;
				iter->second->bDown = false;
				iter->second->bStay = false;
			}

			else if (iter->second->bUp)
			{
				iter->second->bUp = false;
			}
		}

		if (iter->first == "MouseLButton")
		{
			m_pMouse->SetLButtonInfo(iter->second->bDown, iter->second->bStay,
				iter->second->bUp);
		}

		else if (iter->first == "MouseRButton")
		{
			m_pMouse->SetRButtonInfo(iter->second->bDown, iter->second->bStay,
				iter->second->bUp);
		}
	}

	m_pMouseObj->Update(fTime);
}

bool CDx11Input::KeyDown(const string & strName)
{
	PKEYINFO	pKey = FindKey(strName);

	if (!pKey)
		return false;

	return pKey->bDown;
}

bool CDx11Input::KeyStay(const string & strName)
{
	PKEYINFO	pKey = FindKey(strName);

	if (!pKey)
		return false;

	return pKey->bStay;
}

bool CDx11Input::KeyUp(const string & strName)
{
	PKEYINFO	pKey = FindKey(strName);

	if (!pKey)
		return false;

	return pKey->bUp;
}

CDx11Mouse* CDx11Input::GetMouse()
{
	m_pMouse->AddRef();
	return m_pMouse;
}

CDx11GameObject * CDx11Input::GetMouseObj()
{
	return m_pMouseObj;
}

Vec2 CDx11Input::GetMousePos()	const
{
	return m_pMouse->GetMousePos();
}

Vec2 CDx11Input::GetMouseGap()	const
{
	return m_pMouse->GetMouseGap();
}

void CDx11Input::ClearInfo()
{
	m_sWheel = 0;
	m_pMouse->ClearInfo();
}

void CDx11Input::ResizeWindow(UINT iWidth, UINT iHeight)
{
	RESOLUTION_TYPE	eRSType = DX11_GET_SINGLE(CDx11Device)->GetResolutionType();

	m_vDisplayRatio.x = g_tRS[eRSType].iWidth / (float)iWidth;
	m_vDisplayRatio.y = g_tRS[eRSType].iHeight / (float)iHeight;

	if (m_pMouse)
		m_pMouse->ResizeWindow(m_vDisplayRatio);
}

void CDx11Input::SetInput(bool bInput)
{
	m_bInput = bInput;
}

bool CDx11Input::GetInput()
{
	return m_bInput;
}

void CDx11Input::Wheel(short sWheel)
{
	m_sWheel = sWheel / 120;
	m_pMouse->SetWheel(m_sWheel);
}
