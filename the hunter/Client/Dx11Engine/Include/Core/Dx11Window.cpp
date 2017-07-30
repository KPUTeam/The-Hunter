#include "Dx11Window.h"

DX11_USING

CDx11Window::CDx11Window()
{
	SetTypeName<CDx11Window>();
}

CDx11Window::~CDx11Window()
{
}

HWND CDx11Window::GetWindowHandle()
{
	return m_hWnd;
}

bool CDx11Window::Create(HINSTANCE hInst,
	RESOLUTION_TYPE eType, TCHAR * pTitle, TCHAR * pClass,
	int iIconID, int iSmallIconID, WNDPROC wndProc)
{
	m_hInst = hInst;
	m_eType = eType;

	MyRegisterClass(hInst, pClass, iIconID, iSmallIconID, wndProc);

	if (!InitWindow(pTitle, pClass))
		return false;

	return true;
}

ATOM CDx11Window::MyRegisterClass(HINSTANCE hInst, TCHAR* pClass,
	int iIconID, 
	int iSmallIconID, WNDPROC wndProc)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = wndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInst;
	wcex.hIcon = LoadIcon(hInst, MAKEINTRESOURCE(iIconID));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = pClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(iSmallIconID));

	return RegisterClassExW(&wcex);
}

BOOL CDx11Window::InitWindow(TCHAR* pTilte, TCHAR* pClass)
{
	m_hWnd = CreateWindowW(pClass, pTilte, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, m_hInst, nullptr);

	RECT	rc = { 0, 0, (LONG)g_tRS[m_eType].iWidth,(LONG)g_tRS[m_eType].iHeight };

	if (!m_hWnd)
		return FALSE;

	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
	SetWindowPos(m_hWnd, HWND_TOPMOST, 0, 0, rc.right - rc.left,
		rc.bottom - rc.top, SWP_NOZORDER | SWP_NOMOVE);

	ShowWindow(m_hWnd, SW_SHOW);
	UpdateWindow(m_hWnd);

	return TRUE;
}
