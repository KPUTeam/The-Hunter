#include "Dx11Device.h"

DX11_USING

DX11_DEFINITION_SINGLE(CDx11Device)

CDx11Device::CDx11Device()	:
	m_pDevice(NULL),
	m_pContext(NULL),
	m_pSwapChain(NULL),
	m_pRenderTargetView(NULL),
	m_pDepthTex(NULL),
	m_pDepthStencilView(NULL),
	m_iMSAA(0),
	m_bMSAA(false)
{
	SetTypeName<CDx11Device>();
}

CDx11Device::~CDx11Device()
{
	if (m_pContext)
		m_pContext->ClearState();

	SAFE_RELEASE(m_pDepthTex);
	SAFE_RELEASE(m_pDepthStencilView);
	SAFE_RELEASE(m_pRenderTargetView);
	SAFE_RELEASE(m_pSwapChain);
	SAFE_RELEASE(m_pContext);
	SAFE_RELEASE(m_pDevice);
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
	return m_eRT;
}

bool CDx11Device::Init(RESOLUTION_TYPE eRT, bool bWindowMode, HWND hWnd)
{
	m_fClearColor[0] = 0.f;
	m_fClearColor[1] = 0.f;
	m_fClearColor[2] = 0.f;
	m_fClearColor[3] = 1.f;

	m_eRT = eRT;
	m_hWnd = hWnd;
	m_bWindowMode = bWindowMode;

	UINT	iFlag = 0;

#if defined(_DEBUG) | defined(DEBUG)
	iFlag = D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_FEATURE_LEVEL	eLevel;

	if (FAILED(D3D11CreateDevice(0, D3D_DRIVER_TYPE_HARDWARE, NULL,
		iFlag, 0, 0, D3D11_SDK_VERSION, &m_pDevice, &eLevel, &m_pContext)))
		return false;
	
	// 4X MSAA 체크
	if (FAILED(m_pDevice->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM,
		4, &m_iMSAA)))
		return false;

	m_bMSAA = m_iMSAA > 0;

	//  스왑체인을 생성한다.
	if (!CreateSwapChain())
		return false;

	// RenderTargetView를 생성한다.
	if (!CreateRenderTargetView())
		return false;

	// Depth, Stencil View를 생성한다.
	if (!CreateDepthStencilView())
		return false;
	
	// RenderTargetView와 DepthStencilView를 출력병합기 단계에
	// 묶어준다.
	m_pContext->OMSetRenderTargets(1, &m_pRenderTargetView,
		m_pDepthStencilView);

	CreateViewport();

	return true;
}

bool CDx11Device::CreateSwapChain()
{
	DXGI_SWAP_CHAIN_DESC	tDesc = {};
	tDesc.BufferDesc.Width = g_tRS[m_eRT].iWidth;
	tDesc.BufferDesc.Height = g_tRS[m_eRT].iHeight;
	tDesc.BufferDesc.RefreshRate.Numerator = 60;
	tDesc.BufferDesc.RefreshRate.Denominator = 1;
	tDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	tDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	tDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	if (m_bMSAA)
	{
		tDesc.SampleDesc.Count = 4;
		tDesc.SampleDesc.Quality = m_iMSAA - 1;
	}

	else
	{
		tDesc.SampleDesc.Count = 1;
		tDesc.SampleDesc.Quality = 0;
	}

	tDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	tDesc.BufferCount = 1;
	tDesc.OutputWindow = m_hWnd;
	tDesc.Windowed = m_bWindowMode;
	tDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	IDXGIDevice*	pDXGIDevice = NULL;
	
	if (FAILED(m_pDevice->QueryInterface(__uuidof(IDXGIDevice),
		(void**)&pDXGIDevice)))
		return false;

	IDXGIAdapter*	pAdapter = NULL;
	if (FAILED(pDXGIDevice->GetParent(__uuidof(IDXGIAdapter),
		(void**)&pAdapter)))
	{
		SAFE_RELEASE(pDXGIDevice);
		return false;
	}

	IDXGIFactory*	pFactory = NULL;
	if (FAILED(pAdapter->GetParent(__uuidof(IDXGIFactory),
		(void**)&pFactory)))
	{
		SAFE_RELEASE(pDXGIDevice);
		SAFE_RELEASE(pAdapter);
		return false;
	}

	// 스왑체인을 생성한다.
	if (FAILED(pFactory->CreateSwapChain(m_pDevice, &tDesc,
		&m_pSwapChain)))
	{
		SAFE_RELEASE(pFactory);
		SAFE_RELEASE(pAdapter);
		SAFE_RELEASE(pDXGIDevice);
		return false;
	}

	SAFE_RELEASE(pFactory);
	SAFE_RELEASE(pAdapter);
	SAFE_RELEASE(pDXGIDevice);

	return true;
}

bool CDx11Device::CreateRenderTargetView()
{
	ID3D11Texture2D*	pBackBuffer = NULL;

	m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D),
		(void**)&pBackBuffer);
	m_pDevice->CreateRenderTargetView(pBackBuffer, NULL,
		&m_pRenderTargetView);
	SAFE_RELEASE(pBackBuffer);

	return true;
}

bool CDx11Device::CreateDepthStencilView()
{
	D3D11_TEXTURE2D_DESC	tDesc = {};
	tDesc.Width = g_tRS[m_eRT].iWidth;
	tDesc.Height = g_tRS[m_eRT].iHeight;
	tDesc.MipLevels = 1;
	tDesc.ArraySize = 1;
	tDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;


	if (m_bMSAA)
	{
		tDesc.SampleDesc.Count = 4;
		tDesc.SampleDesc.Quality = m_iMSAA - 1;
	}

	else
	{
		tDesc.SampleDesc.Count = 1;
		tDesc.SampleDesc.Quality = 0;
	}


	tDesc.Usage = D3D11_USAGE_DEFAULT;
	tDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	tDesc.CPUAccessFlags = 0;
	tDesc.MiscFlags = 0;

	if (FAILED(m_pDevice->CreateTexture2D(&tDesc, NULL, 
		&m_pDepthTex)))
		return false;

	if (FAILED(m_pDevice->CreateDepthStencilView(m_pDepthTex,
		0, &m_pDepthStencilView)))
		return false;

	return true;
}

bool CDx11Device::CreateViewport()
{
	D3D11_VIEWPORT	vp = {};
	vp.Width = g_tRS[m_eRT].iWidth;
	vp.Height = g_tRS[m_eRT].iHeight;
	vp.MaxDepth = 1.f;

	m_pContext->RSSetViewports(1, &vp);

	return true;
}

void CDx11Device::ClearTarget()
{
	m_pContext->ClearRenderTargetView(m_pRenderTargetView,
		m_fClearColor);
	m_pContext->ClearDepthStencilView(m_pDepthStencilView,
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.f, 0);
}

void CDx11Device::Present()
{
	m_pSwapChain->Present(0, 0);
}
