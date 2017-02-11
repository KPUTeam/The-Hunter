#include "Dx11Material.h"
#include "../Resources/Dx11Sampler.h"
#include "../Resources/Dx11Texture.h"
#include "../Resources/Dx11ResourcesManager.h"
#include "../Rendering/Dx11Shader.h"

DX11_USING

CDx11Material::CDx11Material()	:
	m_pDiffuseTex(NULL),
	m_pDiffuseSampler(NULL)
{
	m_eComType = CT_MATERIAL;
	SetTypeName<CDx11Material>();
}

CDx11Material::~CDx11Material()
{
	SAFE_RELEASE(m_pDiffuseTex);
	SAFE_RELEASE(m_pDiffuseSampler);
}

void CDx11Material::SetDiffuse(const Vec4 & vDif)
{
	m_tInfo.vDif = vDif;
}

void CDx11Material::SetAmbient(const Vec4 & vAmb)
{
	m_tInfo.vAmb = vAmb;
}

void CDx11Material::SetSpecular(const Vec4 & vSpc)
{
	m_tInfo.vSpc.x = vSpc.x;
	m_tInfo.vSpc.y = vSpc.y;
	m_tInfo.vSpc.z = vSpc.z;
}

void CDx11Material::SetEmissive(const Vec4 & vEmv)
{
	m_tInfo.vEmv = vEmv;
}

void CDx11Material::SetSpecularPower(float fPower)
{
	m_tInfo.vSpc.w = fPower;
}

bool CDx11Material::Init()
{
	SetDiffuse(White);
	SetAmbient(Vec4(0.2f, 0.2f, 0.2f, 1.f));
	SetSpecular(White);
	SetEmissive(Black);
	SetSpecularPower(3.2f);

	return true;
}

int CDx11Material::Update(float fTime)
{
	return 0;
}

int CDx11Material::LateUpdate(float fTime)
{
	return 0;
}

void CDx11Material::Render(float fTime)
{
}

CDx11Material * CDx11Material::Clone()
{
	return new CDx11Material(*this);
}

bool CDx11Material::SetDiffuseTexture(const string & strKey, 
	TCHAR * pFileName, const string & strPathKey)
{
	SAFE_RELEASE(m_pDiffuseTex);

	m_pDiffuseTex = DX11_GET_SINGLE(CDx11ResourcesManager)->LoadTexture(strKey, pFileName, strPathKey);

	if (!m_pDiffuseTex)
		return false;

	return true;
}

bool CDx11Material::SetDiffuseSampler(const string & strKey)
{
	SAFE_RELEASE(m_pDiffuseSampler);

	m_pDiffuseSampler = DX11_GET_SINGLE(CDx11ResourcesManager)->FindSampler(strKey);

	if (!m_pDiffuseSampler)
		return false;
	
	return true;
}

void CDx11Material::SetMaterial(class CDx11Shader* pShader, UINT iRegisterTex, UINT iRegisterSampler)
{
	if (!pShader)
		return;

	pShader->UpdateConstantBuffer("Material", &m_tInfo, ST_PIXEL);

	if (m_pDiffuseTex)
		m_pDiffuseTex->SetTexture(iRegisterTex);

	if (m_pDiffuseSampler)
		m_pDiffuseSampler->SetSampler(iRegisterSampler);
}
