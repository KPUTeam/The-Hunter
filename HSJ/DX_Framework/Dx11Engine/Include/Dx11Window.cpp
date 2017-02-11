#include "Dx11Window.h"

DX11_USING

CDx11Window::CDx11Window()
{
}

CDx11Window::~CDx11Window()
{
}

RESOLUTION_TYPE CDx11Window::GetResolutionType()
{
	return m_eRSType;
}

bool CDx11Window::Init(TCHAR * pTitle, TCHAR * pClass,
	HINSTANCE hInst, int iIconID, int iSmallIconID, 
	RESOLUTION_TYPE eRT,
	WNDPROC pProc)
{
	m_hInst = hInst;

	MyRegisterClass(hInst, pClass, iIconID, iSmallIconID, pProc);
	InitInstance(pTitle, pClass, eRT);

	return true;
}

ATOM CDx11Window::MyRegisterClass(HINSTANCE hInst, TCHAR* pClass,
	int iIconID, int iSmallIconID, WNDPROC pProc)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = pProc;
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

BOOL CDx11Window::InitInstance(TCHAR * pTitle, TCHAR * pClass, 
	RESOLUTION_TYPE eRT)
{
	m_hWnd = CreateWindowW(pClass, pTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, g_tRS[eRT].iWidth, g_tRS[eRT].iHeight, nullptr, nullptr, m_hInst,
		nullptr);

	if (!m_hWnd)
	{
		return FALSE;
	}

	RECT	rc = { 0, 0, g_tRS[eRT].iWidth, g_tRS[eRT].iHeight };

	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
	SetWindowPos(m_hWnd, HWND_TOPMOST, 0, 0, rc.right - rc.left,
		rc.bottom - rc.top, SWP_NOMOVE | SWP_NOZORDER);

	ShowWindow(m_hWnd, SW_SHOW);
	UpdateWindow(m_hWnd);

	return TRUE;
}
