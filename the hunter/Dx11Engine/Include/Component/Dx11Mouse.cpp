#include "Dx11Mouse.h"

DX11_USING

CDx11Mouse::CDx11Mouse()
{
	m_eComType = CT_MOUSE;
	SetTypeName<CDx11Mouse>();
}


CDx11Mouse::CDx11Mouse(const CDx11Mouse & mouse) :
	CDx11Component(mouse)
{
}

CDx11Mouse::~CDx11Mouse()
{
}

short CDx11Mouse::GetWheelDir() const
{
	return m_sWheel;
}

POINT CDx11Mouse::GetMousePos() const
{
	return m_ptPos;
}

Vec2 CDx11Mouse::GetMouseGap() const
{
	return m_ptGap;
}

bool CDx11Mouse::GetLeftDrag() const
{
	return m_bLeftDrag;
}

bool CDx11Mouse::GetLeftDown() const
{
	return m_bLeftDown;
}

bool CDx11Mouse::GetRightDrag() const
{
	return m_bRightDrag;
}

bool CDx11Mouse::GetRightDown() const
{
	return m_bRightDown;
}

bool CDx11Mouse::GetMouseSens() const
{
	return m_fMouseSens;
}

void CDx11Mouse::SetWindowHandle(HWND hWnd)
{
	m_hWnd = hWnd;
}

void CDx11Mouse::SetWheel(short sWheel)
{
	m_sWheel = sWheel;
}

void CDx11Mouse::SetMouseLeftState(bool bLeftDown, bool bLeftPush, bool bLeftUp)
{
	m_bLeftDown = bLeftDown;
	m_bLeftDrag = bLeftPush;
	m_bLeftUp = bLeftUp;
}

void CDx11Mouse::SetMouseRightState(bool bRightDown, bool bRightPush, bool bRightUp)
{
	m_bRightDown = bRightDown;
	m_bRightDrag = bRightPush;
	m_bRightUp = bRightUp;
}

void CDx11Mouse::Clear()
{
	m_sWheel = 0;
}

bool CDx11Mouse::Init()
{
	m_bLeftDown = false;
	m_bLeftDrag = false;
	m_bLeftUp = false;
	m_bRightDown = false;
	m_bRightDrag = false;
	m_bRightUp = false;
	m_fMouseSens = 0.003f;

	GetCursorPos(&m_ptPos);
	ScreenToClient(m_hWnd, &m_ptPos);
	m_ptPrevPos = m_ptPos;

	return true;
}

void CDx11Mouse::Input(float fTime)
{
}

int CDx11Mouse::Update(float fTime)
{
	// 마우스의 위치를 갱신한다.
	GetCursorPos(&m_ptPos);

	// 스크린 좌표를 클라이언트 좌표로 변환한다.
	ScreenToClient(m_hWnd, &m_ptPos);

	// 갭을 구한다.
	m_ptGap.x = m_ptPos.x - m_ptPrevPos.x;
	m_ptGap.y = m_ptPos.y - m_ptPrevPos.y;
	m_ptPrevPos = m_ptPos;

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
