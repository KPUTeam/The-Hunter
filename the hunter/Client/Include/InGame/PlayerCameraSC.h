#pragma once
#include "Component\Dx11Script.h"

DX11_USING

class CPlayerCameraSC :
	public CDx11Script
{
public:
	CPlayerCameraSC();
	~CPlayerCameraSC();

public:
	virtual bool Init();
	virtual void Input(float fTime);
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Render(float fTime);
	virtual CPlayerCameraSC* Clone();
	virtual void OnCollisionEnter(class CDx11Collider* pColl, float fTime);
	virtual void OnCollision(class CDx11Collider* pColl, float fTime);
	virtual void OnCollisionExit(class CDx11Collider* pColl, float fTime);
};

