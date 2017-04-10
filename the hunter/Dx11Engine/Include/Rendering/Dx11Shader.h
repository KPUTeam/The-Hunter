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
	ID3DBlob*					m_pVSBlob;			//파일에서 로드된 정점 쉐이더 정보를 블록에 저장한다.
	ID3D11VertexShader*			m_pVS;				//Blob의 데이터를 이용해 정점 쉐이더를 만든다.
	ID3DBlob*					m_pPSBlob;
	ID3D11PixelShader*			m_pPS;

private:
//	버퍼와 버퍼의 사이즈, 레지스터 번호를 묶어놓기
	typedef struct _tagCBufferInfo
	{
		ID3D11Buffer*	pBuffer;
		UINT			iSize;
		UINT			iRegister;
	}CBUFFERINFO, *PCBUFFERINFO;

//	상수버퍼는 맵으로 관리
	unordered_map<string, PCBUFFERINFO> m_mapConstantBuffer;
	//UINT		m_iVertexConstantSlot;
	//UINT		m_iPixelConstantSlot;

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