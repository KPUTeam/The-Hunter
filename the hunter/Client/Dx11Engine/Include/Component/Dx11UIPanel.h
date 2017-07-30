#pragma once
#include "Dx11UI.h"

DX11_BEGIN

class DX11_DLL CDx11UIPanel :
	public CDx11UI
{
private:
	friend class CDx11GameObject;

private:
	CDx11UIPanel();
	CDx11UIPanel(const CDx11UIPanel& panel);
	~CDx11UIPanel();

private:
	bool	m_bDrag;

public:
	virtual bool Init();
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Render(float fTime);
	virtual CDx11UIPanel* Clone();
	virtual void OnCollisionEnter(class CDx11Collider* pMy, class CDx11Collider* pYour, float fTime);
	virtual void OnCollision(class CDx11Collider* pMy, class CDx11Collider* pYour, float fTime);
	virtual void OnCollisionExit(class CDx11Collider* pMy, class CDx11Collider* pYour, float fTime);

};

DX11_END
