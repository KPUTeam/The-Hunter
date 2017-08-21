#pragma once

#include "Component/Dx11Script.h"

DX11_USING

class CMainSceneSC :
	public CDx11Script
{
public:
	CMainSceneSC();
	CMainSceneSC(const CMainSceneSC& script);
	virtual ~CMainSceneSC();

private:
	FADE_STATE	m_eState;
	float		m_fFadeAmount;

public:
	virtual bool Init();
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Render(float fTime);
	virtual CMainSceneSC* Clone();

public:
	void FadeOut(float fTime);
	void FadeIn(float fTime);
};

