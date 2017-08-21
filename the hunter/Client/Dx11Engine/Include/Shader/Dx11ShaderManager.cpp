#include "Dx11ShaderManager.h"
#include "Dx11Shader.h"

DX11_USING

DX11_SINGLE_DEFINITION(CDx11ShaderManager)

CDx11ShaderManager::CDx11ShaderManager()
{
	SetTypeName<CDx11ShaderManager>();
}

CDx11ShaderManager::~CDx11ShaderManager()
{
	Safe_Release_Map(m_mapShader);
}

bool CDx11ShaderManager::Init()
{
	// 기본 셰이더 로딩
	CDx11Shader*	pShader = LoadShader(L"Default.fx", "DefaultColorVS",
		"DefaultColorPS", 5.0f, "DefaultShader");

	pShader->AddInputElement("POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,
		0, 12);
	pShader->AddInputElement("COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT,
		0, 16);
	pShader->CreateInputLayout();

	SAFE_RELEASE(pShader);

	// 텍스쳐 셰이더 로딩
	pShader = LoadShader(L"Default.fx", "DefaultTexVS", "DefaultTexPS",
		5.0f, "TexShader");

	pShader->AddInputElement("POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,
		0, 12);
	pShader->AddInputElement("NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT,
		0, 12);
	pShader->AddInputElement("TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,
		0, 8);
	pShader->AddInputElement("TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT,
		0, 12);
	pShader->AddInputElement("BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT,
		0, 12);
	pShader->CreateInputLayout();

	SAFE_RELEASE(pShader);

	// 애니메이션
	pShader = LoadShader(L"Default.fx", "DefaultAnimationTexVS",
		"DefaultTexPS",
		5.0f, "AnimationTexShader");

	pShader->AddInputElement("POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,
		0, 12);
	pShader->AddInputElement("NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT,
		0, 12);
	pShader->AddInputElement("TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,
		0, 8);
	pShader->AddInputElement("TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT,
		0, 12);
	pShader->AddInputElement("BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT,
		0, 12);
	pShader->AddInputElement("WEIGHTS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT,
		0, 16);
	pShader->AddInputElement("BLENDINDICES", 0, DXGI_FORMAT_R32G32B32A32_FLOAT,
		0, 16);
	pShader->CreateInputLayout();

	SAFE_RELEASE(pShader);

	// Collider Shader
	pShader = LoadShader(L"Collider.fx", "ColliderVS", "ColliderPS",
		5.0f, "ColliderShader");

	pShader->AddInputElement("POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,
		0, 12);
	pShader->CreateInputLayout();

	pShader->CreateConstantBuffer("Color", sizeof(Vec4), 6);

	SAFE_RELEASE(pShader);

	// Sky Shader
	pShader = LoadShader(L"Sky.fx", "SkyVS", "SkyPS",
		5.0f, "SkyShader");

	pShader->AddInputElement("POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,
		0, 12);
	pShader->CreateInputLayout();

	SAFE_RELEASE(pShader);

	// UI Shader
	pShader = LoadShader(L"UI.fx", "UIVS", "UIPS",
		5.0f, UISHADER);

	pShader->AddInputElement("POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,
		0, 12);
	pShader->AddInputElement("TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,
		0, 8);
	pShader->CreateInputLayout();

	SAFE_RELEASE(pShader);

	// UI Button Shader
	pShader = LoadShader(L"UI.fx", "UIVS", "UIButtonPS",
		5.0f, UIBUTTONSHADER);

	pShader->AddInputElement("POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,
		0, 12);
	pShader->AddInputElement("TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,
		0, 8);
	pShader->CreateInputLayout();

	SAFE_RELEASE(pShader);

	// Effect Shader
	pShader = LoadShader(L"Effect.fx", "EffectVS", "EffectPS",
		5.0f, EFFECTSHADER);

	pShader->AddInputElement("POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,
		0, 12);
	pShader->AddInputElement("TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,
		0, 8);
	pShader->CreateInputLayout();

	SAFE_RELEASE(pShader);

	// Effect Atlas Shader
	pShader = LoadShader(L"Effect.fx", "EffectAtlasVS", "EffectPS",
		5.0f, "EffectAtlasShader");

	pShader->AddInputElement("POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,
		0, 12);
	pShader->AddInputElement("TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,
		0, 8);
	pShader->CreateInputLayout();

	pShader->CreateConstantBuffer("UVFrame", sizeof(UVFRAME), 6);

	SAFE_RELEASE(pShader);

	return true;
}

CDx11Shader * CDx11ShaderManager::LoadShader(TCHAR * pFileName, char* pVSEntry, char* pPSEntry, float fVersion, 
	const string& strKey,
	const string & strPathKey)
{
	CDx11Shader*	pShader = FindShader(strKey);

	if (pShader)
		return pShader;

	pShader = new CDx11Shader;

	if (!pShader->LoadShader(pFileName, pVSEntry, pPSEntry, fVersion, strPathKey))
	{
		SAFE_RELEASE(pShader);
		return NULL;
	}

	pShader->AddRef();
	m_mapShader.insert(make_pair(strKey, pShader));

	return pShader;
}

CDx11Shader * CDx11ShaderManager::FindShader(const string & strKey)
{
	unordered_map<string, CDx11Shader*>::iterator	iter = m_mapShader.find(strKey);

	if (iter == m_mapShader.end())
		return NULL;

	iter->second->AddRef();

	return iter->second;
}
