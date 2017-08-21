#pragma once
#include "../Dx11Object.h"

DX11_BEGIN

class DX11_DLL CDx11ShaderManager :
	public CDx11Object
{
	DX11_SINGLE_DECLARE(CDx11ShaderManager)

private:
	unordered_map<string, class CDx11Shader*>	m_mapShader;

public:
	bool Init();
	CDx11Shader* LoadShader(TCHAR* pFileName, char* pVSEntry, char* pPSEntry, float fVersion, 
		const string& strKey, const string& strPathKey = DX11_SHADER_PATH);
	CDx11Shader* FindShader(const string& strKey);
};

DX11_END
