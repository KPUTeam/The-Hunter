#pragma once
#include "../Dx11Object.h"

DX11_BEGIN

class DX11_DLL CDx11Debug :
	public CDx11Object
{
	DX11_SINGLE_DECLARE(CDx11Debug)

private:
	bool	m_bConsole;
	HWND	m_hWnd;

public:
	bool Init(HWND hWnd);
	void OpenConsole();
	void CloseConsole();
	void OutputConsole(char* pMsg);
	void OutputVisualDebug(char* pMsg);
	void OutputWindowTitle(TCHAR* pMsg);
};

DX11_END
