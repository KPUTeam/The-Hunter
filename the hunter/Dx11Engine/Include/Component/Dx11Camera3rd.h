#pragma once
#include "Dx11Component.h"

DX11_BEGIN

class DX11_DLL CDx11Camera3rd :
	public CDx11Component
{
private:
	friend class CDx11GameObject;
	friend class CDx11Camera;

private:
	CDx11Camera3rd();
	CDx11Camera3rd(const CDx11Camera3rd& camera);
	~CDx11Camera3rd();

private:
	float	m_fMin;
	float	m_fMax;

public:
	void SetMinMax(float fMin, float fMax);

public:
	virtual bool Init();
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Render(float fTime);
	virtual CDx11Camera3rd* Clone();
};

DX11_END
