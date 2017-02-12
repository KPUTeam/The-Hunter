#pragma once
#include "../Dx11Object.h"

DX11_BEGIN

class DX11_DLL CDx11Debug :
	public CDx11Object
{
	DX11_DECLARE_SINGLE(CDx11Debug)

private:
	HWND		m_hWnd;

public:
	bool Init(HWND hWnd);
	void OutputConsole(char* pStr);
	void OutputVisual(char* pStr);
	void OutputWindowTitle(char* pStr);
};

DX11_END