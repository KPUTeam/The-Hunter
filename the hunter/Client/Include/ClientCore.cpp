#include "ClientCore.h"
#include "Dx11Core.h"
#include "Scene/Dx11Scene.h"
#include "Scene/Dx11SceneManager.h"

#include "Logo\LogoScene.h"

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
	// 엔진 초기화
	if (!DX11_GET_SINGLE(CDx11Core)->Init(pTitle, pClass,
		hInst, IDI_ICON1, IDI_ICON1, eRT, bWindowMode, pProc))
	{
		DX11_DESTROY_SINGLE(CDx11Core);
		return 0;
	}

	// 디폴트 씬
	// 씬 네임 "Scene"
	CDx11Scene*	pScene = DX11_GET_SINGLE(CDx11SceneManager)->GetCurrentScene();
	
	// 씬 스크립트 추가
	CLogoScene*	pLogoScene = pScene->CreateSceneScript<CLogoScene>("LogoScene");
	SAFE_RELEASE(pLogoScene);

	SAFE_RELEASE(pScene);

	return true;
}

int CClientCore::Run()
{
	int	iRet = DX11_GET_SINGLE(CDx11Core)->Run();

	return iRet;
}
