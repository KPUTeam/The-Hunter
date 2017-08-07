#pragma once
#include "Dx11Resources.h"

DX11_BEGIN

typedef struct _tagVertexBuffer
{
	ID3D11Buffer*	pVB;
	UINT			iVertexCount;
	UINT			iVertexStride;
}VERTEXBUFFER, *PVERTEXBUFFER;

typedef struct _tagIndexBuffer
{
	ID3D11Buffer*		pIB;
	UINT				iIndexCount;
	UINT				iIndexStride;
	UINT				iPrimitiveCount;
	DXGI_FORMAT			eIndexFmt;
}INDEXBUFFER, *PINDEXBUFFER;

typedef struct _tagMeshContainer
{
	VERTEXBUFFER	tVB;
	vector<INDEXBUFFER>		vecIB;
	vector<class CDx11Material*>	vecMaterial;
}MESHCONTAINER, *PMESHCONTAINER;

class DX11_DLL CDx11Mesh :
	public CDx11Resources
{
protected:
	friend class CDx11ResourcesManager;

protected:
	CDx11Mesh();
	~CDx11Mesh();

private:
	vector<PMESHCONTAINER>		m_vecMeshContainer;
	D3D11_PRIMITIVE_TOPOLOGY	m_ePrimitive;
	class CDx11Animation3D*		m_pAnimation;

public:
	size_t GetContainerCount()	const;
	size_t GetSubsetCount(int iContainer = 0)	const;
	bool IsValidMaterial()	const;
	class CDx11Material* CloneMaterial(int iContainer = 0, int iSubset = 0)	const;

public:
	bool CreateBuffer(void* pVertices, UINT iVertexCount, UINT iVertexStride,
		void* pIndices, UINT iIndexCount, UINT iIndexStride, DXGI_FORMAT eIndexFmt,
		D3D11_PRIMITIVE_TOPOLOGY ePrimitive = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	bool LoadMesh(const string& strKey, TCHAR* pFileName, const string& strPathKey = DX11_MESH_PATH);
	bool LoadMesh(const string& strKey, const wchar_t* pFileName);
	bool CreateVertexBuffer(void* pVertices, UINT iVertexCount, UINT iVertexStride);
	bool CreateIndexBuffer(void* pIndices, UINT iIndexCount, UINT iIndexStride,
		DXGI_FORMAT eIndexFmt);

	void Render();
	void Render(int iContainer, int iSubset);
	class CDx11Animation3D* CloneAnimationController()	const;
};

DX11_END
