#pragma once
#include "Dx11Resources.h"

DX11_BEGIN

typedef struct _tagVertexBuffer
{
	ID3D11Buffer*	pBuffer;
	UINT			iVtxSize;
	UINT			iVtxCount;
}VERTEXBUFFER, *PVERTEXBUFFER;

typedef struct _tagIndexBuffer
{
	ID3D11Buffer*	pBuffer;
	UINT			iIdxSize;
	UINT			iIdxCount;
	DXGI_FORMAT		eldxFmt;
}INDEXBUFFER, *PINDEXBUFFER;

typedef struct _tagBufferInfo
{
	VERTEXBUFFER			tVB;
	vector<INDEXBUFFER>		vecIB;
	vector<class CDx11Material*>	vecMaterial;

	_tagBufferInfo() :
		tVB({})
	{
	}
}BUFFERINFO, *PBUFFERINFO;

class DX11_DLL CDx11Mesh :
	public CDx11Resources
{
private:
	friend class CDx11ResourcesManager;

private:
	CDx11Mesh();
	~CDx11Mesh();

private:
	// 버퍼가 여러개 있을 수 있으므로 벡터 컨테이너를 사용한다.
	vector<BUFFERINFO>			m_vecBuffer;
	UINT						m_iContainerCount;
	UINT						m_iSubsetCount;
	D3D11_PRIMITIVE_TOPOLOGY	m_eTopology;
	class CDx11FbxLoader*		m_pLoader;

public:
	UINT GetContainerCount();
	UINT GetSubsetCount(UINT iContainer);
	class CDx11Material* CloneMaterial(int iContainer, int iSubset);
	bool IsMaterial();

public:
	bool CreateMesh(UINT iVtxSize,
		UINT iVtxCount, void* pVertices, UINT iIdxSize,
		UINT iIdxCount, void* pIndices, DXGI_FORMAT eIdxFmt,
		D3D11_PRIMITIVE_TOPOLOGY eTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	bool LoadMesh(const string& strKey, TCHAR* pFileName, const string& strPathKey = MESH_PATH);
	bool LoadMesh(const string& strKey, TCHAR* pFileName, 
		vector<vector<TCHAR*>>& vecTexName, const string& strPathKey = MESH_PATH);


	bool CreateSphere(float fRadius, UINT iSliceCount, UINT iStackCount);
	void Render();
	void Render(int iContainer, int iSubset);

private:
	bool CreateVertexBuffer(UINT iVtxSize,
		UINT iVtxCount, void* pVertices,  PVERTEXBUFFER pVB);
	bool CreateIndexBuffer(UINT iIdxSize,
		UINT iIdxCount, void* pIndices, DXGI_FORMAT	eIdxFmt, PINDEXBUFFER pIB);
	bool LoadVertexData();
	bool LoadMaterialData(const string& strKey);
	bool LoadMaterialData(const string& strKey, TCHAR* pTexName,
		const string& strPathKey);
};

DX11_END