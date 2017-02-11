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
	D3D11_INPUT_ELEMENT_DESC*	m_pElement;
	ID3D11InputLayout*			m_pInputLayout;
	UINT						m_iElementCount;
	UINT						m_iElementCountMax;
	UINT						m_iElementOffset;

private:
	ID3DBlob*			m_pVSBlob;
	ID3D11VertexShader*	m_pVS;
	ID3DBlob*			m_pPSBlob;
	ID3D11PixelShader*	m_pPS;

private:
	typedef struct _tagCBufferInfo
	{
		ID3D11Buffer*	pBuffer;
		UINT			iSize;
		UINT			iRegister;
	}CBUFFERINFO, *PCBUFFERINFO;
	unordered_map<string, PCBUFFERINFO>	m_mapConstantBuffer;

public:
	bool LoadShader(TCHAR* pFileName, char* pEntry[ST_MAX], int iHighV, int iLowV,
		const string& strPathKey = SHADER_PATH);
	void AddInputLayout(char* pSemanticName, UINT iSemanticIdx,
		DXGI_FORMAT eFmt, UINT iInputSlot, UINT iSize,
		D3D11_INPUT_CLASSIFICATION eInputClass,
		UINT iInstanceData);
	bool CreateInputLayout();
	void SetShader();
	bool CreateConstantBuffer(const string& strKey, UINT iSize, UINT iRegister);
	bool UpdateConstantBuffer(const string& strKey, void* pData,
		SHADER_TYPE eType);
	void End();

private:
	bool LoadVertexShader(TCHAR* pFileName, char* pEntry, int iHighV,
		int iLowV, const string& strPathKey);
	bool LoadPixelShader(TCHAR* pFileName, char* pEntry, int iHighV,
		int iLowV, const string& strPathKey);
	PCBUFFERINFO FindConstantBuffer(const string& strKey);
};

DX11_END