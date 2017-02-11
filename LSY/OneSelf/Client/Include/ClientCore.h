#pragma once

#include "Client.h"

class CClientCore
{
	DX11_DECLARE_SINGLE(CClientCore)

public:
	bool Init(TCHAR* pTitle, TCHAR* pClass, HINSTANCE hInst,
		int iIconID, int iSmallIconID, RESOLUTION_TYPE eRT,
		bool bWindowMode, WNDPROC pProc = NULL);
	int Run();
};

