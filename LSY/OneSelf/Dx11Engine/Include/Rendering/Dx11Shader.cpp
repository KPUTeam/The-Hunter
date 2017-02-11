#include "Dx11Shader.h"
#include "../Core/Dx11Device.h"
#include "../Core/Dx11FilePathManager.h"

DX11_USING

CDx11Shader::CDx11Shader()	:
	m_pElement(NULL),
	m_pVSBlob(NULL),
	m_pVS(NULL),
	m_pPSBlob(NULL),
	m_pPS(NULL),
	m_pInputLayout(NULL),
	m_iElementCount(0),
	m_iElementOffset(0),
	m_iElementCountMax(0)
{
	SetTypeName<CDx11Shader>();
}

CDx11Shader::~CDx11Shader()
{
	unordered_map<string, PCBUFFERINFO>::iterator	iter;
	for (iter = m_mapConstantBuffer.begin(); iter != m_mapConstantBuffer.end();
		++iter)
	{
		SAFE_RELEASE(iter->second->pBuffer);
		SAFE_DELETE(iter->second);
	}

	m_mapConstantBuffer.clear();

	SAFE_RELEASE(m_pInputLayout);
	SAFE_RELEASE(m_pVSBlob);
	SAFE_RELEASE(m_pVS);
	SAFE_RELEASE(m_pPSBlob);
	SAFE_RELEASE(m_pPS);
	SAFE_DELETE_ARRAY(m_pElement);
}

bool CDx11Shader::LoadShader(TCHAR * pFileName, char * pEntry[ST_MAX], int iHighV,
	int iLowV, const string & strPathKey)
{
	// ¹öÅØ½º ¼ÎÀÌ´õ ·Îµù
	if (!LoadVertexShader(pFileName, pEntry[ST_VERTEX], iHighV, iLowV, strPathKey))
		return false;

	// ÇÈ¼¿ ¼ÎÀÌ´õ ·Îµù
	if (!LoadPixelShader(pFileName, pEntry[ST_PIXEL], iHighV, iLowV, strPathKey))
		return false;

	return true;
}

void CDx11Shader::AddInputLayout(char * pSemanticName, UINT iSemanticIdx,
	DXGI_FORMAT eFmt, UINT iInputSlot, UINT iSize, 
	D3D11_INPUT_CLASSIFICATION eInputClass, UINT iInstanceData)
{
	if (!m_pElement)
	{
		m_iElementCountMax = 2;
		m_pElement = new D3D11_INPUT_ELEMENT_DESC[m_iElementCountMax];
		memset(m_pElement, 0, sizeof(D3D11_INPUT_ELEMENT_DESC) * m_iElementCountMax);
	}

	else if (m_iElementCount == m_iElementCountMax)
	{
		m_iElementCountMax *= 2;

		D3D11_INPUT_ELEMENT_DESC*	pElement = NULL;
		pElement	= new D3D11_INPUT_ELEMENT_DESC[m_iElementCountMax];

		memcpy(pElement, m_pElement, sizeof(D3D11_INPUT_ELEMENT_DESC) * m_iElementCount);
		SAFE_DELETE_ARRAY(m_pElement);

		m_pElement = pElement;
	}
	
	m_pElement[m_iElementCount].SemanticName = pSemanticName;
	m_pElement[m_iElementCount].SemanticIndex = iSemanticIdx;
	m_pElement[m_iElementCount].InputSlot = iInputSlot;
	m_pElement[m_iElementCount].Format = eFmt;
	m_pElement[m_iElementCount].AlignedByteOffset = m_iElementOffset;
	m_pElement[m_iElementCount].InputSlotClass = eInputClass;
	m_pElement[m_iElementCount].InstanceDataStepRate = iInstanceData;

	++m_iElementCount;
	m_iElementOffset += iSize;
}

bool CDx11Shader::CreateInputLayout()
{
	if (FAILED(DX11_DEVICE->CreateInputLayout(m_pElement, m_iElementCount,
		m_pVSBlob->GetBufferPointer(), m_pVSBlob->GetBufferSize(),
		&m_pInputLayout)))
		return false;

	return true;
}

void CDx11Shader::SetShader()
{
	DX11_CONTEXT->IASetInputLayout(m_pInputLayout);
	DX11_CONTEXT->VSSetShader(m_pVS, NULL, 0);
	DX11_CONTEXT->PSSetShader(m_pPS, NULL, 0);
}

bool CDx11Shader::CreateConstantBuffer(const string& strKey, UINT iSize, 
	UINT iRegister)
{
	PCBUFFERINFO	pBuffer = FindConstantBuffer(strKey);

	if (pBuffer)
		return false;

	pBuffer = new CBUFFERINFO;

	memset(pBuffer, 0, sizeof(CBUFFERINFO));

	pBuffer->iSize = iSize;

	unordered_map<string, PCBUFFERINFO>::iterator	iter;
	unordered_map<string, PCBUFFERINFO>::iterator	iterEnd = m_mapConstantBuffer.end();
	for (iter = m_mapConstantBuffer.begin(); iter != iterEnd; ++iter)
	{
		if (iter->second->iRegister == iRegister)
		{
			SAFE_DELETE(pBuffer);
			return false;
		}
	}

	pBuffer->iRegister = iRegister;

	D3D11_BUFFER_DESC	tBuffer = {};

	tBuffer.ByteWidth = iSize;
	tBuffer.Usage = D3D11_USAGE_DYNAMIC;
	tBuffer.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	tBuffer.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	if (FAILED(DX11_DEVICE->CreateBuffer(&tBuffer, NULL, &pBuffer->pBuffer)))
		return false;

	m_mapConstantBuffer.insert(make_pair(strKey, pBuffer));

	return true;
}

bool CDx11Shader::UpdateConstantBuffer(const string & strKey, void * pData,
	SHADER_TYPE eType)
{
	PCBUFFERINFO	pBuffer = FindConstantBuffer(strKey);

	if (!pBuffer)
		return false;

	D3D11_MAPPED_SUBRESOURCE	tMap = {};
	DX11_CONTEXT->Map(pBuffer->pBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &tMap);

	memcpy(tMap.pData, pData, pBuffer->iSize);

	DX11_CONTEXT->Unmap(pBuffer->pBuffer, 0);

	switch (eType)
	{
	case ST_VERTEX:
		DX11_CONTEXT->VSSetConstantBuffers(pBuffer->iRegister, 1,
			&pBuffer->pBuffer);
		break;
	case ST_PIXEL:
		DX11_CONTEXT->PSSetConstantBuffers(pBuffer->iRegister, 1,
			&pBuffer->pBuffer);
		break;
	}

	return true;
}

void CDx11Shader::End()
{
}

bool CDx11Shader::LoadVertexShader(TCHAR * pFileName, char * pEntry, 
	int iHighV, int iLowV, const string & strPathKey)
{
	SAFE_RELEASE(m_pVSBlob);
	SAFE_RELEASE(m_pVS);

	DWORD	dwFlag = 0;

#if defined(_DEBUG) | defined(DEBUG)
	dwFlag = D3DCOMPILE_DEBUG;
#endif // defined(_DEBUG) | defined(DEBUG)

	wstring	strPath = DX11_GET_SINGLE(CDx11FilePathManager)->FindPath(strPathKey);
	strPath += pFileName;

	char	strVersion[32] = {};
	sprintf_s(strVersion, "vs_%d_%d", iHighV, iLowV);

	ID3DBlob*	pErr = NULL;
	if (FAILED(D3DCompileFromFile(strPath.c_str(), NULL, NULL, pEntry,
		strVersion, dwFlag, 0, &m_pVSBlob, &pErr)))
	{
		MessageBoxA(NULL, (char*)pErr->GetBufferPointer(), "Error", MB_OK);
		SAFE_RELEASE(pErr);
		return false;
	}

	if (FAILED(DX11_DEVICE->CreateVertexShader(m_pVSBlob->GetBufferPointer(),
		m_pVSBlob->GetBufferSize(), NULL, &m_pVS)))
		return false;

	return true;
}

bool CDx11Shader::LoadPixelShader(TCHAR * pFileName, char * pEntry, int iHighV, int iLowV, const string & strPathKey)
{
	SAFE_RELEASE(m_pPSBlob);
	SAFE_RELEASE(m_pPS);

	DWORD	dwFlag = 0;

#if defined(_DEBUG) | defined(DEBUG)
	dwFlag = D3DCOMPILE_DEBUG;
#endif // defined(_DEBUG) | defined(DEBUG)

	wstring	strPath = DX11_GET_SINGLE(CDx11FilePathManager)->FindPath(strPathKey);
	strPath += pFileName;

	char	strVersion[32] = {};
	sprintf_s(strVersion, "ps_%d_%d", iHighV, iLowV);

	ID3DBlob*	pErr = NULL;
	if (FAILED(D3DCompileFromFile(strPath.c_str(), NULL, NULL, pEntry,
		strVersion, dwFlag, 0, &m_pPSBlob, &pErr)))
	{
		MessageBoxA(NULL, (char*)pErr->GetBufferPointer(), "Error", MB_OK);
		SAFE_RELEASE(pErr);
		return false;
	}

	if (FAILED(DX11_DEVICE->CreatePixelShader(m_pPSBlob->GetBufferPointer(),
		m_pPSBlob->GetBufferSize(), NULL, &m_pPS)))
		return false;

	return true;
}

CDx11Shader::PCBUFFERINFO CDx11Shader::FindConstantBuffer(const string & strKey)
{
	unordered_map<string, PCBUFFERINFO>::iterator	iter = m_mapConstantBuffer.find(strKey);

	if (iter == m_mapConstantBuffer.end())
		return NULL;

	return iter->second;
}
