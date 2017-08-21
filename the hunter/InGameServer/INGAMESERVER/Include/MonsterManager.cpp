#include "MonsterManager.h"

CMonsterManager* CMonsterManager::m_pInst = NULL;

CMonsterManager::CMonsterManager()
{
}


CMonsterManager::~CMonsterManager()
{
//	Safe_Release_VecList(m_MonsterList);
}

//list<monsterInfo> CMonsterManager::getMonsterList() const
//{
//	return m_MonsterList;
//}

void CMonsterManager::SetMonsterPos()
{
	Vec3	vPos{ 41.5f, 0.f, 120.f };	Vec3	vRot{ 0.f, 225.f, 0.f };
}

bool CMonsterManager::Init()
{
	/*Vec3	vPos1{ 21, 0, 71 };		Vec3	vRot1{ 0, 90, 0 };
	Vec3	vPos2{ 37.5, 0, 124 };	Vec3	vRot2{ 0, 45, 0 };
	Vec3	vPos3{ 36, 0, 126 };	Vec3	vRot3{ 0, 90, 0 };
	Vec3	vPos4{ 37, 0, 128 };	Vec3	vRot4{ 0, 135, 0 };
	Vec3	vPos5{ 39, 0, 129 };	Vec3	vRot5{ 0, 180, 0 };
	Vec3	vPos6{ 41.5, 0, 127 };	Vec3	vRot6{ 0, 225, 0 };
	Vec3	vPos7{ 41, 0, 124 };	Vec3	vRot7{ 0, 315, 0 };
	Vec3	vPos8{ 94, 0, 60 };		Vec3	vRot8{ 0, 45, 0 };
	Vec3	vPos9{ 96, 0, 62 };		Vec3	vRot9{ 0, 270, 0 };
	Vec3	vPos10{ 92, 0, 58 };	Vec3	vRot10{ 0, 315, 0 };
	Vec3	vPos11{ 95.5, 0, 64 };	Vec3	vRot11{ 0, 10, 0 };
	Vec3	vPos12{ 91.5, 0, 60 };	Vec3	vRot12{ 0, 220, 0 };
	Vec3	vPos13{ 110, 0, 102 };	Vec3	vRot13{ 0, 10, 0 };
	Vec3	vPos14{ 110, 0, 100 };	Vec3	vRot14{ 0, 300, 0 };
	Vec3	vPos15{ 112, 0, 108 };	Vec3	vRot15{ 0, 150, 0 };
	Vec3	vPos16{ 114, 0, 104 };	Vec3	vRot16{ 0, 240, 0 };
	Vec3	vPos17{ 108, 0, 98 };	Vec3	vRot17{ 0, 315, 0 };
	Vec3	vPos18{ 106, 0, 96 };	Vec3	vRot18{ 0, 270, 0 };
	Vec3	vPos19{ 107, 0, 105 };	Vec3	vRot19{ 0, 270, 0 };
	Vec3	vPos20{ 112, 0, 96 };	Vec3	vRot20{ 0, 270, 0 };
	Vec3	vPos21{ 117, 0, 100 };	Vec3	vRot21{ 0, 180, 0 };
	Vec3	vPos22{ 100, 0, 101 };	Vec3	vRot22{ 0, 225, 0 };

	m_MonsterList.push_back(make_pair(vPos1, vRot1));
	m_MonsterList.push_back(make_pair(vPos2, vRot2));
	m_MonsterList.push_back(make_pair(vPos3, vRot3));
	m_MonsterList.push_back(make_pair(vPos4, vRot4));
	m_MonsterList.push_back(make_pair(vPos5, vRot5));
	m_MonsterList.push_back(make_pair(vPos6, vRot6));
	m_MonsterList.push_back(make_pair(vPos7, vRot7));
	m_MonsterList.push_back(make_pair(vPos8, vRot8));
	m_MonsterList.push_back(make_pair(vPos9, vRot9));
	m_MonsterList.push_back(make_pair(vPos10, vRot10));
	m_MonsterList.push_back(make_pair(vPos11, vRot11));
	m_MonsterList.push_back(make_pair(vPos12, vRot12));
	m_MonsterList.push_back(make_pair(vPos13, vRot13));
	m_MonsterList.push_back(make_pair(vPos14, vRot14));
	m_MonsterList.push_back(make_pair(vPos15, vRot15));
	m_MonsterList.push_back(make_pair(vPos16, vRot16));
	m_MonsterList.push_back(make_pair(vPos17, vRot17));
	m_MonsterList.push_back(make_pair(vPos18, vRot18));
	m_MonsterList.push_back(make_pair(vPos19, vRot19));
	m_MonsterList.push_back(make_pair(vPos20, vRot20));
	m_MonsterList.push_back(make_pair(vPos21, vRot21));
	m_MonsterList.push_back(make_pair(vPos22, vRot21));*/
	return true;
}