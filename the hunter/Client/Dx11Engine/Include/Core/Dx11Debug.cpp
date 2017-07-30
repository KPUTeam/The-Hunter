#include "Dx11Debug.h"

DX11_USING

DX11_SINGLE_DEFINITION(CDx11Debug)

CDx11Debug::CDx11Debug()
{
	SetTypeName<CDx11Debug>();
}

CDx11Debug::~CDx11Debug()
{
	CloseConsole();
}

bool CDx11Debug::Init(HWND hWnd)
{
#if defined(DEBUG) | defined(_DEBUG1)
	m_bConsole = false;
#endif // _DEBUG1
	m_hWnd = hWnd;

	return true;
}

void CDx11Debug::OpenConsole()
{
#if defined(DEBUG) | defined(_DEBUG1)
	if (!m_bConsole)
	{
		AllocConsole();
		m_bConsole = true;
	}
#endif
}

void CDx11Debug::CloseConsole()
{
#if defined(DEBUG) | defined(_DEBUG1)
	if (m_bConsole)
	{
		m_bConsole = false;
		FreeConsole();
	}
#endif
}

void CDx11Debug::OutputConsole(char* pMsg)
{
	_cprintf(pMsg);
}

void CDx11Debug::OutputVisualDebug(char * pMsg)
{
	OutputDebugStringA(pMsg);
}

void CDx11Debug::OutputWindowTitle(TCHAR * pMsg)
{
	SetWindowText(m_hWnd, pMsg);
}
