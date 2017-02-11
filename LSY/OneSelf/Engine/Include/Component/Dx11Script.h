#pragma once
#include "Dx11Component.h"

DX11_BEGIN

class DX11_DLL CDx11Script :
	public CDx11Component
{
protected:
	CDx11Script();
	CDx11Script(const CDx11Script& script);
	virtual ~CDx11Script() = 0;

public:
	virtual bool Init();
	virtual void Input(float fTime);
	virtual int Update(float fTime) = 0;
	virtual int LateUpdate(float fTime) = 0;
	virtual void Render(float fTime) = 0;
	virtual CDx11Script* Clone() = 0;
};

DX11_END
