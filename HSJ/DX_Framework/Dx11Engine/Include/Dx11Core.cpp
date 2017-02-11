#include "Dx11Core.h"
#include "Core/Dx11Window.h"
#include "Core\Dx11Device.h"
#include "Resources\Dx11ResourcesManager.h"
#include "Rendering\Dx11ShaderManager.h"
#include "Core\Dx11FilePathManager.h"
#include "Rendering\Dx11Shader.h"
#include "Resources\Dx11Mesh.h"
#include "Scene\Dx11SceneManager.h"
#include "Core\Dx11TimerManager.h"
#include "Core\Dx11Timer.h"
#include "Core\Dx11Debug.h"
#include "GameObject\Dx11GameObjectManager.h"
#include "Core\Dx11Input.h"
#include "Core\Dx11Scheduler.h"
#include "Rendering\Dx11RenderManager.h"

DX11_USING

CDx11Core* CDx11Core::m_pInst = NULL;
bool CDx11Core::m_bLoop = true;

CDx11Core::CDx11Core()	:
	m_pWindow(NULL)
{
	srand(GetTickCount());
	SetTypeName<CDx11Core>();
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CRTDBG_ALLOC_MEM_DF);
	//_CrtSetBreakAlloc(286);
#endif // defined(DEBUG) | defined(_DEBUG)

}

CDx11Core::~CDx11Core()
{
	DX11_DESTROY_SINGLE(CDx11Scheduler);
	DX11_DESTROY_SINGLE(CDx11SceneManager);
	DX11_DESTROY_SINGLE(CDx11GameObjectManager);
	DX11_DESTROY_SINGLE(CDx11RenderManager);
	DX11_DESTROY_SINGLE(CDx11TimerManager);
	DX11_DESTROY_SINGLE(CDx11ResourcesManager);
	DX11_DESTROY_SINGLE(CDx11ShaderManager);
	DX11_DESTROY_SINGLE(CDx11FilePathManager);
	DX11_DESTROY_SINGLE(CDx11Input);
	DX11_DESTROY_SINGLE(CDx11Device);
	DX11_DESTROY_SINGLE(CDx11Debug);
	SAFE_RELEASE(m_pWindow);
}

CDx11Core * CDx11Core::GetInst()
{
	if (!m_pInst)
		m_pInst = new CDx11Core;

	return m_pInst;
}

void CDx11Core::DestroyInst()
{
	SAFE_DELETE(m_pInst);
}

bool CDx11Core::Init(TCHAR * pTitle, TCHAR * pClass,
	HINSTANCE hInst, int iIconID, int iSmallIconID, 
	RESOLUTION_TYPE eRT, bool bWindowMode, WNDPROC pProc)
{
	// 윈도우 창을 생성한다.
	m_pWindow = new CDx11Window;

	if (!pProc)
		pProc = CDx11Core::WndProc;

	if (!m_pWindow->Init(pTitle, pClass, hInst, iIconID, iSmallIconID,
		eRT, pProc))
		return false;

	if (!DX11_GET_SINGLE(CDx11Debug)->Init(m_pWindow->m_hWnd))
		return false;

	// 디바이스를 초기화한다.
	if (!DX11_GET_SINGLE(CDx11Device)->Init(eRT, bWindowMode, m_pWindow->GetWindowHandle()))
		return false;

	// 파일 경로 관리자를 초기화한다.
	if (!DX11_GET_SINGLE(CDx11FilePathManager)->Init())
		return false;

	// 리소스 관리자를 초기화한다.
	if (!DX11_GET_SINGLE(CDx11ResourcesManager)->Init())
		return false;

	// 스케쥴러 초기화
	if (!DX11_GET_SINGLE(CDx11Scheduler)->Init())
		return false;

	// 렌더링 관리자를 초기화한다.
	if (!DX11_GET_SINGLE(CDx11RenderManager)->Init())
		return false;

	// Shader 관리자를 초기화한다.
	if (!DX11_GET_SINGLE(CDx11ShaderManager)->Init())
		return false;

	// 타이머 관리자를 초기화한다.
	if (!DX11_GET_SINGLE(CDx11TimerManager)->Init())
		return false;

	// 입력 관리자를 초기화한다.
	if (!DX11_GET_SINGLE(CDx11Input)->Init())
		return false;
	
	// 오브젝트 관리자를 초기화한다.
	if (!DX11_GET_SINGLE(CDx11GameObjectManager)->Init())
		return false;

	// 장면 관리자를 초기화한다.
	if (!DX11_GET_SINGLE(CDx11SceneManager)->Init())
		return false;

	return true;
}

int CDx11Core::Run()
{
	MSG msg;

	// 메시지 루프입니다.
	while (m_bLoop)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		else
		{
			Logic();
		}
	}

	return (int)msg.wParam;
}

void CDx11Core::Logic()
{
	CDx11Timer*	pTimer = DX11_GET_SINGLE(CDx11TimerManager)->FindTimer("Main");

	pTimer->Update();

	float	fFrameTime = pTimer->GetFrameTime();

	SAFE_RELEASE(pTimer);

	Input(fFrameTime);
	Update(fFrameTime);
	LateUpdate(fFrameTime);
	Collision(fFrameTime);
	Render(fFrameTime);
}

void CDx11Core::Input(float fTime)
{
	DX11_GET_SINGLE(CDx11Input)->Update(fTime);
	// 스케쥴 관리자 업데이트
	DX11_GET_SINGLE(CDx11Scheduler)->Update(fTime);

	DX11_GET_SINGLE(CDx11SceneManager)->Input(fTime);
}

int CDx11Core::Update(float fTime)
{
	DX11_GET_SINGLE(CDx11SceneManager)->Update(fTime);

	return 0;
}

int CDx11Core::LateUpdate(float fTime)
{
	DX11_GET_SINGLE(CDx11SceneManager)->LateUpdate(fTime);

	return 0;
}

void CDx11Core::Collision(float fTime)
{
	DX11_GET_SINGLE(CDx11SceneManager)->Collision(fTime);
}

void CDx11Core::Render(float fTime)
{
	DX11_GET_SINGLE(CDx11SceneManager)->Render(fTime);
}

LRESULT CDx11Core::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		// TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다.
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DESTROY:
		m_bLoop = false;
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
