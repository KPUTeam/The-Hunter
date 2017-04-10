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
	// 정점 쉐이더를 로딩한다.
	if (!LoadVertexShader(pFileName, pEntry[ST_VERTEX], iHighV, iLowV, strPathKey))
		return false;

	// 픽셀 쉐이더를 로딩한다.
	if (!LoadPixelShader(pFileName, pEntry[ST_PIXEL], iHighV, iLowV, strPathKey))
		return false;

	return true;
}

void CDx11Shader::AddInputLayout(char * pSemanticName, UINT iSemanticIdx,
	DXGI_FORMAT eFmt, UINT iInputSlot, UINT iSize, 
	D3D11_INPUT_CLASSIFICATION eInputClass, UINT iInstanceData)
{
	//입력 배치 배열을 정의한다.
	//동적할당으로 정의하고, 크기가 Max와 같아지면 2배로 늘려준다.
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

		//2배를 해주고, 기존부분은 memcpy해줌.
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
	//정점 정보들을 가지고 있는 입력 배치 배열을 생성한다.
	//이 때, 입력 배치에게 정점 쉐이더 버퍼의 포인터와 그 크기를 알려준다.
	if (FAILED(DX11_DEVICE->CreateInputLayout(m_pElement, m_iElementCount,
		m_pVSBlob->GetBufferPointer(), m_pVSBlob->GetBufferSize(),
		&m_pInputLayout)))
		return false;

	return true;
}

void CDx11Shader::SetShader()
{
	//생성한 입력배치를 파이프라인에 Set해준다.
	DX11_CONTEXT->IASetInputLayout(m_pInputLayout);

	//정점 쉐이더와 픽셀 쉐이더를 Set해준다.
	DX11_CONTEXT->VSSetShader(m_pVS, NULL, 0);
	DX11_CONTEXT->PSSetShader(m_pPS, NULL, 0);
}

bool CDx11Shader::CreateConstantBuffer(const string & strKey, UINT iSize, UINT iRegister)
{
	PCBUFFERINFO	pBuffer = FindConstantBuffer(strKey);

	//버퍼가 이미 있으면 생성안함
	if (pBuffer)
		return false;

	pBuffer = new CBUFFERINFO;

	memset(pBuffer, 0, sizeof(CBUFFERINFO));

	pBuffer->iSize = iSize;

	//이미 있는 레지스터 번호인지 확인
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

	// 없는 번호일시 해당 레지스터 번호 지정해줌
	pBuffer->iRegister = iRegister;

	//버퍼 생성
	D3D11_BUFFER_DESC	tBuffer = {};

	tBuffer.ByteWidth = iSize;
	tBuffer.Usage = D3D11_USAGE_DYNAMIC;				// 다이나믹으로 설정
	tBuffer.BindFlags = D3D11_BIND_CONSTANT_BUFFER;		// 상수버퍼로 설정
	tBuffer.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;	// CPU 쓰기가능

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

	// 맵을 하면 해당 버퍼의 메모리주소를 가지고온다.
	// 맵을 통해서 버퍼가 갱신가능하게 만들어주고, 언맵하면 다시 닫아준다.

	// tMap이 pBuffer의 메모리주소를 가지고와서 데이터 쓰기를 가능하게해줌.
	D3D11_MAPPED_SUBRESOURCE	tMap = {};
	DX11_CONTEXT->Map(pBuffer->pBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &tMap);

	// tMap의 데이터에 pBuffer의 사이즈 만큼 void*형 데이터가 저장되고,
	// 이 데이터가 pBuffer에 쓰여진다.
	memcpy(tMap.pData, pData, pBuffer->iSize);

	// 맵이 끝났으면 항상 언맵을 해준다.
	DX11_CONTEXT->Unmap(pBuffer->pBuffer, 0);

	//어떤 쉐이더인지 스위치문으로 식별하고 해당 쉐이더로 Set
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
	//이미 할당된 버퍼가 있다면 릴리즈해준다.
	SAFE_RELEASE(m_pVSBlob);
	SAFE_RELEASE(m_pVS);

	//디버그 모드 플래그
	DWORD	dwFlag = 0;

#if defined(_DEBUG) | defined(DEBUG)
	dwFlag = D3DCOMPILE_DEBUG;
#endif // defined(_DEBUG) | defined(DEBUG)

	
	wstring	strPath = DX11_GET_SINGLE(CDx11FilePathManager)->FindPath(strPathKey);
	strPath += pFileName;

	//쉐이더 버전은 vs_5_0 이다.
	char	strVersion[32] = {};
	sprintf_s(strVersion, "vs_%d_%d", iHighV, iLowV);

	//D3DCompileFromFrom() 함수를 이용해 정점 쉐이더 부분의 fx파일을 로드한다.
	//로드된 데이터는 Blob에 저장된다.
	//만약 함수가 실패한다면 메세지박스에 에러메세지를 띄운다.

	ID3DBlob*	pErr = NULL;
	if (FAILED(D3DCompileFromFile(strPath.c_str(), NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, pEntry,
		strVersion, dwFlag, 0, &m_pVSBlob, &pErr)))
	{
		MessageBoxA(NULL, (char*)pErr->GetBufferPointer(), "Error", MB_OK);
		SAFE_RELEASE(pErr);
		return false;
	}

	//성공적으로 파일이 로드되었다면 해당 데이터가 저장된 VSBlob 버퍼를 이용하여 VS를 생성한다.
	if (FAILED(DX11_DEVICE->CreateVertexShader(m_pVSBlob->GetBufferPointer(),
		m_pVSBlob->GetBufferSize(), NULL, &m_pVS)))
		return false;

	return true;
}

//픽셀 쉐이더도 위와 마찬가지로 로드한다.
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
	if (FAILED(D3DCompileFromFile(strPath.c_str(), NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, pEntry,
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
