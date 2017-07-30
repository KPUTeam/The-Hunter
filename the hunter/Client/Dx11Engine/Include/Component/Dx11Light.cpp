#include "Dx11Light.h"
#include "../Shader/Dx11Shader.h"

DX11_USING

CDx11Light::CDx11Light()	:
	m_tInfo({})
{
	m_eComType = CT_LIGHT;
}

CDx11Light::CDx11Light(const CDx11Light & light)	:
	CDx11Component(light)
{
	m_iRef = 1;
	m_tInfo = light.m_tInfo;
}

CDx11Light::~CDx11Light()
{
}

void CDx11Light::SetLightInfo(const Vec4 & vDif, const Vec4 & vAmb, const Vec4 & vSpc)
{
	m_tInfo.vDif = vDif;
	m_tInfo.vAmb = vAmb;
	m_tInfo.vSpc = vSpc;
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

void CDx11Light::SetLightType(LIGHT_TYPE eType)
{
	m_tInfo.iLightType = (int)eType;
}

void CDx11Light::SetConstant(CDx11Shader * pShader)
{
	pShader->UpdateConstantBuffer("LightInfo", &m_tInfo, ST_PIXEL);
}

bool CDx11Light::Init()
{
	SetDiffuse(Vec4(1.f, 1.f, 1.f, 1.f));
	SetAmbient(Vec4(0.5f, 0.5f, 0.5f, 1.f));
	SetSpecular(Vec4(1.f, 1.f, 1.f, 1.f));

	return true;
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
