#include "Dx11Mesh.h"
#include "../Core/Dx11Device.h"

DX11_USING

CDx11Mesh::CDx11Mesh()	:
	m_pVB(NULL),
	m_pIB(NULL)
{
	SetTypeName<CDx11Mesh>();
}

CDx11Mesh::~CDx11Mesh()
{
	SAFE_RELEASE(m_pVB);
	SAFE_RELEASE(m_pIB);
}

bool CDx11Mesh::CreateMesh(UINT iVtxSize, UINT iVtxCount, 
	void * pVertices, UINT iIdxSize, UINT iIdxCount, 
	void * pIndices, DXGI_FORMAT eIdxFmt,
	D3D11_PRIMITIVE_TOPOLOGY eTopology)
{
	m_eTopology = eTopology;

	if (!CreateVertexBuffer(iVtxSize, iVtxCount, pVertices))
		return false;
	
	if (!CreateIndexBuffer(iIdxSize, iIdxCount, pIndices, eIdxFmt))
		return false;

	return true;
}

bool CDx11Mesh::CreateSphere(float fRadius, UINT iSliceCount, UINT iStackCount)
{
	vector<BUMPVERTEX>		vecVertices;
	vector<UINT>			vecIndices;

	BUMPVERTEX topVertex(0.0f, fRadius, 0.0f, 0.0f, +1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	BUMPVERTEX bottomVertex(0.0f, -fRadius, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);

	vecVertices.push_back(topVertex);

	float phiStep = DX11_PI / iStackCount;
	float thetaStep = 2.0f*DX11_PI / iSliceCount;

	// Compute vertices for each stack ring (do not count the poles as rings).
	for (UINT i = 1; i <= iStackCount - 1; ++i)
	{
		float phi = i*phiStep;

		// Vertices of ring.
		for (UINT j = 0; j <= iSliceCount; ++j)
		{
			float theta = j*thetaStep;

			BUMPVERTEX v;

			// spherical to cartesian
			v.vPos.x = fRadius*sinf(phi)*cosf(theta);
			v.vPos.y = fRadius*cosf(phi);
			v.vPos.z = fRadius*sinf(phi)*sinf(theta);

			// Partial derivative of P with respect to theta
			v.vTangent.x = -fRadius*sinf(phi)*sinf(theta);
			v.vTangent.y = 0.0f;
			v.vTangent.z = fRadius*sinf(phi)*cosf(theta);

			XMVECTOR T = XMLoadFloat3(&v.vTangent);
			XMStoreFloat3(&v.vTangent, XMVector3Normalize(T));

			XMVECTOR p = XMLoadFloat3(&v.vPos);
			XMStoreFloat3(&v.vNormal, XMVector3Normalize(p));

			v.vUV.x = theta / XM_2PI;
			v.vUV.y = phi / XM_PI;

			vecVertices.push_back(v);
		}
	}

	vecVertices.push_back(bottomVertex);

	for (UINT i = 1; i <= iSliceCount; ++i)
	{
		vecIndices.push_back(0);
		vecIndices.push_back(i + 1);
		vecIndices.push_back(i);
	}

	UINT baseIndex = 1;
	UINT ringVertexCount = iSliceCount + 1;
	for (UINT i = 0; i < iStackCount - 2; ++i)
	{
		for (UINT j = 0; j < iSliceCount; ++j)
		{
			vecIndices.push_back(baseIndex + i*ringVertexCount + j);
			vecIndices.push_back(baseIndex + i*ringVertexCount + j + 1);
			vecIndices.push_back(baseIndex + (i + 1)*ringVertexCount + j);

			vecIndices.push_back(baseIndex + (i + 1)*ringVertexCount + j);
			vecIndices.push_back(baseIndex + i*ringVertexCount + j + 1);
			vecIndices.push_back(baseIndex + (i + 1)*ringVertexCount + j + 1);
		}
	}

	UINT southPoleIndex = (UINT)vecVertices.size() - 1;

	// Offset the indices to the index of the first vertex in the last ring.
	baseIndex = southPoleIndex - ringVertexCount;

	for (UINT i = 0; i < iSliceCount; ++i)
	{
		vecIndices.push_back(southPoleIndex);
		vecIndices.push_back(baseIndex + i);
		vecIndices.push_back(baseIndex + i + 1);
	}


	m_eTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	CreateVertexBuffer(sizeof(BUMPVERTEX), vecVertices.size(),
		&vecVertices[0]);
	CreateIndexBuffer(sizeof(UINT), vecIndices.size(),
		&vecIndices[0], DXGI_FORMAT_R32_UINT);

	return true;
}

bool CDx11Mesh::CreateVertexBuffer(UINT iVtxSize, UINT iVtxCount,
	void * pVertices)
{

	m_iVtxSize = iVtxSize;
	m_iVtxCount = iVtxCount;

	D3D11_BUFFER_DESC	tDesc = {};

	tDesc.ByteWidth = iVtxSize * iVtxCount;
	tDesc.Usage = D3D11_USAGE_DEFAULT;
	tDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA	tData = {};
	tData.pSysMem = pVertices;

	if (FAILED(DX11_DEVICE->CreateBuffer(&tDesc, &tData,
		&m_pVB)))
	{
		SAFE_RELEASE(m_pVB);
		return false;
	}

	return true;
}

bool CDx11Mesh::CreateIndexBuffer(UINT iIdxSize, UINT iIdxCount,
	void * pIndices, DXGI_FORMAT eIdxFmt)
{
	m_iIdxSize = iIdxSize;
	m_iIdxCount = iIdxCount;
	m_eIdxFmt = eIdxFmt;
	D3D11_BUFFER_DESC	tDesc = {};

	tDesc.ByteWidth = iIdxSize * iIdxCount;
	tDesc.Usage = D3D11_USAGE_DEFAULT;
	tDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	D3D11_SUBRESOURCE_DATA	tData = {};
	tData.pSysMem = pIndices;

	if (FAILED(DX11_DEVICE->CreateBuffer(&tDesc, &tData,
		&m_pIB)))
	{
		SAFE_RELEASE(m_pIB);
		return false;
	}

	return true;
}

void CDx11Mesh::Render()
{
	UINT	iOffset = 0;
	DX11_CONTEXT->IASetPrimitiveTopology(m_eTopology);
	DX11_CONTEXT->IASetVertexBuffers(0, 1, &m_pVB, &m_iVtxSize, &iOffset);
	DX11_CONTEXT->IASetIndexBuffer(m_pIB, m_eIdxFmt, 0);
	DX11_CONTEXT->DrawIndexed(m_iIdxCount, 0, 0);
}
