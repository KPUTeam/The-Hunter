#pragma once
#include "Component\Dx11Script.h"

DX11_USING	

class CPlayer :
	public CDx11Script
{
public:
	CPlayer();
	CPlayer(const CPlayer & player);
	~CPlayer();

public:
	virtual bool Init();
	virtual void Input(float fTime);
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Render(float fTime);
	virtual CPlayer* Clone();

private:
	float x;
	float y;
	float z;

private:
	bool CreateKey();
	bool CreateMesh();
	bool CreateTex();
	int CreateBullet_Clone();

};

