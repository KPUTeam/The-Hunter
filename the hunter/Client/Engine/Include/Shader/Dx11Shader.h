#pragma once
#include "../Dx11Object.h"

DX11_BEGIN

class DX11_DLL CDx11Shader :
	public CDx11Object
{
private:
	friend class CDx11ShaderManager;

private:
	CDx11Shader();
	~CDx11Shader();

private:
	typedef struct _tagCBufferInfo
	{
		ID3D11Buffer*	pBuffer;
		UINT			iSize;
		UINT			iRegister;
	}CBUFFERINFO, *PCBUFFERINFO;

private:
	ID3DBlob*			m_pVSBlob;
	ID3DBlob*			m_pPSBlob;
	ID3D11VertexShader*	m_pVS;
	ID3D11PixelShader*	m_pPS;
	ID3D11InputLayout*	m_pInputLayout;
	D3D11_INPUT_ELEMENT_DESC*	m_pElementDesc;
	UINT						m_iElementCount;
	UINT						m_iElementSize;
	UINT						m_iByteOffset;
	unordered_map<string, PCBUFFERINFO>	m_mapCBuffer;
	UINT						m_iVSCBufferIdx;
	UINT						m_iPSCBufferIdx;

public:
	bool LoadShader(TCHAR* pFileName, char* pVSEntry, char* pPSEntry, float fVersion, const string& strPathKey);
	bool LoadVertexShader(TCHAR* pFileName, char* pEntryPoint, float fVersion, const string& strPathKey);
	bool LoadPixelShader(TCHAR* pFileName, char* pEntryPoint, float fVersion, const string& strPathKey);
	void AddInputElement(char* pSemantic, UINT iSemanticIdx, DXGI_FORMAT eFmt,
		UINT iInputSlot, UINT iSize);
	bool CreateInputLayout();
	void SetShader();
	bool CreateConstantBuffer(const string& strKey, UINT iBufferSize, 
		UINT iRegister);
	bool UpdateConstantBuffer(const string& strKey, void* pData,
		SHADER_TYPE eType);
	void Begin();
	void End();

private:
	PCBUFFERINFO FindCBuffer(const string& strKey);
};

DX11_END
