#pragma once
#include "Dx11Component.h"

DX11_BEGIN

class DX11_DLL CDx11Mouse :
	public CDx11Component
{
private:
	friend class CDx11GameObject;
	friend class CDx11Input;

public:
	CDx11Mouse();
	CDx11Mouse(const CDx11Mouse& mouse);
	~CDx11Mouse();

private:
	Vec2		m_vPos;
	Vec2		m_vGap;

	bool		m_bLDown;
	bool		m_bLStay;
	bool		m_bLUp;

	bool		m_bRDown;
	bool		m_bRStay;
	bool		m_bRUp;
	HWND		m_hWnd;
	short		m_sWheel;
	Vec2		m_vDisplayRatio;
	class CDx11ColliderPoint*	m_pColl;
	class CDx11ColliderRay*		m_pRayColl;

private:
	void SetLButtonInfo(bool bDown, bool bStay, bool bUp);
	void SetRButtonInfo(bool bDown, bool bStay, bool bUp);
	void SetWindowHandle(HWND hWnd);
	void SetWheel(short sWheel);

public:
	bool LButtonDown();
	bool LButtonStay();
	bool LButtonUp();
	bool RButtonDown();
	bool RButtonStay();
	bool RButtonUp();
	short GetWheelDir();

private:
	void SetPos(const POINT& ptPos);
	void SetPos(const Vec2& vPos);
	void SetPos(float x, float y);
	void SetPos(int x, int y);
	void SetGap(const POINT& ptGap);
	void SetGap(const Vec2& vGap);
	void SetGap(float x, float y);
	void SetGap(int x, int y);
	void ClearInfo();
	void ResizeWindow(const Vec2& vRatio);

public:
	Vec2 GetMousePos();
	Vec2 GetMouseGap();

public:
	virtual bool Init();
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Render(float fTime);
	virtual CDx11Mouse* Clone();
};

DX11_END
