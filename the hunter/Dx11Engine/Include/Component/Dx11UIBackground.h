#pragma once
#include "Dx11UI.h"

DX11_BEGIN

class DX11_DLL CDx11UIBackground :
	public CDx11UI
{
private:
	friend class CDx11GameObject;

private:
	CDx11UIBackground();
	CDx11UIBackground(const CDx11UIBackground& ui);
	~CDx11UIBackground();

public:
	virtual bool Init();
	virtual void Input(float fTime);
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Render(float fTime);
	virtual CDx11UIBackground* Clone();

	virtual void OnCollisionEnter(class CDx11Collider* pColl, float fTime);
	virtual void OnCollision(class CDx11Collider* pColl, float fTime);
	virtual void OnCollisionExit(class CDx11Collider* pColl, float fTime);
};

DX11_END