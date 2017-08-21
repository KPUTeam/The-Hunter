#pragma once
#include "Dx11Script.h"

DX11_BEGIN

class DX11_DLL CDx11Camera3rd :
	public CDx11Script
{
private:
	friend class CDx11Camera;
	friend class CDx11GameObject;

private:
	CDx11Camera3rd();
	CDx11Camera3rd(const CDx11Camera3rd& camera);
	~CDx11Camera3rd();

private:
	float		m_fMouseSens;
	float		m_fMinDist;
	float		m_fMaxDist;

public:
	void SetLimitDistance(float fMin, float fMax);

public:
	virtual bool Init();
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Render(float fTime);
	virtual CDx11Camera3rd* Clone();
};

DX11_END
