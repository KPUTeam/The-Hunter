#include "Dx11ShaderManager.h"
#include "Dx11Shader.h"

DX11_USING

DX11_DEFINITION_SINGLE(CDx11ShaderManager)

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
	char*	pEntry[ST_MAX] = {"ColorVS", "ColorPS"};
	CDx11Shader*	pShader	= LoadShader(DEFAULT_SHADER, L"Default.fx",
		pEntry, 5, 0);
	
	AddInputElement(DEFAULT_SHADER, "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,
		0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0);
	AddInputElement(DEFAULT_SHADER, "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT,
		0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0);

	pShader->CreateConstantBuffer("Transform", sizeof(TRANSFORMCBUFFER), 0);

	CreateInputLayout(DEFAULT_SHADER);

	SAFE_RELEASE(pShader);

	// Tex Shader
	pEntry[ST_VERTEX] = "DefaultTexVS";
	pEntry[ST_PIXEL] = "DefaultTexPS";
	pShader = LoadShader(DEFAULT_TEX_SHADER, L"Default.fx",
		pEntry, 5, 0);

	AddInputElement(DEFAULT_TEX_SHADER, "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,
		0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0);
	AddInputElement(DEFAULT_TEX_SHADER, "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT,
		0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0);
	AddInputElement(DEFAULT_TEX_SHADER, "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,
		0, 8, D3D11_INPUT_PER_VERTEX_DATA, 0);

	pShader->CreateConstantBuffer("Transform", sizeof(TRANSFORMCBUFFER), 0);
	pShader->CreateConstantBuffer("Material", sizeof(MATERIAL), 1);
	pShader->CreateConstantBuffer("LightInfo", sizeof(LIGHTINFO), 2);
	pShader->CreateConstantBuffer("LightDir", sizeof(Vec4), 3);
	pShader->CreateConstantBuffer("LightPoint", sizeof(Vec4), 4);

	CreateInputLayout(DEFAULT_TEX_SHADER);

	SAFE_RELEASE(pShader);

	// Sky Shader
	pEntry[ST_VERTEX] = "SkyVS";
	pEntry[ST_PIXEL] = "SkyPS";
	pShader = LoadShader(SKY_SHADER, L"Sky.fx",
		pEntry, 5, 0);

	AddInputElement(SKY_SHADER, "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,
		0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0);

	pShader->CreateConstantBuffer("Transform", sizeof(TRANSFORMCBUFFER), 0);

	CreateInputLayout(SKY_SHADER);

	SAFE_RELEASE(pShader);

	return true;
}

CDx11Shader * CDx11ShaderManager::LoadShader(const string & strKey, 
	TCHAR * pFileName, char * pEntry[ST_MAX], int iHighV, int iLowV, 
	const string & strPathKey)
{
	CDx11Shader*	pShader = FindShader(strKey);

	if (pShader)
		return pShader;

	pShader = new CDx11Shader;

	if (!pShader->LoadShader(pFileName, pEntry, iHighV, iLowV, strPathKey))
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
	unordered_map<string, class CDx11Shader*>::iterator	iter = m_mapShader.find(strKey);

	if (iter == m_mapShader.end())
		return NULL;

	iter->second->AddRef();

	return iter->second;
}

bool CDx11ShaderManager::AddInputElement(const string & strKey, 
	char * pSemanticName, UINT iSemanticIdx, DXGI_FORMAT eFmt, 
	UINT iInputSlot, UINT iSize, D3D11_INPUT_CLASSIFICATION eInputClass, 
	UINT iInstanceData)
{
	CDx11Shader*	pShader = FindShader(strKey);

	if (!pShader)
		return false;

	pShader->AddInputLayout(pSemanticName, iSemanticIdx, eFmt,
		iInputSlot, iSize, eInputClass, iInstanceData);

	SAFE_RELEASE(pShader);

	return true;
}

bool CDx11ShaderManager::CreateInputLayout(const string & strKey)
{
	CDx11Shader*	pShader = FindShader(strKey);

	if (!pShader)
		return false;

	if (!pShader->CreateInputLayout())
	{
		SAFE_RELEASE(pShader);
		return false;
	}

	SAFE_RELEASE(pShader);

	return true;
}
