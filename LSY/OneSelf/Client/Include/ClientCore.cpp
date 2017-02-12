#include "ClientCore.h"
#include "Dx11Core.h"
#include "SceneScript\MainScene.h"
#include "Scene/Dx11Scene.h"
#include "Scene/Dx11SceneManager.h"

DX11_USING

DX11_DEFINITION_SINGLE(CClientCore)

CClientCore::CClientCore()
{
}

CClientCore::~CClientCore()
{
	DX11_DESTROY_SINGLE(CDx11Core);
}

bool CClientCore::Init(TCHAR* pTitle, TCHAR* pClass, HINSTANCE hInst,
	int iIconID, int iSmallIconID, RESOLUTION_TYPE eRT,
	bool bWindowMode, WNDPROC pProc)
{
	if (!DX11_GET_SINGLE(CDx11Core)->Init(pTitle, pClass,
		hInst, IDI_ICON1, IDI_ICON1, eRT, bWindowMode, pProc))
	{
		DX11_DESTROY_SINGLE(CDx11Core);
		return 0;
	}

	CDx11Scene*	pScene = DX11_GET_SINGLE(CDx11SceneManager)->GetCurrentScene();

	CMainScene*	pMainScene = pScene->CreateSceneScript<CMainScene>("MainScene");

	SAFE_RELEASE(pMainScene);

	SAFE_RELEASE(pScene);

	return true;
}

int CClientCore::Run()
{
	int	iRet = DX11_GET_SINGLE(CDx11Core)->Run();

	return iRet;
}
