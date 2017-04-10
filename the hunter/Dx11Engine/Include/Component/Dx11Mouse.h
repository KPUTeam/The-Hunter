#pragma once
#include "Dx11Component.h"

DX11_BEGIN

class DX11_DLL CDx11Mouse :
	public CDx11Component
{
private:
	friend class CDx11GameObject;

private:
	CDx11Mouse();
	CDx11Mouse(const CDx11Mouse& mouse);
	~CDx11Mouse();

private:
	HWND		m_hWnd;
	short		m_sWheel;
	POINT		m_ptPos;
	POINT		m_ptPrevPos;
	Vec2		m_ptGap;
	float		m_fMouseSens;

private:
	bool		m_bLeftDown;
	bool		m_bLeftDrag;
	bool		m_bLeftUp;
	bool		m_bRightDown;
	bool		m_bRightDrag;
	bool		m_bRightUp;
	
public:
	short GetWheelDir() const;
	POINT GetMousePos() const;
	Vec2 GetMouseGap() const;
	bool GetLeftDrag() const;
	bool GetLeftDown() const;
	bool GetRightDrag() const;
	bool GetRightDown() const;
	bool GetMouseSens() const;


public:
	void SetWindowHandle(HWND hWnd);
	void SetWheel(short sWheel);
	void SetMouseLeftState(bool bLeftDown, bool bLeftPush, bool bLeftUp);
	void SetMouseRightState(bool bRightDown, bool bRightPush, bool bRightUp);
	void Clear();
public:
	virtual bool Init();
	virtual void Input(float fTime);
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Render(float fTime);
	virtual CDx11Mouse* Clone();
};

DX11_END
