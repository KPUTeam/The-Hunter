#include "Dx11Mouse.h"
#include "Dx11Transform.h"
#include "Dx11ColliderPoint.h"
#include "Dx11ColliderRay.h"
#include "../GameObject/Dx11GameObject.h"
#include "../Core/Dx11Input.h"
#include "../Core/Dx11Device.h"

DX11_USING

CDx11Mouse::CDx11Mouse()
{
	m_eComType = CT_MOUSE;
	SetTypeName<CDx11Mouse>();
}

CDx11Mouse::CDx11Mouse(const CDx11Mouse & mouse)	:
	CDx11Component(mouse)
{
}

CDx11Mouse::~CDx11Mouse()
{
	SAFE_RELEASE(m_pColl);
	SAFE_RELEASE(m_pRayColl);
}

void CDx11Mouse::SetMousePositionToCenter()
{
	// 마우스 관련 옵션
	RECT rect;
	HWND hWnd = DX11_GET_SINGLE(CDx11Device)->GetHWND();
	GetWindowRect(hWnd, &rect);
	//	ClipCursor(&rect);
	//	ShowCursor(false);

	POINT pt;
	pt.x = (rect.right - rect.left) / 2;
	pt.y = (rect.bottom - rect.top) / 2;

	ClientToScreen(hWnd, &pt);
	SetCursorPos(pt.x, pt.y);

	POINT m_ptPos;
	GetCursorPos(&m_ptPos);
	ScreenToClient(m_hWnd, &m_ptPos);
	m_vGap.x = m_vGap.y = 0.f;
	m_vPos = m_ptPos;
}

void CDx11Mouse::SetPos(const POINT & ptPos)
{
	Vec2	vPos;

	vPos.x = (float)ptPos.x;
	vPos.y = (float)ptPos.y;

	m_vGap = vPos - m_vPos;
	m_vPos = vPos;
}

void CDx11Mouse::SetPos(const Vec2 & vPos)
{
	m_vGap = vPos - m_vPos;
	m_vPos = vPos;
}

void CDx11Mouse::SetPos(float x, float y)
{
	Vec2	vPos(x, y);

	m_vGap = vPos - m_vPos;

	m_vPos.x = x;
	m_vPos.y = y;
}

void CDx11Mouse::SetPos(int x, int y)
{
	Vec2	vPos((float)x, (float)y);

	m_vGap = vPos - m_vPos;

	m_vPos.x = (float)x;
	m_vPos.y = (float)y;
}

void CDx11Mouse::SetGap(const POINT & ptGap)
{
	m_vGap.x = (float)ptGap.x;
	m_vGap.y = (float)ptGap.y;
}

void CDx11Mouse::SetGap(const Vec2 & vGap)
{
	m_vGap = vGap;
}

void CDx11Mouse::SetGap(float x, float y)
{
	m_vGap.x = x;
	m_vGap.y = y;
}

void CDx11Mouse::SetGap(int x, int y)
{
	m_vGap.x = (float)x;
	m_vGap.y = (float)y;
}

void CDx11Mouse::ClearInfo()
{
	m_sWheel = 0;
}

void CDx11Mouse::ResizeWindow(const Vec2 & vRatio)
{
	m_vDisplayRatio = vRatio;
}

Vec2 CDx11Mouse::GetMousePos()
{
	return m_vPos;
}

Vec2 CDx11Mouse::GetMouseGap()
{
	return m_vGap;
}

bool CDx11Mouse::Init()
{
	m_bLDown = false;
	m_bLStay = false;
	m_bLUp = false;

	m_bRDown = false;
	m_bRStay = false;
	m_bRUp = false;

	m_vDisplayRatio = Vec2(1.f, 1.f);

	POINT		ptPos;
	GetCursorPos(&ptPos);
	ScreenToClient(m_hWnd, &ptPos);

	m_vPos = ptPos;
	m_pTransform->SetWorldPos(Vec3(m_vPos.x, m_vPos.y, 0.f));

	m_pColl = m_pGameObject->AddComponent<CDx11ColliderPoint>("Mouse");
	m_pRayColl = m_pGameObject->AddComponent<CDx11ColliderRay>("Ray");
	
	return true;
}

int CDx11Mouse::Update(float fTime)
{
	POINT		ptPos;
	GetCursorPos(&ptPos);
	ScreenToClient(m_hWnd, &ptPos);

	ptPos.x *= (LONG)m_vDisplayRatio.x;
	ptPos.y *= (LONG)m_vDisplayRatio.y;

	// Gap을 구한다.
	m_vGap.x = ptPos.x - m_vPos.x;
	m_vGap.y = ptPos.y - m_vPos.y;

	m_vPos = ptPos;
	m_pTransform->SetWorldPos(Vec3(m_vPos.x, m_vPos.y, 0.f));

	m_pColl->SetPoint(ptPos);
	m_pRayColl->SetMousePos(ptPos);

	return 0;
}

int CDx11Mouse::LateUpdate(float fTime)
{
	return 0;
}

void CDx11Mouse::Render(float fTime)
{
}

CDx11Mouse * CDx11Mouse::Clone()
{
	return new CDx11Mouse(*this);
}

void CDx11Mouse::SetLButtonInfo(bool bDown, bool bStay, bool bUp)
{
	m_bLDown = bDown;
	m_bLStay = bStay;
	m_bLUp = bUp;
}

void CDx11Mouse::SetRButtonInfo(bool bDown, bool bStay, bool bUp)
{
	m_bRDown = bDown;
	m_bRStay = bStay;
	m_bRUp = bUp;
}

void CDx11Mouse::SetWindowHandle(HWND hWnd)
{
	m_hWnd = hWnd;
}

void CDx11Mouse::SetWheel(short sWheel)
{
	m_sWheel = sWheel;
}

bool CDx11Mouse::LButtonDown()
{
	return m_bLDown;
}

bool CDx11Mouse::LButtonStay()
{
	return m_bLStay;
}

bool CDx11Mouse::LButtonUp()
{
	return m_bLUp;
}

bool CDx11Mouse::RButtonDown()
{
	return m_bRDown;
}

bool CDx11Mouse::RButtonStay()
{
	return m_bRStay;
}

bool CDx11Mouse::RButtonUp()
{
	return m_bRUp;
}

short CDx11Mouse::GetWheelDir()
{
	return m_sWheel;
}
