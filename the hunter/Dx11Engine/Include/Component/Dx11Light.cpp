#include "Dx11Light.h"
#include "../Rendering/Dx11Shader.h"
#include "../Scene/Dx11Scene.h"
#include "../GameObject/Dx11GameObject.h"
#include "../Component/Dx11Transform.h"

DX11_USING

CDx11Light::CDx11Light()	:
	m_tInfo({}),
	m_tFogInfo({})
{
	m_eComType = CT_LIGHT;
	SetTypeName<CDx11Light>();
}


CDx11Light::CDx11Light(const CDx11Light & light)	:
	CDx11Component(light)
{
	m_tInfo = light.m_tInfo;
	m_tFogInfo = light.m_tFogInfo;
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

void CDx11Light::SetFogPos(const Vec3 & vPos)
{
	m_tFogInfo.vEyePos = vPos;
}

void CDx11Light::SetFogColor(const Vec4 & vColor)
{
	m_tFogInfo.vColor = vColor;
}

bool CDx11Light::Init()
{
	m_tInfo.vDif = White;
	m_tInfo.vAmb = White;
	m_tInfo.vSpc = White;

	m_tFogInfo.vEyePos = Vec3(0.f, 0.f, 0.f);
	m_tFogInfo.fStart = 20.f;
	m_tFogInfo.fRange = 100.f;
	m_tFogInfo.vColor = Vec4(0.5f, 0.5f, 0.5f, 1.f);
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
	CDx11GameObject*	pCameraObj = m_pScene->GetMainCamera();
	CDx11Transform*		pCamTr = pCameraObj->GetTransform();

	SetFogPos(pCamTr->GetWorldPos());

	SAFE_RELEASE(pCamTr);
	SAFE_RELEASE(pCameraObj);

	pShader->UpdateConstantBuffer("LightInfo", &m_tInfo, ST_PIXEL);
	pShader->UpdateConstantBuffer("FogInfo", &m_tFogInfo, ST_PIXEL);
}
