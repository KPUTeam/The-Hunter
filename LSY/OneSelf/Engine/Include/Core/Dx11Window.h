#pragma once

#include "../Dx11Object.h"

DX11_BEGIN

class DX11_DLL CDx11Window:
	public CDx11Object
{
private:
	friend class CDx11Core;

private:
	CDx11Window();
	~CDx11Window();

private:
	HWND		m_hWnd;
	HINSTANCE	m_hInst;
	RESOLUTION_TYPE	m_eRSType;

public:
	RESOLUTION_TYPE GetResolutionType();
	HWND GetWindowHandle();

public:
	bool Init(TCHAR* pTitle, TCHAR* pClass, HINSTANCE hInst,
		int iIconID, int iSmallIconID, RESOLUTION_TYPE eRT,
		WNDPROC pProc);

private:
	ATOM MyRegisterClass(HINSTANCE hInst, TCHAR* pClass,
		int iIconID, int iSmallIconID, WNDPROC pProc);
	BOOL InitInstance(TCHAR* pTitle, TCHAR* pClass, RESOLUTION_TYPE eRT);
};

DX11_END