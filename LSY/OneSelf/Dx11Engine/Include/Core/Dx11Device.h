#pragma once
#include "../Dx11Object.h"

DX11_BEGIN

class DX11_DLL CDx11Device :
	public CDx11Object
{
	DX11_DECLARE_SINGLE(CDx11Device)

private:
	ID3D11Device*			m_pDevice;
	ID3D11DeviceContext*	m_pContext;
	IDXGISwapChain*			m_pSwapChain;
	ID3D11RenderTargetView*	m_pRenderTargetView;
	ID3D11Texture2D*		m_pDepthTex;
	ID3D11DepthStencilView*	m_pDepthStencilView;
	float					m_fClearColor[4];
	RESOLUTION_TYPE			m_eRT;
	bool					m_bWindowMode;
	UINT					m_iMSAA;
	bool					m_bMSAA;
	HWND					m_hWnd;

public:
	ID3D11Device* GetDevice();
	ID3D11DeviceContext* GetContext();
	RESOLUTION_TYPE GetResolutionType();

public:
	bool Init(RESOLUTION_TYPE eRT, bool bWindowMode, HWND hWnd);
	void ClearTarget();
	void Present();

private:
	bool CreateSwapChain();
	bool CreateRenderTargetView();
	bool CreateDepthStencilView();	
	bool CreateViewport();
};

DX11_END