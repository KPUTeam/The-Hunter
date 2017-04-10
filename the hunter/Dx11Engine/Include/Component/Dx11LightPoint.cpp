#include "Dx11LightPoint.h"
#include "Dx11Transform.h"
#include "../Rendering/Dx11Shader.h"

DX11_USING

CDx11LightPoint::CDx11LightPoint()	:
	m_vPos(Vec3Zero),
	m_fRange(1.f)
{
}

CDx11LightPoint::CDx11LightPoint(const CDx11LightPoint & light)
{
	m_vPos = light.m_vPos;
	m_fRange = light.m_fRange;
}


CDx11LightPoint::~CDx11LightPoint()
{
}

void CDx11LightPoint::SetPos(const Vec3 & vPos)
{
	m_pTransform->SetWorldPos(vPos);
}

void CDx11LightPoint::SetRange(float fRange)
{
	m_fRange = fRange;
}

bool CDx11LightPoint::Init()
{
	if (!CDx11Light::Init())
		return false;

	m_tInfo.iLightType = LT_POINT;
	m_fRange = 100.f;

	return true;
}

void CDx11LightPoint::Input(float fTime)
{
}

int CDx11LightPoint::Update(float fTime)
{

	return 0;
}

int CDx11LightPoint::LateUpdate(float fTime)
{
	m_vPos = m_pTransform->GetWorldPos();

	return 0;
}

void CDx11LightPoint::Render(float fTime)
{
}

CDx11LightPoint * CDx11LightPoint::Clone()
{
	return new CDx11LightPoint(*this);
}

void CDx11LightPoint::SetLight(CDx11Shader * pShader)
{
	CDx11Light::SetLight(pShader);
	Vec4	vPoint = Vec4(m_vPos.x, m_vPos.y, m_vPos.z, m_fRange);
	pShader->UpdateConstantBuffer("LightPoint", &vPoint, ST_PIXEL);
}
