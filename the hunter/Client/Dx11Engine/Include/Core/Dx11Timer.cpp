#include "Dx11Timer.h"
#include "Dx11Debug.h"

DX11_USING

CDx11Timer::CDx11Timer()
{
	SetTypeName<CDx11Timer>();
}

CDx11Timer::~CDx11Timer()
{
}

float CDx11Timer::GetDeltaTime()
{
	return m_fDeltaTime;
}

float CDx11Timer::GetFPS()
{
	return m_fFPS;
}

bool CDx11Timer::Init()
{
	QueryPerformanceFrequency(&m_tSecond);
	QueryPerformanceCounter(&m_tCount);

	m_fDeltaTime = 0.f;
	m_fFPS = 0.f;
	m_fFPSTime = 0.f;
	m_iFrameCount = 0;
	m_iFrameLimitCount = 60;

	return true;
}

void CDx11Timer::Update()
{
	UpdateTime();
	UpdateFPS();
}

void CDx11Timer::UpdateTime()
{
	LARGE_INTEGER	tCount = {};
	QueryPerformanceCounter(&tCount);

	m_fDeltaTime = (tCount.QuadPart - m_tCount.QuadPart) /
		(float)m_tSecond.QuadPart;

	m_tCount = tCount;
}

void CDx11Timer::UpdateFPS()
{
	m_fFPSTime += m_fDeltaTime;
	++m_iFrameCount;

	if (m_iFrameCount == m_iFrameLimitCount)
	{
		m_fFPS = m_iFrameCount / m_fFPSTime;
		m_iFrameCount = 0;
		m_fFPSTime = 0.f;
		char	strMsg[128] = {};
		sprintf_s(strMsg, "FPS : %.5f\n", m_fFPS);

		TCHAR	strMsg1[128] = {};
		MultiByteToWideChar(CP_ACP, 0, strMsg, -1, strMsg1, strlen(strMsg) * 2);
		DX11_GET_SINGLE(CDx11Debug)->OutputWindowTitle(strMsg1);
	}
}
