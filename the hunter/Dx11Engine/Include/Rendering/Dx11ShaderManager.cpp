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
	// VS와 PS를 엔트리 배열에 넣는다.
	// 각 원소는 '정점 쉐이더와 픽셀 쉐이더 함수의 이름이다.
	char* pEntry[ST_MAX] = { "ColorVS", "ColorPS" };

	// 쉐이더 객체를 선언하고 로드한다.
	CDx11Shader*	pShader = LoadShader(DEFAULT_SHADER, L"Default.fx",
		pEntry, 5, 0);

	//입력배치 객체를 정의한다.
	//COLORVERTEX 시멘틱은 POSITION과 NORMAL, COLOR
	AddInputElement(DEFAULT_SHADER, "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,
		0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0);
	AddInputElement(DEFAULT_SHADER, "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT,
		0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0);
	AddInputElement(DEFAULT_SHADER, "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT,
		0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0);

	//상수 버퍼를 생성한다.
	pShader->CreateConstantBuffer("Transform", sizeof(TRANSFORMCBUFFER), 0);
	pShader->CreateConstantBuffer("Material", sizeof(MATERIAL), 1);
	pShader->CreateConstantBuffer("LightInfo", sizeof(LIGHTINFO), 2);
	pShader->CreateConstantBuffer("LightDir", sizeof(Vec4), 3);
	pShader->CreateConstantBuffer("LightPoint", sizeof(Vec4), 4);
	pShader->CreateConstantBuffer("LightSpot", sizeof(SPOTLIGHT), 5);
	pShader->CreateConstantBuffer("FogInfo", sizeof(FOGINFO), 6);

	//입력배치를 생성한다.
	CreateInputLayout(DEFAULT_SHADER);
	SAFE_RELEASE(pShader);


	// 텍스처 쉐이더를 로드한다.
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


	// 범프 쉐이더를 로드한다.
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


	// 이펙트 쉐이더를 로드한다.
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


	// 스카이 쉐이더를 로드한다.

	pEntry[ST_VERTEX] = "SkyVS";
	pEntry[ST_PIXEL] = "SkyPS";
	pShader = LoadShader(SKY_SHADER, L"Sky.fx", pEntry, 5, 0);

	AddInputElement(SKY_SHADER, "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,
		0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0);

	pShader->CreateConstantBuffer("Transform", sizeof(TRANSFORMCBUFFER), 0);

	CreateInputLayout(SKY_SHADER);
	SAFE_RELEASE(pShader);



	// 충돌 쉐이더를 로드한다.

	pEntry[ST_VERTEX] = "ColliderVS";
	pEntry[ST_PIXEL] = "ColliderPS";
	pShader = LoadShader(COLLIDER_SHADER, L"Collider.fx", pEntry, 5, 0);

	AddInputElement(COLLIDER_SHADER, "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,
		0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0);

	pShader->CreateConstantBuffer("Transform", sizeof(TRANSFORMCBUFFER), 0);
	pShader->CreateConstantBuffer("Color", sizeof(Vec4), 1);

	CreateInputLayout(COLLIDER_SHADER);
	SAFE_RELEASE(pShader);

	

	// UI 쉐이더를 로드한다.

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
	//쉐이더를 로드했을 때 이미 있는 쉐이더인지 맵에 이름을 검색하고,
	//이미 있다면 그 객체를 리턴한다.
	CDx11Shader*	pShader = FindShader(strKey);

	if (pShader)
		return pShader;

	//없다면 새로 메모리를 할당하여 만들어준다.
	pShader = new CDx11Shader;

	if (!pShader->LoadShader(pFileName, pEntry, iHighV, iLowV, strPathKey))
	{
		SAFE_RELEASE(pShader);
		return NULL;
	}

	//참조 카운트를 증가시키고, 맵에 삽입해준다.
	pShader->AddRef();
	m_mapShader.insert(make_pair(strKey, pShader));

	//만들어진 쉐이더를 리턴.
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

	//해당 이름으로 만들어진 쉐이더가 없으면 false를 리턴한다.
	if (!pShader)
		return false;

	//쉐이더 클래스의 AddInputLayout 함수를 호출
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
