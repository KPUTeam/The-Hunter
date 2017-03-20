#pragma once
#include "Component\Dx11Script.h"

DX11_USING

class CLogoScene :
	public CDx11Script
{
public:
	CLogoScene();
	~CLogoScene();

public:
	virtual bool Init();
	virtual void Input(float fTime);
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Render(float fTime);
	virtual CLogoScene* Clone();
};

