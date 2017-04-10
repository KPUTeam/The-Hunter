#pragma once
#include "Dx11Component.h"

DX11_BEGIN

class DX11_DLL CDx11Terrain :
	public CDx11Component
{
public:
	CDx11Terrain();
	CDx11Terrain(const CDx11Terrain& terrain);
	~CDx11Terrain();

private:
	UINT		m_iNumX;
	UINT		m_iNumY;
	vector<Vec3>	m_vecPos;

public:
	virtual bool Init();
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Render(float fTime);
	virtual CDx11Terrain* Clone();

public:
	void CreataTerrain(const string& strKey, UINT iNumX, UINT iNumY,
		const string& strDiffuseTexKey, TCHAR* pDiffuseTexName, char* pHeightName = NULL,
		const string& strHeightMapPathKey = TEXTURE_PATH);
	float GetHeight(const Vec3& vPos);
};

DX11_END