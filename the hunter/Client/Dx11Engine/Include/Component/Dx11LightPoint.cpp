#include "Dx11LightPoint.h"
#include "Dx11Transform.h"
#include "../Shader/Dx11Shader.h"

DX11_USING

CDx11LightPoint::CDx11LightPoint()
{
}

CDx11LightPoint::CDx11LightPoint(const CDx11LightPoint & light)	:
	CDx11Light(light)
{
	m_tPoint = light.m_tPoint;
}

CDx11LightPoint::~CDx11LightPoint()
{
}

void CDx11LightPoint::SetPointInfo(const Vec3 & vPos, float fRange, const Vec3 & vAtt)
{
	m_tPoint.vPos = vPos;
	m_tPoint.fRange = fRange;
	m_tPoint.vAtt = vAtt;
}

void CDx11LightPoint::SetConstant(CDx11Shader * pShader)
{
	CDx11Light::SetConstant(pShader);

	pShader->UpdateConstantBuffer("LightPoint", &m_tPoint, ST_PIXEL);
}

bool CDx11LightPoint::Init()
{
	if (!CDx11Light::Init())
		return false;

	m_tPoint.fRange = 10.f;
	m_tPoint.vAtt = Vec3(0.f, 0.1f, 0.f);
	m_tInfo.iLightType = LT_POINT;

	return true;
}

int CDx11LightPoint::Update(float fTime)
{
	CDx11Light::Update(fTime);

	return 0;
}

int CDx11LightPoint::LateUpdate(float fTime)
{
	CDx11Light::LateUpdate(fTime);

	m_tPoint.vPos = m_pTransform->GetWorldPos();

	return 0;
}

void CDx11LightPoint::Render(float fTime)
{
	CDx11Light::Render(fTime);
}

CDx11LightPoint * CDx11LightPoint::Clone()
{
	return new CDx11LightPoint(*this);
}
