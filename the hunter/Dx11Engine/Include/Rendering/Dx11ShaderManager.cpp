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
	// VS�� PS�� ��Ʈ�� �迭�� �ִ´�.
	// �� ���Ҵ� '���� ���̴��� �ȼ� ���̴� �Լ��� �̸��̴�.
	char* pEntry[ST_MAX] = { "ColorVS", "ColorPS" };

	// ���̴� ��ü�� �����ϰ� �ε��Ѵ�.
	CDx11Shader*	pShader = LoadShader(DEFAULT_SHADER, L"Default.fx",
		pEntry, 5, 0);

	//�Է¹�ġ ��ü�� �����Ѵ�.
	//COLORVERTEX �ø�ƽ�� POSITION�� NORMAL, COLOR
	AddInputElement(DEFAULT_SHADER, "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,
		0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0);
	AddInputElement(DEFAULT_SHADER, "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT,
		0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0);
	AddInputElement(DEFAULT_SHADER, "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT,
		0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0);

	//��� ���۸� �����Ѵ�.
	pShader->CreateConstantBuffer("Transform", sizeof(TRANSFORMCBUFFER), 0);
	pShader->CreateConstantBuffer("Material", sizeof(MATERIAL), 1);
	pShader->CreateConstantBuffer("LightInfo", sizeof(LIGHTINFO), 2);
	pShader->CreateConstantBuffer("LightDir", sizeof(Vec4), 3);
	pShader->CreateConstantBuffer("LightPoint", sizeof(Vec4), 4);
	pShader->CreateConstantBuffer("LightSpot", sizeof(SPOTLIGHT), 5);
	pShader->CreateConstantBuffer("FogInfo", sizeof(FOGINFO), 6);

	//�Է¹�ġ�� �����Ѵ�.
	CreateInputLayout(DEFAULT_SHADER);
	SAFE_RELEASE(pShader);


	// �ؽ�ó ���̴��� �ε��Ѵ�.
	pEntry[ST_VERTEX] = "DefaultTexVS";
	pEntry[ST_PIXEL] = "DefaultTexPS";
	pShader = LoadShader(DEFAULT_TEX_SHADER, L"Default.fx", pEntry, 5, 0);

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
	pShader->CreateConstantBuffer("LightSpot", sizeof(SPOTLIGHT), 5);
	pShader->CreateConstantBuffer("FogInfo", sizeof(FOGINFO), 6);

	CreateInputLayout(DEFAULT_TEX_SHADER);
	SAFE_RELEASE(pShader);


	// ���� ���̴��� �ε��Ѵ�.
	pEntry[ST_VERTEX] = "DefaultBumpVS";
	pEntry[ST_PIXEL] = "DefaultBumpPS";
	pShader = LoadShader(DEFAULT_BUMP_SHADER, L"Default.fx", pEntry, 5, 0);

	AddInputElement(DEFAULT_BUMP_SHADER, "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,
		0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0);
	AddInputElement(DEFAULT_BUMP_SHADER, "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT,
		0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0);
	AddInputElement(DEFAULT_BUMP_SHADER, "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,
		0, 8, D3D11_INPUT_PER_VERTEX_DATA, 0);
	AddInputElement(DEFAULT_BUMP_SHADER, "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT,
		0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0);
	AddInputElement(DEFAULT_BUMP_SHADER, "BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT,
		0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0);


	pShader->CreateConstantBuffer("Transform", sizeof(TRANSFORMCBUFFER), 0);
	pShader->CreateConstantBuffer("Material", sizeof(MATERIAL), 1);
	pShader->CreateConstantBuffer("LightInfo", sizeof(LIGHTINFO), 2);
	pShader->CreateConstantBuffer("LightDir", sizeof(Vec4), 3);
	pShader->CreateConstantBuffer("LightPoint", sizeof(Vec4), 4);
	pShader->CreateConstantBuffer("LightSpot", sizeof(SPOTLIGHT), 5);
	pShader->CreateConstantBuffer("FogInfo", sizeof(FOGINFO), 6);

	CreateInputLayout(DEFAULT_BUMP_SHADER);
	SAFE_RELEASE(pShader);


	// ����Ʈ ���̴��� �ε��Ѵ�.
	pEntry[ST_VERTEX] = "EffectVS";
	pEntry[ST_PIXEL] = "EffectPS";
	pShader = LoadShader(EFFECT_SHADER, L"Effect.fx", pEntry, 5, 0);
	
	AddInputElement(EFFECT_SHADER, "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,
		0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0);
	AddInputElement(EFFECT_SHADER, "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,
		0, 8, D3D11_INPUT_PER_VERTEX_DATA, 0);

	pShader->CreateConstantBuffer("Transform", sizeof(TRANSFORMCBUFFER), 0);
	pShader->CreateConstantBuffer("UVFrame", sizeof(UVFRAME), 1);
	
	CreateInputLayout(EFFECT_SHADER);
	SAFE_RELEASE(pShader);


	// ��ī�� ���̴��� �ε��Ѵ�.

	pEntry[ST_VERTEX] = "SkyVS";
	pEntry[ST_PIXEL] = "SkyPS";
	pShader = LoadShader(SKY_SHADER, L"Sky.fx", pEntry, 5, 0);

	AddInputElement(SKY_SHADER, "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,
		0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0);

	pShader->CreateConstantBuffer("Transform", sizeof(TRANSFORMCBUFFER), 0);

	CreateInputLayout(SKY_SHADER);
	SAFE_RELEASE(pShader);



	// �浹 ���̴��� �ε��Ѵ�.

	pEntry[ST_VERTEX] = "ColliderVS";
	pEntry[ST_PIXEL] = "ColliderPS";
	pShader = LoadShader(COLLIDER_SHADER, L"Collider.fx", pEntry, 5, 0);

	AddInputElement(COLLIDER_SHADER, "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,
		0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0);

	pShader->CreateConstantBuffer("Transform", sizeof(TRANSFORMCBUFFER), 0);
	pShader->CreateConstantBuffer("Color", sizeof(Vec4), 1);

	CreateInputLayout(COLLIDER_SHADER);
	SAFE_RELEASE(pShader);

	

	// UI ���̴��� �ε��Ѵ�.

	pEntry[ST_VERTEX] = "UIVS";
	pEntry[ST_PIXEL] = "UIPS";
	pShader = LoadShader(UI_SHADER, L"UI.fx", pEntry, 5, 0);

	AddInputElement(UI_SHADER, "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,
		0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0);
	AddInputElement(UI_SHADER, "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,
		0, 8, D3D11_INPUT_PER_VERTEX_DATA, 0);

	pShader->CreateConstantBuffer("Transform", sizeof(TRANSFORMCBUFFER), 0);
	pShader->CreateConstantBuffer("Material", sizeof(MATERIAL), 1);

	CreateInputLayout(UI_SHADER);
	SAFE_RELEASE(pShader);

	return true;
}

CDx11Shader * CDx11ShaderManager::LoadShader(const string & strKey,
	TCHAR * pFileName, char * pEntry[ST_MAX], int iHighV, int iLowV,
	const string & strPathKey)
{
	//���̴��� �ε����� �� �̹� �ִ� ���̴����� �ʿ� �̸��� �˻��ϰ�,
	//�̹� �ִٸ� �� ��ü�� �����Ѵ�.
	CDx11Shader*	pShader = FindShader(strKey);

	if (pShader)
		return pShader;

	//���ٸ� ���� �޸𸮸� �Ҵ��Ͽ� ������ش�.
	pShader = new CDx11Shader;

	if (!pShader->LoadShader(pFileName, pEntry, iHighV, iLowV, strPathKey))
	{
		SAFE_RELEASE(pShader);
		return NULL;
	}

	//���� ī��Ʈ�� ������Ű��, �ʿ� �������ش�.
	pShader->AddRef();
	m_mapShader.insert(make_pair(strKey, pShader));

	//������� ���̴��� ����.
	return pShader;
}

CDx11Shader * CDx11ShaderManager::FindShader(const string & strKey)
{
	unordered_map<string, class CDx11Shader*>::iterator iter = m_mapShader.find(strKey);
	
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

	//�ش� �̸����� ������� ���̴��� ������ false�� �����Ѵ�.
	if (!pShader)
		return false;

	//���̴� Ŭ������ AddInputLayout �Լ��� ȣ��
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
