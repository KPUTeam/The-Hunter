#include "Dx11LightSpot.h"
#include "Dx11Transform.h"
#include "../Rendering/Dx11Shader.h"

DX11_USING

CDx11LightSpot::CDx11LightSpot()	:
	m_vPos(Vec3Zero),
	m_vDir(Vec3Z),
	m_fRange(1.f),
	m_fSpot(0.f)
{
}


CDx11LightSpot::CDx11LightSpot(const CDx11LightSpot & light) :
	CDx11Light(light)
{
	m_vPos = light.m_vPos;
	m_vDir = light.m_vDir;
	m_fRange = light.m_fRange;
	m_fSpot = light.m_fSpot;
}

CDx11LightSpot::~CDx11LightSpot()
{
}

void CDx11LightSpot::SetPos(const Vec3 & vPos)
{
	m_pTransform->SetWorldPos(vPos);
}

void CDx11LightSpot::SetRange(float fRange)
{
	m_fRange = fRange;
}

void CDx11LightSpot::SetDireciton(const Vec3 & vDir)
{
}

void CDx11LightSpot::SetSpot(float fSpot)
{
	m_fSpot = fSpot;
}

bool CDx11LightSpot::Init()
{
	if (!CDx11Light::Init())
		return false;

	m_tInfo.iLightType = LT_SPOT;
	m_fRange = 20.f;
	m_fSpot = 5.f;

	return true;
}

void CDx11LightSpot::Input(float fTime)
{
}

int CDx11LightSpot::Update(float fTime)
{
	if (GetAsyncKeyState(VK_UP) & 0x8000)
	{
		m_pTransform->MoveWorld(2.f, fTime, AXIS_Y);
	}

	if (GetAsyncKeyState(VK_DOWN) & 0x8000)
	{
		m_pTransform->MoveWorld(-2.f, fTime, AXIS_Y);
	}

	if (GetAsyncKeyState(VK_LEFT) & 0x8000)
	{
		m_pTransform->MoveWorld(-2.f, fTime, AXIS_X);
	}

	if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
	{
		m_pTransform->MoveWorld(2.f, fTime, AXIS_X);
	}

	if (GetAsyncKeyState('P') & 0x8000)
	{
		m_pTransform->MoveWorld(2.f, fTime, AXIS_Z);
	}

	if (GetAsyncKeyState('L') & 0x8000)
	{
		m_pTransform->MoveWorld(-2.f, fTime, AXIS_Z);
	}

	return 0;
}

int CDx11LightSpot::LateUpdate(float fTime)
{
	m_vPos = m_pTransform->GetWorldPos();
	m_vDir = m_pTransform->GetWorldRot();

	return 0;
}

void CDx11LightSpot::Render(float fTime)
{
}

CDx11LightSpot * CDx11LightSpot::Clone()
{
	return new CDx11LightSpot(*this);
}

void CDx11LightSpot::SetLight(CDx11Shader * pShader)
{
	CDx11Light::SetLight(pShader);
	SPOTLIGHT SpotInfo;
	SpotInfo.vPos = m_vPos;
	SpotInfo.fRange = m_fRange;
	SpotInfo.vDir = m_vDir;
	SpotInfo.fSpot = m_fSpot;

	pShader->UpdateConstantBuffer("LightSpot", &SpotInfo, ST_PIXEL);
}
