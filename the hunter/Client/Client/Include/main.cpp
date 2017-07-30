
#include "ClientCore.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	if (!GET_SINGLE(CClientCore)->Init(hInstance))
	{
		DESTROY_SINGLE(CClientCore);
		return 0;
	}

	int	iRet = GET_SINGLE(CClientCore)->Run();

	DESTROY_SINGLE(CClientCore);

	return iRet;
}