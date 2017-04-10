#pragma once
#include "Component/Dx11Script.h"

DX11_USING

class CInGameScene :
	public CDx11Script
{
public:
	CInGameScene();
	~CInGameScene();

public:
	virtual bool Init();
	virtual void Input(float fTime);
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Render(float fTime);
	virtual CInGameScene* Clone();
};

