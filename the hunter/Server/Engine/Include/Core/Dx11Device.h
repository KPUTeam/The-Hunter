#pragma once
#include "../Dx11Object.h"

DX11_BEGIN

class DX11_DLL CDx11Device :
	public CDx11Object
{
private:
	ID3D11Device*			m_pDevice;
	ID3D11DeviceContext*	m_pContext;
	IDXGISwapChain*			m_pSwapChain;
	ID3D11RenderTargetView*	m_pRenderTargetView;
	ID3D11Texture2D*		m_pBackBuffer;
	ID3D11DepthStencilView*	m_pDepthStencilView;
	ID3D11Texture2D*		m_pDepthStencilBuffer;
	UINT		m_i4XMSAA;
	bool		m_bWindowMode;
	HWND		m_hWnd;
	RESOLUTION_TYPE	m_eResolutionType;
	float		m_fClearColor[4];

public:
	ID3D11Device* GetDevice();
	ID3D11DeviceContext* GetContext();
	RESOLUTION_TYPE GetResolutionType();

public:
	bool Init(HWND hWnd, RESOLUTION_TYPE eType, bool bWindowMode);
	void Cleanup();
	void CreateViewport(UINT iWidth, UINT iHeight);
	void ClearView();
	void Present();
	void SetClearColor(int iR, int iG, int iB, int iA);
	void SetClearColor(float fR, float fG, float fB, float fA);

private:
	bool CreateDevice();
	bool CreateSwapChain(HWND hWnd, RESOLUTION_TYPE eType, bool bWindowMode);
	bool CreateRenderTargetView();
	bool CreateDepthStencilView();

	DX11_SINGLE_DECLARE(CDx11Device)
};

DX11_END
