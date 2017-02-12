#pragma once
#include "Component\Dx11Script.h"

DX11_USING

class CSkill4 :
	public CDx11Script
{
public:
	CSkill4();
	CSkill4(const CSkill4& skill4);
	~CSkill4();

private:
	float		m_fSpeed;
	float		m_fDist;

public:
	virtual bool Init();
	virtual void Input(float fTime);
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Render(float fTime);
	virtual CSkill4* Clone();
};

