#pragma once
#include "Dx11Resource.h"

DX11_BEGIN

class DX11_DLL CDx11Mesh :
	public CDx11Resource
{
private:
	friend class CDx11ResourcesManager;

private:
	CDx11Mesh();
	~CDx11Mesh();

private:
	ID3D11Buffer*		m_pVB;
	ID3D11Buffer*		m_pIB;
	UINT				m_iVtxSize;
	UINT				m_iVtxCount;
	UINT				m_iIdxSize;
	UINT				m_iIdxCount;
	DXGI_FORMAT			m_eIdxFmt;
	D3D11_PRIMITIVE_TOPOLOGY	m_eTopology;

public:
	bool CreateMesh(UINT iVtxSize,
		UINT iVtxCount, void* pVertices, UINT iIdxSize, 
		UINT iIdxCount, void* pIndices, DXGI_FORMAT eIdxFmt,
		D3D11_PRIMITIVE_TOPOLOGY eTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	bool CreateSphere(float fRadius, UINT iSliceCount, UINT iStackCount);
	void Render();

private:
	bool CreateVertexBuffer(UINT iVtxSize,
		UINT iVtxCount, void* pVertices);
	bool CreateIndexBuffer(UINT iIdxSize,
		UINT iIdxCount, void* pIndices, DXGI_FORMAT	eIdxFmt);
};

DX11_END