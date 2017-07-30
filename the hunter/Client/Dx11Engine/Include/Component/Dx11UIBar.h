#pragma once
#include "Dx11UI.h"

DX11_BEGIN

class DX11_DLL CDx11UIBar :
	public CDx11UI
{
private:
	friend class CDx11GameObject;

private:
	CDx11UIBar();
	CDx11UIBar(const CDx11UIBar& ui);
	~CDx11UIBar();

private:
	float		m_fMin;
	float		m_fMax;
	float		m_fValue;
	float		m_fEmpty;
	Vec3		m_vMaxScale;
	Vec3		m_vOriginPos;
	UIBAR_DIR	m_eBarDir;

public:
	void SetBarDir(UIBAR_DIR eDir);
	void SetMin(float fMin);
	void SetMax(float fMax);
	void SetValue(float fValue);
	void SetInfo(float fMin, float fMax, float fValue);
	void AddValue(float fValue);
	bool SetBarTexture(TCHAR* pFileName, const string& strKey,
		const string& strPathKey = DX11_TEXTURE_PATH);
	bool SetBarTexture(const string& strKey);
	bool SetMesh(const string& strKey, void* pVertices, UINT iVertexCount, UINT iVertexStride,
		void* pIndices, UINT iIndexCount, UINT iIndexStride, DXGI_FORMAT eIndexFmt,
		D3D11_PRIMITIVE_TOPOLOGY ePrimitive = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

private:
	void ComputeScale();

public:
	virtual bool Init();
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Render(float fTime);
	virtual CDx11UIBar* Clone();
};

DX11_END
