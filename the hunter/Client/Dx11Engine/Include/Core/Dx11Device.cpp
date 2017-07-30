#include "Dx11Device.h"

DX11_USING

DX11_SINGLE_DEFINITION(CDx11Device)

CDx11Device::CDx11Device()	:
	m_pDevice(NULL),
	m_pContext(NULL),
	m_pSwapChain(NULL),
	m_pRenderTargetView(NULL),
	m_pBackBuffer(NULL),
	m_pDepthStencilView(NULL),
	m_pDepthStencilBuffer(NULL),
	m_eResolutionType(RT_MINI)
{
	SetTypeName<CDx11Device>();
	for (int i = 0; i < 4; ++i)
	{
		m_fClearColor[i] = 0;
	}
}

CDx11Device::~CDx11Device()
{
	Cleanup();
}

void CDx11Device::Cleanup()
{
	SAFE_RELEASE(m_pDepthStencilBuffer);
	SAFE_RELEASE(m_pDepthStencilView);
	SAFE_RELEASE(m_pBackBuffer);
	SAFE_RELEASE(m_pRenderTargetView);
	SAFE_RELEASE(m_pSwapChain);
	SAFE_RELEASE(m_pContext);
	SAFE_RELEASE(m_pDevice);
}

bool CDx11Device::CreateDevice()
{
	UINT	iFlag = 0;

#ifdef _DEBUG1
	iFlag |= D3D11_CREATE_DEVICE_DEBUG;
#endif // _DEBUG1

	D3D_FEATURE_LEVEL	fLefvel;
	if (FAILED(D3D11CreateDevice(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL,
		iFlag, 0, 0, D3D11_SDK_VERSION, &m_pDevice, &fLefvel,
		&m_pContext)))
	{
		MessageBox(NULL, L"Create Device Failed.", L"Failed", MB_OK);
		return false;
	}

	// 11지원이 안될경우 종료한다.
	if (fLefvel != D3D_FEATURE_LEVEL_11_0)
	{
		MessageBox(NULL, L"Direct Feature Level Unsupported.",
			L"Failed", MB_OK);
		return false;
	}

	if (FAILED(m_pDevice->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM,
		4, &m_i4XMSAA)))
		return false;

	if (m_i4XMSAA <= 0)
		return false;

	return true;
}

bool CDx11Device::CreateSwapChain(HWND hWnd,
	RESOLUTION_TYPE eType,
	bool bWindowMode)
{
	m_eResolutionType = eType;
	m_bWindowMode = bWindowMode;
	m_hWnd = hWnd;

	DXGI_SWAP_CHAIN_DESC	tDesc = {};
	tDesc.BufferDesc.Width = g_tRS[eType].iWidth;
	tDesc.BufferDesc.Height = g_tRS[eType].iHeight;
	tDesc.BufferDesc.RefreshRate.Numerator = 60;
	tDesc.BufferDesc.RefreshRate.Denominator = 1;
	tDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	tDesc.SampleDesc.Count = 1;
	tDesc.SampleDesc.Quality = 0;
	tDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	tDesc.BufferCount = 1;
	tDesc.OutputWindow = hWnd;
	tDesc.Windowed = bWindowMode;
	tDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	tDesc.Flags = 0;

	IDXGIDevice*	pDXGIDevice = NULL;
	if (FAILED(m_pDevice->QueryInterface(__uuidof(IDXGIDevice), 
		(void**)&pDXGIDevice)))
		return false;

	// DXGIAdapter를 얻어온다.
	IDXGIAdapter*	pDXGIAdapter = NULL;

	if (FAILED(pDXGIDevice->GetParent(__uuidof(IDXGIAdapter),
		(void**)&pDXGIAdapter)))
		return false;

	// Factory를 얻어온다.
	IDXGIFactory*	pFactory = NULL;
	if (FAILED(pDXGIAdapter->GetParent(__uuidof(IDXGIFactory),
		(void**)&pFactory)))
		return false;

	if (FAILED(pFactory->CreateSwapChain(m_pDevice, &tDesc,
		&m_pSwapChain)))
		return false;

	SAFE_RELEASE(pDXGIDevice);
	SAFE_RELEASE(pDXGIAdapter);
	SAFE_RELEASE(pFactory);

	return true;
}

bool CDx11Device::CreateRenderTargetView()
{
	// 스왑체인으로부터 백버퍼를 얻어온다.
	m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&m_pBackBuffer);

	// 렌더링 타겟 뷰를 생성한다.
	if (FAILED(m_pDevice->CreateRenderTargetView(m_pBackBuffer, NULL, &m_pRenderTargetView)))
		return false;

	return true;
}

bool CDx11Device::CreateDepthStencilView()
{
	D3D11_TEXTURE2D_DESC	tTexture = {};

	tTexture.Width = g_tRS[m_eResolutionType].iWidth;
	tTexture.Height = g_tRS[m_eResolutionType].iHeight;
	tTexture.MipLevels = 1;
	tTexture.ArraySize = 1;
	tTexture.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	tTexture.SampleDesc.Count = 1;
	tTexture.SampleDesc.Quality = 0;
	tTexture.Usage = D3D11_USAGE_DEFAULT;
	tTexture.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	tTexture.CPUAccessFlags = 0;
	tTexture.MiscFlags = 0;

	if (FAILED(m_pDevice->CreateTexture2D(&tTexture, 0,
		&m_pDepthStencilBuffer)))
		return false;

	if (FAILED(m_pDevice->CreateDepthStencilView(m_pDepthStencilBuffer,
		0, &m_pDepthStencilView)))
		return false;

	return true;
}

void CDx11Device::CreateViewport(UINT iWidth, UINT iHeight)
{
	D3D11_VIEWPORT	tVP = {};
	tVP.Width = (float)iWidth;
	tVP.Height = (float)iHeight;
	tVP.MinDepth = 0.f;
	tVP.MaxDepth = 1.f;

	m_pContext->RSSetViewports(1, &tVP);
}

void CDx11Device::SetClearColor(int iR, int iG, int iB, int iA)
{
	m_fClearColor[0] = iR / 255.f;
	m_fClearColor[1] = iG / 255.f;
	m_fClearColor[2] = iB / 255.f;
	m_fClearColor[3] = iA / 255.f;
}

void CDx11Device::SetClearColor(float fR, float fG, float fB, float fA)
{
	m_fClearColor[0] = fR;
	m_fClearColor[1] = fG;
	m_fClearColor[2] = fB;
	m_fClearColor[3] = fA;
}

bool CDx11Device::Init(HWND hWnd, RESOLUTION_TYPE eType, 
	bool bWindowMode)
{
	// 디바이스 및 장치문맥을 생성한다.
	if (!CreateDevice())
		return false;

	// SwapChain을 생성한다.
	if (!CreateSwapChain(hWnd, eType, bWindowMode))
		return false;

	// 렌더링 타겟 뷰를 생성한다.
	if (!CreateRenderTargetView())
		return false;

	// 깊이 & 스텐실 버퍼 및 뷰를 생성한다.
	if (!CreateDepthStencilView())
		return false;

	m_pContext->OMSetRenderTargets(1, &m_pRenderTargetView,
		m_pDepthStencilView);

	CreateViewport(g_tRS[m_eResolutionType].iWidth,
		g_tRS[m_eResolutionType].iHeight);

	return true;
}

ID3D11Device * CDx11Device::GetDevice()
{
	return m_pDevice;
}

ID3D11DeviceContext * CDx11Device::GetContext()
{
	return m_pContext;
}

RESOLUTION_TYPE CDx11Device::GetResolutionType()
{
	return m_eResolutionType;
}

HWND CDx11Device::GetHWND()
{
	return m_hWnd;
}

void CDx11Device::ClearView()
{
	m_pContext->ClearRenderTargetView(m_pRenderTargetView, m_fClearColor);
	m_pContext->ClearDepthStencilView(m_pDepthStencilView,
		D3D11_CLEAR_DEPTH | D3D10_CLEAR_STENCIL, 1.f, 0);
}

void CDx11Device::Present()
{
	m_pSwapChain->Present(0, 0);
}
