#include "Dx11Debug.h"

DX11_USING

DX11_DEFINITION_SINGLE(CDx11Debug)

CDx11Debug::CDx11Debug()
{
	SetTypeName<CDx11Debug>();
}

CDx11Debug::~CDx11Debug()
{
#if defined(DEBUG) | defined(_DEBUG)
	FreeConsole();
#endif
}

bool CDx11Debug::Init(HWND hWnd)
{
	m_hWnd = hWnd;
#if defined(DEBUG) | defined(_DEBUG)
	AllocConsole();
#endif

	return true;
}

void CDx11Debug::OutputConsole(char * pStr)
{
#if defined(DEBUG) | defined(_DEBUG)
	_cprintf(pStr);
#endif
}

void CDx11Debug::OutputVisual(char * pStr)
{
#if defined(DEBUG) | defined(_DEBUG)
	OutputDebugStringA(pStr);
#endif
}

void CDx11Debug::OutputWindowTitle(char * pStr)
{
	SetWindowTextA(m_hWnd, pStr);
}
