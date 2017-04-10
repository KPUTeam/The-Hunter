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

float CDx11Timer::GetFrameTime()
{
	return (float)m_dFrameTime;
}

float CDx11Timer::GetFPS()
{
	return m_fFPS;
}

bool CDx11Timer::Init()
{
	QueryPerformanceFrequency(&m_tSecond);
	QueryPerformanceCounter(&m_tTime);

	m_dFrameTime = 0.0;

	m_fFPS = 0.f;
	m_fFPSTime = 0.f;
	m_iFPSCount = 0;
	m_iFPSLimitCount = 60;

	return true;
}

void CDx11Timer::Update()
{
	//FrameTime�� FPS�� �����Ѵ�.
	UpdateTime();
	UpdateFPS();
}

void CDx11Timer::UpdateTime()
{
	LARGE_INTEGER	tTime;
	QueryPerformanceCounter(&tTime);

	//FrameTime = (����ð� - �����ð�)�� CPU Cycle Frequency(�� �ð�)�� ������.
	m_dFrameTime = (tTime.QuadPart - m_tTime.QuadPart) /
		(double)m_tSecond.QuadPart;

	//FrameTime�� ���� �� �����ð��� ����ð� ������ ������.
	m_tTime = tTime;
}

void CDx11Timer::UpdateFPS()
{
	m_fFPSTime += (float)m_dFrameTime;
	++m_iFPSCount;

	//FPSCount�� �ִ� ���� ī��Ʈ�� �����ϸ�
	if (m_iFPSCount == m_iFPSLimitCount)
	{
		m_fFPS = m_iFPSCount / m_fFPSTime;
		m_fFPSTime = 0.f;
		m_iFPSCount = 0;

		//FPS ���
		char	str[32] = {};
		sprintf_s(str, "FPS : %.5f\n", m_fFPS);
		DX11_GET_SINGLE(CDx11Debug)->OutputWindowTitle(str);
//		DX11_GET_SINGLE(CDx11Debug)->OutputVisual(str);	

	}
}
