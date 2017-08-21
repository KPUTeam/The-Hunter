#include "Dx11Material.h"
#include "../Resources/Dx11ResourcesManager.h"
#include "../Resources/Dx11Texture.h"
#include "../Shader/Dx11Shader.h"

DX11_USING

CDx11Material::CDx11Material()	:
	m_pDiffuseTex(NULL),
	m_pSpecularTex(NULL),
	m_pNormalMapTex(NULL)
{
	memset(&m_tMaterial, 0, sizeof(m_tMaterial));
	SetTypeName<CDx11Material>();
	m_eComType = CT_MATERIAL;
	//XMMatrixRotationAxis();
}

CDx11Material::CDx11Material(const CDx11Material & script)	:
	CDx11Component(script)
{
	*this = script;
	m_iRef = 1;

	if(m_pDiffuseTex)
		m_pDiffuseTex->AddRef();

	if (m_pSpecularTex)
		m_pSpecularTex->AddRef();

	if (m_pNormalMapTex)
		m_pNormalMapTex->AddRef();

	for (size_t i = 0; i < m_vecMultiTex.size(); ++i)
	{
		m_vecMultiTex[i]->AddRef();
	}
}

CDx11Material::~CDx11Material()
{
	Safe_Release_VecList(m_vecMultiTex);
	SAFE_RELEASE(m_pNormalMapTex);
	SAFE_RELEASE(m_pSpecularTex);
	SAFE_RELEASE(m_pDiffuseTex);
}

void CDx11Material::SetInfo(const Vec3 & vDif, const Vec3 & vAmb, const Vec3 & vSpc, const Vec3 & vEmv, float fPower, float fFadeAmount)
{
	memcpy(&m_tMaterial.vDif, &vDif, sizeof(Vec3));
	memcpy(&m_tMaterial.vAmb, &vAmb, sizeof(Vec3));
	memcpy(&m_tMaterial.vSpc, &vSpc, sizeof(Vec3));
	memcpy(&m_tMaterial.vEmv, &vEmv, sizeof(Vec3));
	m_tMaterial.vSpc.w = fPower;
	m_tMaterial.fFadeAmount = fFadeAmount;
}

void CDx11Material::SetDiffuseTexture(const string & strKey)
{
	SAFE_RELEASE(m_pDiffuseTex);
	m_pDiffuseTex = DX11_GET_SINGLE(CDx11ResourcesManager)->FindTexture(strKey);
}

void CDx11Material::SetDiffuseTexture(const string & strKey, 
	TCHAR * pFileName, const string & strPathKey)
{
	SAFE_RELEASE(m_pDiffuseTex);
	m_pDiffuseTex = DX11_GET_SINGLE(CDx11ResourcesManager)->LoadTexture(strKey, pFileName, strPathKey);
}

void CDx11Material::SetDiffuseTexture(CDx11Texture * pTexture)
{
	SAFE_RELEASE(m_pDiffuseTex);
	m_pDiffuseTex = pTexture;
	m_pDiffuseTex->AddRef();
}

void CDx11Material::SetDiffuseTexture(const string & strKey, const string & strPath)
{
	SAFE_RELEASE(m_pDiffuseTex);
	m_pDiffuseTex = DX11_GET_SINGLE(CDx11ResourcesManager)->LoadTexture(strKey, strPath);
}

void CDx11Material::SetSpecularTexture(const string & strKey, TCHAR * pFileName, const string & strPathKey)
{
	SAFE_RELEASE(m_pSpecularTex);
	m_pSpecularTex = DX11_GET_SINGLE(CDx11ResourcesManager)->LoadTexture(strKey, pFileName, strPathKey);
	m_tMaterial.vDif.w = 1.f;
}

void CDx11Material::SetSpecularTexture(const string & strKey, const string & strPath)
{
	SAFE_RELEASE(m_pSpecularTex);
	m_pSpecularTex = DX11_GET_SINGLE(CDx11ResourcesManager)->LoadTexture(strKey, strPath);
	m_tMaterial.vDif.w = 1.f;
}

void CDx11Material::SetNormalMapTexture(const string & strKey, TCHAR * pFileName, const string & strPathKey)
{
	SAFE_RELEASE(m_pNormalMapTex);
	m_pNormalMapTex = DX11_GET_SINGLE(CDx11ResourcesManager)->LoadTexture(strKey, pFileName, strPathKey);
	m_tMaterial.vAmb.w = 1.f;
}

void CDx11Material::SetNormalMapTexture(const string & strKey, const string & strPath)
{
	SAFE_RELEASE(m_pNormalMapTex);
	m_pNormalMapTex = DX11_GET_SINGLE(CDx11ResourcesManager)->LoadTexture(strKey, strPath);
	m_tMaterial.vAmb.w = 1.f;
}

void CDx11Material::SetDiffuse(const Vec4 & vDif)
{
	memcpy(&m_tMaterial.vDif, &vDif, sizeof(Vec3));
}

void CDx11Material::SetAmbient(const Vec4 & vAmb)
{
	memcpy(&m_tMaterial.vAmb, &vAmb, sizeof(Vec3));
}

void CDx11Material::SetSpecular(const Vec3 & vSpc)
{
	memcpy(&m_tMaterial.vSpc, &vSpc, sizeof(Vec3));
}

void CDx11Material::SetSpecular(const Vec4 & vSpc)
{
	memcpy(&m_tMaterial.vSpc, &vSpc, sizeof(Vec3));
}

void CDx11Material::SetEmissive(const Vec4 & vEmv)
{
	memcpy(&m_tMaterial.vEmv, &vEmv, sizeof(Vec3));
}

void CDx11Material::SetSpecularPower(float fPower)
{
	m_tMaterial.vSpc.w = fPower;
}

void CDx11Material::AddFadeAmount(float fAmount)
{
	m_tMaterial.fFadeAmount += fAmount;
}

void CDx11Material::SubtractFadeAmount(float fAmount)
{
	m_tMaterial.fFadeAmount -= fAmount;
}

void CDx11Material::SetFadeAmount(float fAmount)
{
	m_tMaterial.fFadeAmount = fAmount;
}

float CDx11Material::GetFadeAmount() const
{
	return m_tMaterial.fFadeAmount;
}

void CDx11Material::SetMaterial(class CDx11Shader* pShader)
{
	if (m_pDiffuseTex)
		m_pDiffuseTex->SetTexture();

	if (m_pSpecularTex)
		m_pSpecularTex->SetTexture(1, UINT_MAX);

	if (m_pNormalMapTex)
		m_pNormalMapTex->SetTexture(2, UINT_MAX);

	for (size_t i = 0; i < m_vecMultiTex.size(); ++i)
	{
		m_vecMultiTex[i]->SetTexture(i + 6, i + 6);
	}

	pShader->UpdateConstantBuffer("Material", &m_tMaterial, ST_VERTEX);
	pShader->UpdateConstantBuffer("Material", &m_tMaterial, ST_PIXEL);
}

void CDx11Material::AddMultiTex(const string & strKey)
{
	CDx11Texture*	pTex = DX11_GET_SINGLE(CDx11ResourcesManager)->FindTexture(strKey);
	if (pTex)
		m_vecMultiTex.push_back(pTex);
}

void CDx11Material::AddMultiTex(const string & strKey, TCHAR * pFileName, const string & strPathKey)
{
	CDx11Texture*	pTex = DX11_GET_SINGLE(CDx11ResourcesManager)->LoadTexture(strKey,
		pFileName, strPathKey);

	if (pTex)
		m_vecMultiTex.push_back(pTex);
}

void CDx11Material::AddMultiTex(CDx11Texture * pTex)
{
	if (!pTex)
		return;

	pTex->AddRef();
	m_vecMultiTex.push_back(pTex);
}

void CDx11Material::DeleteMultiTex(const UINT & idx)
{
	if (idx >= m_vecMultiTex.size())
		return;
	
	vector<CDx11Texture*>::iterator	iter = m_vecMultiTex.begin() + idx;
	SAFE_RELEASE((*iter));
	m_vecMultiTex.erase(iter);
}

void CDx11Material::DeleteMultiTex(const string & strTag)
{
	vector<CDx11Texture*>::iterator	iter;
	vector<CDx11Texture*>::iterator	iterEnd = m_vecMultiTex.end();

	for (iter = m_vecMultiTex.begin(); iter != iterEnd; ++iter)
	{
		if ((*iter)->GetTag() == strTag)
		{
			SAFE_RELEASE((*iter));
			m_vecMultiTex.erase(iter);
			return;
		}
	}
}

void CDx11Material::DeleteMultiTex(TCHAR * pFileName)
{
	vector<CDx11Texture*>::iterator	iter;
	vector<CDx11Texture*>::iterator	iterEnd = m_vecMultiTex.end();

	for (iter = m_vecMultiTex.begin(); iter != iterEnd; ++iter)
	{
		if ((*iter)->GetFileName() == pFileName)
		{
			SAFE_RELEASE((*iter));
			m_vecMultiTex.erase(iter);
			return;
		}
	}
}

bool CDx11Material::Init()
{
	//m_tMaterial.vDif = VEC4ONE;
	//m_tMaterial.vDif.w = 0.f;
	//m_tMaterial.vAmb = VEC4ONE;
	//m_tMaterial.vAmb.w = 0.f;
	//m_tMaterial.vSpc = VEC4ONE;
	//m_tMaterial.vEmv = VEC4ZERO;
	//m_tMaterial.vSpc.w = 3.2f;

	memset(&m_tMaterial, 0, sizeof(m_tMaterial));
	SetInfo(VEC3ONE, Vec3(0.2f, 0.2f, 0.2f), VEC3ONE, VEC3ZERO, 3.2f);

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
