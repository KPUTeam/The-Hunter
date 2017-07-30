#pragma once
#include "Component\Dx11Script.h"
#include "../../ClientKey.h"

DX11_USING

class CTankUISC :
	public CDx11Script
{
public:
	CTankUISC();
	CTankUISC(const CTankUISC& script);
	~CTankUISC();

public:
	virtual bool Init();
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Render(float fTime);
	virtual CTankUISC* Clone();
};