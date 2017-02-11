#pragma once

#include "Component/Dx11Script.h"

DX11_USING

class CMainScene	:
	public CDx11Script
{
public:
	CMainScene();
	~CMainScene();

public:
	virtual bool Init();
	virtual void Input(float fTime);
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Render(float fTime);
	virtual CMainScene* Clone();

public:
	bool CreatePlayer();
	bool CreateBullet();
	bool CreateChild();
	bool CreateCamera();
	bool CreateMonster();
	bool CreateSkill();
	bool CreateSky();
	bool CreateInvisible();

public:
	int CreateMonster(float fTime);
};

