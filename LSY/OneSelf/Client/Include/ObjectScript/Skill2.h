#pragma once
#include "Component\Dx11Script.h"

DX11_USING

class CSkill2 :
	public CDx11Script
{
public:
	CSkill2();
	CSkill2(const CSkill2& skill2);
	~CSkill2();

private:
	float		m_fSpeed;
	float		m_fDist;

public:
	virtual bool Init();
	virtual void Input(float fTime);
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Render(float fTime);
	virtual CSkill2* Clone();
};

