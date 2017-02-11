#pragma once
#include "Component/Dx11Script.h"

DX11_USING

class CMainScene :
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

private:
	bool CreateBottom();
	bool CreatePlayer();
	bool CreateMonster();
	bool CreateBullet();

	int CreateMonster_Clone(float fTime);

};

