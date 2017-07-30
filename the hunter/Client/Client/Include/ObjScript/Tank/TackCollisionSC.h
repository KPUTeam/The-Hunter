#pragma once
#include "Component\Dx11Script.h"
#include "../../ClientKey.h"

DX11_USING

class CTankCollisionSC :
	public CDx11Script
{
public:
	CTankCollisionSC();
	CTankCollisionSC(const CTankCollisionSC& script);
	~CTankCollisionSC();
	

private:
	Vec3 vPrePos;

public:
	virtual bool Init();
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Render(float fTime);
	virtual CTankCollisionSC* Clone();

	void ShakeCamera();

public:
	virtual void OnCollisionEnter(class CDx11Collider* pMy, class CDx11Collider* pYour, float fTime);
	virtual void OnCollision(class CDx11Collider* pMy, class CDx11Collider* pYour, float fTime);
	virtual void OnCollisionExit(class CDx11Collider* pMy, class CDx11Collider* pYour, float fTime);
};

