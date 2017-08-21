#pragma once

#include "Network.h"
#include "GameObject/Dx11GameObject.h"

DX11_USING

//typedef pair<Vec3, Vec3> monsterInfo;

class CMonsterManager
{
public:
	CMonsterManager();
	~CMonsterManager();

private:
	static CMonsterManager* m_pInst;

public:
	static CMonsterManager* GetInst()
	{
		if (!m_pInst)
			m_pInst = new CMonsterManager;
		return m_pInst;
	}

	static void DestroyInst()
	{
		if (m_pInst)
		{
			delete	m_pInst;
			m_pInst = NULL;
		}
	}

public:
	void SetMonsterPos();
//private:
//	list<monsterInfo> m_MonsterList;

//public:
//	list<monsterInfo>	getMonsterList() const;

public:
	bool Init();
};

