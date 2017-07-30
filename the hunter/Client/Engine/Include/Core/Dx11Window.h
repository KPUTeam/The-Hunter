#pragma once

#include "../Dx11Object.h"

DX11_BEGIN

class DX11_DLL CDx11Window	:
	public CDx11Object
{
private:
	friend class CDx11Core;

protected:
	CDx11Window();
	~CDx11Window();

private:
	HWND		m_hWnd;
	HINSTANCE	m_hInst;
	RESOLUTION_TYPE	m_eType;

public:
	RESOLUTION_TYPE GetResolutionType()
	{
		return m_eType;
	}

	HWND GetWindowHandle();

public:
	bool Create(HINSTANCE hInst, RESOLUTION_TYPE eType,
		TCHAR* pTitle, TCHAR* pClass, int iIconID, int iSmallIconID,
		WNDPROC wndProc);

private:
	ATOM MyRegisterClass(HINSTANCE hInst, TCHAR* pClass, int iIconID, int iSmallIconID,
		WNDPROC wndProc);
	BOOL InitWindow(TCHAR* pTilte, TCHAR* pClass);
};

DX11_END
