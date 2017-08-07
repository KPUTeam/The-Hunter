#pragma once
#include "Dx11Component.h"

DX11_BEGIN

class DX11_DLL CDx11Script :
	public CDx11Component
{
private:
	friend class CDx11GameObject;

protected:
	CDx11Script();
	CDx11Script(const CDx11Script& script);
	virtual ~CDx11Script() = 0;

public:
	virtual bool Init();
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Render(float fTime);
	virtual CDx11Script* Clone() = 0;
	virtual void OnCollisionEnter(class CDx11Collider* pMy, class CDx11Collider* pYour, float fTime);
	virtual void OnCollision(class CDx11Collider* pMy, class CDx11Collider* pYour, float fTime);
	virtual void OnCollisionExit(class CDx11Collider* pMy, class CDx11Collider* pYour, float fTime);
};


DX11_END
