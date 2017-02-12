#include "Dx11Input.h"

DX11_USING

DX11_DEFINITION_SINGLE(CDx11Input)

CDx11Input::CDx11Input()	:
	m_pCreateKey(NULL)
{
}

CDx11Input::~CDx11Input()
{
	Safe_Delete_Map(m_mapKey);
}

bool CDx11Input::Init()
{
	CreateKey("MoveFront", 'W');
	CreateKey('S', "MoveBack");
	CreateKey("RotZFront", 'D');
	CreateKey('A', "RotZBack");
	CreateKey("Fire", VK_SPACE);

	// Sample
	CreateKey(VK_CONTROL, '1', "Skill1");

	return true;
}

int CDx11Input::Update(float fTime)
{
	unordered_map<string, PKEYINFO>::iterator	iter;
	unordered_map<string, PKEYINFO>::iterator	iterEnd = m_mapKey.end();

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
	}

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

PKEYINFO CDx11Input::FindKey(const string & strKey)
{
	unordered_map<string, PKEYINFO>::iterator	iter = m_mapKey.find(strKey);

	if (iter == m_mapKey.end())
		return NULL;

	return iter->second;
}
