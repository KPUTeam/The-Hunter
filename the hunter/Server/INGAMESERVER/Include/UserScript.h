#pragma once

#include "Component/Dx11Script.h"

DX11_USING

class CUserScript :
	public CDx11Script
{
public:
	CUserScript();
	CUserScript(const CUserScript& user);
	~CUserScript();

private:
	float				m_fHP;
	CDx11GameObject*	m_pHPBar;
	CDx11GameObject*	m_pFrameBar;

private:
	class CUserSession*	m_pSession;
	INT64				m_iSerialNumber;

public:
	void SetSession(class CUserSession* pSession);
	void SetSerialNumber(INT64 iSerialNumber);

	void CreateBar();
	void SetBarPos(float fTime);


public:
	virtual bool init();
	virtual void Input(float fTime);
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Render(float fTime);
	virtual CUserScript* Clone();
};

