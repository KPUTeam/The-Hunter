#pragma once
#include "../Dx11Object.h"

DX11_BEGIN

class DX11_DLL CDx11ResourcesManager :
	public CDx11Object
{
	DX11_SINGLE_DECLARE(CDx11ResourcesManager)

private:
	unordered_map<string, class CDx11Mesh*>	m_mapMesh;
	unordered_map<string, class CDx11Texture*>	m_mapTexture;

public:
	bool Init();
	CDx11Mesh* CreateMesh(const string& strKey, void* pVertices, UINT iVertexCount, UINT iVertexStride,
		void* pIndices, UINT iIndexCount, UINT iIndexStride, DXGI_FORMAT eIndexFmt,
		D3D11_PRIMITIVE_TOPOLOGY ePrimitive = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	CDx11Mesh* LoadMesh(const string& strKey, TCHAR* pFileName,
		const string& strPathKey = DX11_MESH_PATH);
	CDx11Mesh* LoadMesh(const string& strKey, const wchar_t* pFullPath);
	bool CreateBox(MESHDATA& tMeshData, UINT iWidth, UINT iHeight, UINT iDepth);
	bool CreateSphere(MESHDATA& tMeshData, float fRadius, UINT iSlice,
		UINT iStack);
	CDx11Mesh* FindMesh(const string& strKey);
	CDx11Texture* LoadTexture(const string& strKey, TCHAR* pFileName,
		const string& strPathKey = DX11_TEXTURE_PATH);
	CDx11Texture* LoadTexture(const string& strKey, const string& strPath);
	CDx11Texture* CreateTexture(const string& strKey, UINT iWidth, UINT iHeight, UINT iArraySize,
		DXGI_FORMAT eFmt, D3D11_USAGE eUsage,
		D3D11_BIND_FLAG eBindFlag,
		int iCpuFlag);
	CDx11Texture* CreateTexture(const string& strKey, UINT iLength, UINT iArraySize, DXGI_FORMAT eFmt,
		D3D11_USAGE eUsage,
		D3D11_BIND_FLAG eBindFlag,
		int iCpuFlag);
	CDx11Texture* FindTexture(const string& strKey);
};

DX11_END
