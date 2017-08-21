#include "Dx11LightDir.h"
#include "Dx11Transform.h"
#include "../Shader/Dx11Shader.h"

DX11_USING

CDx11LightDir::CDx11LightDir()
{
	SetTypeName<CDx11LightDir>();
}

CDx11LightDir::CDx11LightDir(const CDx11LightDir & light)	:
	CDx11Light(light)
{
	m_vDir = light.m_vDir;
}

CDx11LightDir::~CDx11LightDir()
{
}

void CDx11LightDir::SetLightDir(const Vec3 & vDir)
{
	m_vDir.x = vDir.x;
	m_vDir.y = vDir.y;
	m_vDir.z = vDir.z;
}

void CDx11LightDir::SetConstant(CDx11Shader * pShader)
{
	CDx11Light::SetConstant(pShader);

	pShader->UpdateConstantBuffer("LightDir", &m_vDir, ST_PIXEL);
}

bool CDx11LightDir::Init()
{
	if (!CDx11Light::Init())
		return false;

	m_tInfo.iLightType = (int)LT_DIR;

	return true;
}

int CDx11LightDir::Update(float fTime)
{
	CDx11Light::Update(fTime);

	Vec3	vDir = m_pTransform->GetWorldAxis(AXIS_Z);
	m_vDir.x = vDir.x;
	m_vDir.y = vDir.y;
	m_vDir.z = vDir.z;

	return 0;
}

int CDx11LightDir::LateUpdate(float fTime)
{
	CDx11Light::LateUpdate(fTime);

	return 0;
}

void CDx11LightDir::Render(float fTime)
{
	CDx11Light::Render(fTime);
}

CDx11LightDir * CDx11LightDir::Clone()
{
	return new CDx11LightDir(*this);
}
