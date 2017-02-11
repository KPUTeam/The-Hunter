#include "Dx11Light.h"
#include "../Rendering/Dx11Shader.h"

DX11_USING

CDx11Light::CDx11Light()	:
	m_tInfo({})
{
	m_eComType = CT_LIGHT;
	SetTypeName<CDx11Light>();
}

CDx11Light::CDx11Light(const CDx11Light & light)	:
	CDx11Component(light)
{
	m_tInfo = light.m_tInfo;
}

CDx11Light::~CDx11Light()
{
}

void CDx11Light::SetDiffuse(const Vec4 & vDif)
{
	m_tInfo.vDif = vDif;
}

void CDx11Light::SetAmbient(const Vec4 & vAmb)
{
	m_tInfo.vAmb = vAmb;
}

void CDx11Light::SetSpecular(const Vec4 & vSpc)
{
	m_tInfo.vSpc = vSpc;
}

bool CDx11Light::Init()
{
	m_tInfo.vDif = White;
	m_tInfo.vAmb = White;
	m_tInfo.vSpc = White;

	return true;
}

void CDx11Light::Input(float fTime)
{
}

int CDx11Light::Update(float fTime)
{
	return 0;
}

int CDx11Light::LateUpdate(float fTime)
{
	return 0;
}

void CDx11Light::Render(float fTime)
{
}

void CDx11Light::SetLight(CDx11Shader * pShader)
{
	pShader->UpdateConstantBuffer("LightInfo", &m_tInfo, ST_PIXEL);
}
