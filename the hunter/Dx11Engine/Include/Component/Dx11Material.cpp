#include "Dx11Material.h"
#include "../Resources/Dx11Sampler.h"
#include "../Resources/Dx11Texture.h"
#include "../Resources/Dx11ResourcesManager.h"
#include "../Rendering/Dx11Shader.h"

DX11_USING

CDx11Material::CDx11Material()	:
	m_pDiffuseTex(NULL),
	m_pSpecularTex(NULL),
	m_pNormalMapTex(NULL),
	m_pDiffuseSampler(NULL)
{
	m_eComType = CT_MATERIAL;
	SetTypeName<CDx11Material>();
}


CDx11Material::CDx11Material(const CDx11Material & mat)	:
	CDx11Component(mat)
{
	*this = mat;

	if (m_pDiffuseTex)
		m_pDiffuseTex->AddRef();

	if (m_pSpecularTex)
		m_pSpecularTex->AddRef();

	if (m_pNormalMapTex)
		m_pNormalMapTex->AddRef();

	if (m_pDiffuseSampler)
		m_pDiffuseSampler->AddRef();
}

CDx11Material::~CDx11Material()
{
	SAFE_RELEASE(m_pDiffuseTex);
	SAFE_RELEASE(m_pSpecularTex);
	SAFE_RELEASE(m_pNormalMapTex);
	SAFE_RELEASE(m_pDiffuseSampler);
}

void CDx11Material::SetInfo(const Vec3 & vDif, const Vec3 & vAmb, 
	const Vec3 & vSpc, const Vec3 & vEmv, float fPower)
{
	memcpy(&m_tInfo.vDif, &vDif, sizeof(Vec3));
	memcpy(&m_tInfo.vAmb, &vAmb, sizeof(Vec3));
	memcpy(&m_tInfo.vSpc, &vSpc, sizeof(Vec3));
	memcpy(&m_tInfo.vEmv, &vEmv, sizeof(Vec3));
	m_tInfo.vSpc.w = fPower;
}

void CDx11Material::SetDiffuse(const Vec4 & vDif)
{
	m_tInfo.vDif = Vec3(vDif.x, vDif.y, vDif.z);
}

void CDx11Material::SetAmbient(const Vec4 & vAmb)
{
	m_tInfo.vAmb = Vec3(vAmb.x, vAmb.y, vAmb.z);
}

void CDx11Material::SetSpecular(const Vec4 & vSpc)
{
	m_tInfo.vSpc.x = vSpc.x;
	m_tInfo.vSpc.y = vSpc.y;
	m_tInfo.vSpc.z = vSpc.z;
}

void CDx11Material::SetEmissive(const Vec4 & vEmv)
{
	m_tInfo.vEmv = Vec3(vEmv.x, vEmv.y, vEmv.z);
}

void CDx11Material::SetSpecularPower(float fPower)
{
	m_tInfo.vSpc.w = fPower;
}

bool CDx11Material::Init()
{
	memset(&m_tInfo, 0, sizeof(m_tInfo));

	SetInfo(Vec3One, Vec3(0.2f, 0.2f, 0.2f), Vec3Zero, Vec3Zero, 3.2f);

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

//	리소스 관리자에서 해당 키의 텍스처를 로드한다.
	m_pDiffuseTex = DX11_GET_SINGLE(CDx11ResourcesManager)->LoadTexture(strKey, pFileName, strPathKey);
	if (!m_pDiffuseTex)
		return false;

	return true;
}

bool CDx11Material::SetDiffuseTexture(CDx11Texture * pTexture)
{
//	이미 로드된 텍스처일 때의 세팅
	SAFE_RELEASE(m_pDiffuseTex);
	
	m_pDiffuseTex = pTexture;
	pTexture->AddRef();

	return true;
}

bool CDx11Material::SetDiffuseTexture(const string & strKey, const string & strPath)
{
	SAFE_RELEASE(m_pDiffuseTex);

	m_pDiffuseTex = DX11_GET_SINGLE(CDx11ResourcesManager)->LoadTexture(strKey, strPath);

	if(!m_pDiffuseTex)
		return false;

	return true;
}

bool CDx11Material::SetSpecularTexture(const string & strKey, 
	TCHAR * pFileName, const string & strPathKey)
{
	SAFE_RELEASE(m_pSpecularTex);

	//	리소스 관리자에서 해당 키의 텍스처를 로드한다.
	m_pSpecularTex = DX11_GET_SINGLE(CDx11ResourcesManager)->LoadTexture(strKey, pFileName, strPathKey);
	if (!m_pSpecularTex)
		return false;

	m_tInfo.vDif.w = 1.f;

	return true;
}

bool CDx11Material::SetSpecularTexture(const string & strKey, const string & strPath)
{
	SAFE_RELEASE(m_pSpecularTex);

	m_pSpecularTex = DX11_GET_SINGLE(CDx11ResourcesManager)->LoadTexture(strKey, strPath);

	if (!m_pSpecularTex)
		return false;

	// Specular Texture가 있음을 vDif의 w값으로 식별.
	m_tInfo.vDif.w = 1.f;
	return true;
}

bool CDx11Material::SetNormalMapTexture(const string & strKey, 
	TCHAR * pFileName, const string & strPathKey)
{
	SAFE_RELEASE(m_pNormalMapTex);

	//	리소스 관리자에서 해당 키의 텍스처를 로드한다.
	m_pNormalMapTex = DX11_GET_SINGLE(CDx11ResourcesManager)->LoadTexture(strKey, pFileName, strPathKey);
	if (!m_pNormalMapTex)
		return false;

	m_tInfo.vAmb.w = 1.f;
	return true;
}

bool CDx11Material::SetNormalMapTexture(const string & strKey, const string & strPath)
{
	SAFE_RELEASE(m_pNormalMapTex);

	m_pNormalMapTex = DX11_GET_SINGLE(CDx11ResourcesManager)->LoadTexture(strKey, strPath);

	if (!m_pNormalMapTex)
		return false;

	// NormalMap Texture가 있음음 vAmb의 w값으로 식별.
	m_tInfo.vAmb.w = 1.f;

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

void CDx11Material::SetMaterial(class CDx11Shader* pShader, 
	UINT iRegisterTex, UINT iRegisterSampler)
{
	if (!pShader)
		return;

	pShader->UpdateConstantBuffer("Material", &m_tInfo, ST_VERTEX);
	pShader->UpdateConstantBuffer("Material", &m_tInfo, ST_PIXEL);

	if (m_pDiffuseTex)
		m_pDiffuseTex->SetTexture(iRegisterTex);

	if (m_pSpecularTex)
		m_pSpecularTex->SetTexture(1);

	if (m_pNormalMapTex)
		m_pNormalMapTex->SetTexture(2);

	if (m_pDiffuseSampler)
		m_pDiffuseSampler->SetSampler(iRegisterSampler);
}
