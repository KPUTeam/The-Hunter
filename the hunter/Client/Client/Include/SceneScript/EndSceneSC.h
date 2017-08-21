#pragma once

#include "Component/Dx11Script.h"

DX11_USING

class CEndSceneSc :
	public CDx11Script
{
public:
	CEndSceneSc();
	CEndSceneSc(const CEndSceneSc& script);
	virtual ~CEndSceneSc();

private:
	class CDx11GameObject*   pEndLogo;

public:
	virtual bool Init();
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Render(float fTime);
	virtual CEndSceneSc* Clone();
	virtual int Exit(float fTime);
};
