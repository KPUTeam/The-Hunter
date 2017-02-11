#include "Dx11LightDir.h"
#include "Dx11Transform.h"
#include "../Rendering/Dx11Shader.h"

DX11_USING

CDx11LightDir::CDx11LightDir()
{
}


CDx11LightDir::CDx11LightDir(const CDx11LightDir & LightDir)
{
	m_vDir = LightDir.m_vDir;
}

CDx11LightDir::~CDx11LightDir()
{
}

bool CDx11LightDir::Init()
{
	m_tInfo.iLightType = LT_DIR;

	return true;
}

void CDx11LightDir::Input(float fTime)
{
}

int CDx11LightDir::Update(float fTime)
{
	return 0;
}

int CDx11LightDir::LateUpdate(float fTime)
{
	m_vDir = m_pTransform->GetWorldAxis(AXIS_Z);

	return 0;
}

void CDx11LightDir::Render(float fTime)
{
}

CDx11LightDir * CDx11LightDir::Clone()
{
	return nullptr;
}

void CDx11LightDir::SetLight(CDx11Shader * pShader)
{
	CDx11Light::SetLight(pShader);
	Vec4	vDir = Vec4(m_vDir.x, m_vDir.y, m_vDir.z, 0.f);
	pShader->UpdateConstantBuffer("LightDir", &vDir, ST_PIXEL);
}
