#pragma once
#include "../Dx11Object.h"

DX11_BEGIN

class DX11_DLL CDx11ShaderManager :
	public CDx11Object
{
	DX11_DECLARE_SINGLE(CDx11ShaderManager)

private:
	unordered_map<string, class CDx11Shader*>	m_mapShader;

public:
	bool Init();
	class CDx11Shader* LoadShader(const string& strKey,
		TCHAR* pFileName, char* pEntry[ST_MAX], int iHighV, int iLowV,
		const string& strPathKey = SHADER_PATH);
	class CDx11Shader* FindShader(const string& strKey);
	bool AddInputElement(const string& strKey, char* pSemanticName, UINT iSemanticIdx,
		DXGI_FORMAT eFmt, UINT iInputSlot, UINT iSize,
		D3D11_INPUT_CLASSIFICATION eInputClass,
		UINT iInstanceData);
	bool CreateInputLayout(const string& strKey);
};

DX11_END