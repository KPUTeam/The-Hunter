#include "Dx11Core.h"
#include "Core\Dx11Window.h"
#include "Core\Dx11Device.h"
#include "Core\Dx11TimerManager.h"
#include "Core\Dx11Timer.h"
#include "Core\Dx11Debug.h"
#include "Resources\Dx11ResourcesManager.h"
#include "Shader\Dx11ShaderManager.h"
#include "Resources\Dx11Mesh.h"
#include "Shader\Dx11Shader.h"
#include "Core\Dx11PathManager.h"
#include "Scene\Dx11SceneManager.h"
#include "GameObject\Dx11PrototypeManager.h"
#include "Core\Dx11Input.h"
#include "Core\Dx11Scheduler.h"
#include "Rendering\Dx11RenderManager.h"
#include "Core\Dx11CollisionManager.h"

DX11_USING

DX11_SINGLE_DEFINITION(CDx11Core)
bool CDx11Core::m_bRun = true;

CDx11Core::CDx11Core() :
	m_pWindow(NULL),
	m_fGravity(0.0098f)
{
#if defined(_DEBUG1) | defined(DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(532);
#endif // defined(_DEBUG1) | defined(DEBUG)

	srand(GetTickCount());
	SetTypeName<CDx11Core>();
}

CDx11Core::~CDx11Core()
{
	DX11_DESTROY_SINGLE(CDx11SceneManager);
	DX11_DESTROY_SINGLE(CDx11PathManager);
	DX11_DESTROY_SINGLE(CDx11ResourcesManager);
	DX11_DESTROY_SINGLE(CDx11ShaderManager);
	DX11_DESTROY_SINGLE(CDx11PrototypeManager);
	DX11_DESTROY_SINGLE(CDx11Scheduler);
	DX11_DESTROY_SINGLE(CDx11CollisionManager);
	DX11_DESTROY_SINGLE(CDx11Debug);
	DX11_DESTROY_SINGLE(CDx11Input);
	DX11_DESTROY_SINGLE(CDx11RenderManager);
	DX11_DESTROY_SINGLE(CDx11TimerManager);
	DX11_DESTROY_SINGLE(CDx11Device);
	SAFE_RELEASE(m_pWindow);
}

float CDx11Core::GetGravity() const
{
	return m_fGravity;
}

bool CDx11Core::Init(HINSTANCE hInst, RESOLUTION_TYPE eType,
	TCHAR* pTitle, TCHAR* pClass, int iIconID, int iSmallIconID,
	bool bWindowMode, WNDPROC wndProc)
{
	// 윈도우 창을 생성한다.
	m_pWindow = new CDx11Window;

	WNDPROC	pProc = wndProc;

	if (!pProc)
		pProc = CDx11Core::WndProc;

	if (!m_pWindow->Create(hInst, eType, pTitle, pClass, iIconID, iSmallIconID,
		pProc))
		return false;

	if (!Init(m_pWindow->m_hWnd, eType, bWindowMode))
		return false;

	return true;
}

bool CDx11Core::Init(HWND hWnd, RESOLUTION_TYPE eType, bool bWindowMode)
{
	// 디버깅 관리자 초기화
	if (!DX11_GET_SINGLE(CDx11Debug)->Init(hWnd))
		return false;

	DX11_GET_SINGLE(CDx11Debug)->OpenConsole();
	DX11_GET_SINGLE(CDx11Debug)->OutputConsole("Core Init\n");
	DX11_GET_SINGLE(CDx11Debug)->OutputVisualDebug("Core Init OutputDebug\n");

	// 디바이스 초기화
	if (!DX11_GET_SINGLE(CDx11Device)->Init(hWnd,
		eType, bWindowMode))
		return false;

	// 렌더링 관리자 초기화
	if (!DX11_GET_SINGLE(CDx11RenderManager)->Init())
		return false;

	// 타이머 관리자 초기화
	if (!DX11_GET_SINGLE(CDx11TimerManager)->Init())
		return false;

	// 패스 관리자 초기화
	if (!DX11_GET_SINGLE(CDx11PathManager)->Init())
		return false;

	// 셰이더 관리자 초기화
	if (!DX11_GET_SINGLE(CDx11ShaderManager)->Init())
		return false;

	// 입력 관리자 초기화
	if (!DX11_GET_SINGLE(CDx11Input)->Init(hWnd))
		return false;

	RESOLUTION_TYPE	eRSType = DX11_GET_SINGLE(CDx11Device)->GetResolutionType();
	DX11_GET_SINGLE(CDx11Input)->ResizeWindow(g_tRS[eRSType].iWidth, g_tRS[eRSType].iHeight);

	// 스케쥴러 관리자 초기화
	if (!DX11_GET_SINGLE(CDx11Scheduler)->Init())
		return false;

	// 프로토타입 관리자 초기화
	if (!DX11_GET_SINGLE(CDx11PrototypeManager)->Init())
		return false;

	// 리소스 관리자 초기화
	if (!DX11_GET_SINGLE(CDx11ResourcesManager)->Init())
		return false;

	// 장면 초기화
	if (!DX11_GET_SINGLE(CDx11SceneManager)->Init())
		return false;

	// 충돌 관리자 초기화
	if (!DX11_GET_SINGLE(CDx11CollisionManager)->Init())
		return false;

	return true;
}

int CDx11Core::Run()
{
	MSG msg;

	// 기본 메시지 루프입니다.
	while (m_bRun)
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

int CDx11Core::RunTool()
{
	Logic();

	return 0;
}

void CDx11Core::Logic()
{
	// 타이머 갱신
	CDx11Timer*	pTimer = DX11_GET_SINGLE(CDx11TimerManager)->FindTimer("MainTimer");

	pTimer->Update();

	float	fDeltaTime = pTimer->GetDeltaTime();

	// 입력 업데이트
	DX11_GET_SINGLE(CDx11Input)->Update(fDeltaTime);

	Input(fDeltaTime);

	SAFE_RELEASE(pTimer);

	DX11_GET_SINGLE(CDx11Scheduler)->Update(fDeltaTime);

	if (Update(fDeltaTime) == MSCF_CHANGE)
		return;

	if (LateUpdate(fDeltaTime) == MSCF_CHANGE)
		return;

	Collision(fDeltaTime);

	Render(fDeltaTime);
}

void CDx11Core::Input(float fTime)
{
	DX11_GET_SINGLE(CDx11CollisionManager)->Input(fTime);
}

int CDx11Core::Update(float fTime)
{
	int iRet = DX11_GET_SINGLE(CDx11SceneManager)->Update(fTime);

	return iRet;
}

int CDx11Core::LateUpdate(float fTime)
{
	int iRet = DX11_GET_SINGLE(CDx11SceneManager)->LateUpdate(fTime);

	return iRet;
}

void CDx11Core::Collision(float fTime)
{
	DX11_GET_SINGLE(CDx11CollisionManager)->AddCollisionObject(DX11_GET_SINGLE(CDx11Input)->GetMouseObj());
	DX11_GET_SINGLE(CDx11SceneManager)->AddCollision();
	DX11_GET_SINGLE(CDx11CollisionManager)->Collision(fTime);
}

void CDx11Core::Render(float fTime)
{
	DX11_GET_SINGLE(CDx11Device)->ClearView();

	DX11_GET_SINGLE(CDx11SceneManager)->Render(fTime);

	DX11_GET_SINGLE(CDx11Device)->Present();

	DX11_GET_SINGLE(CDx11Input)->ClearInfo();
}

LRESULT CDx11Core::WndProc(HWND hWnd, UINT message, WPARAM wParam,
	LPARAM lParam)
{
	switch (message)
	{
	case WM_ACTIVATEAPP:
		// 활성
		if (wParam == TRUE)
		{
			DX11_GET_SINGLE(CDx11Input)->SetInput(true);
		}

		// 비활성
		else
		{
			DX11_GET_SINGLE(CDx11Input)->SetInput(false);
		}
		break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);

		EndPaint(hWnd, &ps);
	}
	break;
	case WM_MOUSEWHEEL:
		DX11_GET_SINGLE(CDx11Input)->Wheel(HIWORD(wParam));
		break;
	case WM_SIZE:
		DX11_GET_SINGLE(CDx11Input)->ResizeWindow(LOWORD(lParam), HIWORD(lParam));
		break;
	case WM_SIZING:
		DX11_GET_SINGLE(CDx11Input)->ResizeWindow(LOWORD(lParam), HIWORD(lParam));
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		m_bRun = false;
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
