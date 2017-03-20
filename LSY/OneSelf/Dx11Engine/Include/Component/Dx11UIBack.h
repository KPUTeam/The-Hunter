#pragma once
#include "Dx11UI.h"

DX11_BEGIN

class DX11_DLL CDx11UIBack :
	public CDx11UI
{
private:
	friend class CDx11GameObject;

private:
	CDx11UIBack();
	CDx11UIBack(const CDx11UIBack& ui);
	~CDx11UIBack();

public:
	virtual bool Init();
	virtual void Input(float fTime);
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Render(float fTime);
	virtual CDx11UIBack* Clone();
};

DX11_END