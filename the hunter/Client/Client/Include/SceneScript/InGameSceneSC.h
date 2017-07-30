#pragma once

#include "Component/Dx11Script.h"
#include "../ClientKey.h"

DX11_USING

class CInGameSceneSC :
	public CDx11Script
{
public:
	CInGameSceneSC();
	CInGameSceneSC(const CInGameSceneSC& script);
	virtual ~CInGameSceneSC();

public:
	virtual bool Init();
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Render(float fTime);
	virtual CInGameSceneSC* Clone();

public:
	void CreateAim();
	void CreateAim2();
	void CreateTerrain();
	void CreateCoordinateArrow();
	void CreateTank();
	void CrateMonster();

	// 선인장 배치
	void CreateCactus2();
	void CreateCactus3();
	void CreateCactus_Short1();
	void CreateCactus_Short2();
	void CreateCactus_Short3();
	void CreateLeafy();

	// 바위배치
	void CreateRock_Cone();
	void CreateRock_Heavy();
	void CreateRock_Large();
	void CreateRock_Small();

	void CreateWall();
	void CreateGrass();
	void CreateCampFire();

	// 바닥 텍스쳐
	void CreateTerrainTexs();
	void CreateTerrainTex(const char* pName, Vec3 p, int i = 0);

	CDx11GameObject * CreateObj(const char * cName, float fRotX, float fRotY, float fRotZ, float fScalX, float fScalY, float fScalZ, float fPosX, float fPosY, float fPosZ);
};

