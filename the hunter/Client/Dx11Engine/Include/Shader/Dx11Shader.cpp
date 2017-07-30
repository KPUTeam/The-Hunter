#include "Dx11Shader.h"
#include "../Core/Dx11PathManager.h"
#include "../Core/Dx11Device.h"

DX11_USING

CDx11Shader::CDx11Shader()	:
	m_pVSBlob(NULL),
	m_pPSBlob(NULL),
	m_pVS(NULL),
	m_pPS(NULL),
	m_pElementDesc(NULL),
	m_pInputLayout(NULL),
	m_iElementCount(0),
	m_iElementSize(0),
	m_iByteOffset(0),
	m_iVSCBufferIdx(0),
	m_iPSCBufferIdx(0)
{
	SetTypeName<CDx11Shader>();
}

CDx11Shader::~CDx11Shader()
{
	//Safe_Release_Map(m_mapCBuffer);
	for (unordered_map<string, PCBUFFERINFO>::iterator iter = m_mapCBuffer.begin();
		iter != m_mapCBuffer.end(); ++iter)
	{
		SAFE_RELEASE(iter->second->pBuffer);
		SAFE_DELETE(iter->second);
	}

	m_mapCBuffer.clear();

	SAFE_DELETE_ARRAY(m_pElementDesc);
	SAFE_RELEASE(m_pInputLayout);
	SAFE_RELEASE(m_pVS);
	SAFE_RELEASE(m_pPS);
	SAFE_RELEASE(m_pPSBlob);
	SAFE_RELEASE(m_pVSBlob);
}

bool CDx11Shader::LoadShader(TCHAR * pFileName, char* pVSEntry, char* pPSEntry, float fVersion, 
	const string & strPathKey)
{
	if (!LoadVertexShader(pFileName, pVSEntry, fVersion, strPathKey))
		return false;

	if (!LoadPixelShader(pFileName, pPSEntry, fVersion, strPathKey))
		return false;

	// 어떤 쉐이더든 Transform 정보는 무조건 있어야 하므로 상수버퍼를
	// 미리 생성해둔다.
	CreateConstantBuffer("Transform", sizeof(TRANSFORMCBUFFER), 0);
	CreateConstantBuffer("LightInfo", sizeof(LIGHTINFO), 1);
	CreateConstantBuffer("LightDir", sizeof(Vec4), 2);
	CreateConstantBuffer("LightPoint", sizeof(LIGHTPOINT), 3);
	CreateConstantBuffer("Material", sizeof(MATERIAL), 4);
	CreateConstantBuffer("BoneArray", sizeof(BONECBUFFER), 5);

	return true;
}

bool CDx11Shader::LoadVertexShader(TCHAR * pFileName, char* pEntryPoint, float fVersion, 
	const string & strPathKey)
{
	UINT	iFlag = 0;

#if defined(DEBUG) | defined(_DEBUG1)
	iFlag |= D3D10_SHADER_DEBUG;
#endif

	// 파일 경로
	wstring	strPath = DX11_GET_SINGLE(CDx11PathManager)->FindPath(strPathKey);
	strPath += pFileName;

	char	strVersion[32] = {};
	sprintf_s(strVersion, "vs_%d_%d", (int)fVersion, (int)(fVersion * 10) % 10);

	ID3DBlob*	pErr = NULL;
	if (FAILED(D3DCompileFromFile(strPath.c_str(), NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, pEntryPoint, strVersion,
		iFlag, 0, &m_pVSBlob, &pErr)))
	{
		MessageBoxA(NULL, (char*)pErr->GetBufferPointer(), "Err", MB_OK);
		return false;
	}

	if (FAILED(DX11_DEVICE->CreateVertexShader(m_pVSBlob->GetBufferPointer(),
		m_pVSBlob->GetBufferSize(), NULL, &m_pVS)))
		return false;

	return true;
}

bool CDx11Shader::LoadPixelShader(TCHAR * pFileName, char* pEntryPoint, float fVersion, 
	const string & strPathKey)
{
	UINT	iFlag = 0;

#if defined(DEBUG) | defined(_DEBUG1)
	iFlag |= D3D10_SHADER_DEBUG;
#endif

	// 파일 경로
	wstring	strPath = DX11_GET_SINGLE(CDx11PathManager)->FindPath(strPathKey);
	strPath += pFileName;

	char	strVersion[32] = {};
	sprintf_s(strVersion, "ps_%d_%d", (int)fVersion, (int)(fVersion * 10) % 10);

	ID3DBlob*	pErr = NULL;
	if (FAILED(D3DCompileFromFile(strPath.c_str(), NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, pEntryPoint, strVersion,
		iFlag, 0, &m_pPSBlob, &pErr)))
		return false;

	if (FAILED(DX11_DEVICE->CreatePixelShader(m_pPSBlob->GetBufferPointer(),
		m_pPSBlob->GetBufferSize(), NULL, &m_pPS)))
		return false;

	return true;
}

void CDx11Shader::AddInputElement(char * pSemantic, UINT iSemanticIdx, DXGI_FORMAT eFmt, UINT iInputSlot, UINT iSize)
{
	if (m_iElementSize == 0)
	{
		m_iElementSize = 2;
		m_pElementDesc = new D3D11_INPUT_ELEMENT_DESC[m_iElementSize];
		memset(m_pElementDesc, 0, sizeof(D3D11_INPUT_ELEMENT_DESC) * m_iElementSize);
	}

	else if (m_iElementCount == m_iElementSize)
	{
		m_iElementSize *= 2;
		D3D11_INPUT_ELEMENT_DESC*	pElement = new D3D11_INPUT_ELEMENT_DESC[m_iElementSize];
		memcpy(pElement, m_pElementDesc, sizeof(D3D11_INPUT_ELEMENT_DESC) * m_iElementCount);
		SAFE_DELETE_ARRAY(m_pElementDesc);

		m_pElementDesc = pElement;
	}

	m_pElementDesc[m_iElementCount].SemanticName = pSemantic;
	m_pElementDesc[m_iElementCount].SemanticIndex = iSemanticIdx;
	m_pElementDesc[m_iElementCount].Format = eFmt;
	m_pElementDesc[m_iElementCount].AlignedByteOffset = m_iByteOffset;
	m_pElementDesc[m_iElementCount].InputSlot = iInputSlot;
	m_pElementDesc[m_iElementCount].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	m_pElementDesc[m_iElementCount].InstanceDataStepRate = 0;
	
	m_iByteOffset += iSize;
	++m_iElementCount;
}

bool CDx11Shader::CreateInputLayout()
{
	if (FAILED(DX11_DEVICE->CreateInputLayout(m_pElementDesc, m_iElementCount,
		m_pVSBlob->GetBufferPointer(), m_pVSBlob->GetBufferSize(),
		&m_pInputLayout)))
		return false;

	return true;
}

void CDx11Shader::SetShader()
{
	DX11_CONTEXT->VSSetShader(m_pVS, NULL, 0);
	DX11_CONTEXT->PSSetShader(m_pPS, NULL, 0);
	DX11_CONTEXT->IASetInputLayout(m_pInputLayout);
}

bool CDx11Shader::CreateConstantBuffer(const string & strKey, 
	UINT iBufferSize, UINT iRegister)
{
	PCBUFFERINFO	pBuffer = FindCBuffer(strKey);

	if (pBuffer)
		return false;

	pBuffer = new CBUFFERINFO;

	pBuffer->iSize = iBufferSize;

	for (unordered_map<string, PCBUFFERINFO>::iterator iter = m_mapCBuffer.begin();
	iter != m_mapCBuffer.end(); ++iter)
	{
		if (iter->second->iRegister == iRegister)
		{
			SAFE_DELETE(pBuffer);
			return false;
		}
	}

	pBuffer->iRegister = iRegister;

	D3D11_BUFFER_DESC	tDesc = {};
	tDesc.Usage = D3D11_USAGE_DYNAMIC;
	tDesc.ByteWidth = iBufferSize;
	tDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	tDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	if (FAILED(DX11_DEVICE->CreateBuffer(&tDesc, NULL, &pBuffer->pBuffer)))
	{
		return false;
	}

	m_mapCBuffer.insert(make_pair(strKey, pBuffer));

	return true;
}

bool CDx11Shader::UpdateConstantBuffer(const string & strKey, 
	void * pData, SHADER_TYPE eType)
{
	PCBUFFERINFO	pBuffer = FindCBuffer(strKey);

	if (!pBuffer)
	{
		return false;
	}

	D3D11_MAPPED_SUBRESOURCE	tMapSr;

	//DX11_CONTEXT->UpdateSubresource(pBuffer, 0, NULL, pData, 0, 0);
	DX11_CONTEXT->Map(pBuffer->pBuffer, 0, D3D11_MAP_WRITE_DISCARD,
		0, &tMapSr);

	memcpy(tMapSr.pData, pData, pBuffer->iSize);

	DX11_CONTEXT->Unmap(pBuffer->pBuffer, 0);

	switch (eType)
	{
	case ST_VERTEX:
		DX11_CONTEXT->VSSetConstantBuffers(pBuffer->iRegister, 1, &pBuffer->pBuffer);
		break;
	case ST_PIXEL:
		DX11_CONTEXT->PSSetConstantBuffers(pBuffer->iRegister, 1, &pBuffer->pBuffer);
		break;
	}

	return true;
}

void CDx11Shader::Begin()
{
}

void CDx11Shader::End()
{
	m_iVSCBufferIdx = 0;
	m_iPSCBufferIdx = 0;
}

CDx11Shader::PCBUFFERINFO CDx11Shader::FindCBuffer(const string & strKey)
{
	unordered_map<string, PCBUFFERINFO>::iterator	iter = m_mapCBuffer.find(strKey);

	if (iter == m_mapCBuffer.end())
		return NULL;

	return iter->second;
}
