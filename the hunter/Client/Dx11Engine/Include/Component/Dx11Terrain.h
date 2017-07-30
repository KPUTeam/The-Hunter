#pragma once
#include "Dx11Component.h"

DX11_BEGIN

class DX11_DLL CDx11Terrain :
	public CDx11Component
{
public:
	CDx11Terrain();
	CDx11Terrain(const CDx11Terrain& terrain);
	virtual ~CDx11Terrain();

private:
	UINT m_iWidth;
	UINT m_iHeight;
	vector<Vec3>	m_vecPos;

public:
	virtual bool Init();
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Render(float fTime);
	virtual CDx11Terrain* Clone();

public:
	void CreateTerrian(
		UINT iW, UINT iH, float fWeight, 
		char * pHeightMap, 
		TCHAR* pDiffuseTex,
		TCHAR* pSpecTex,
		TCHAR* pNormalTex,
		const string & strPathKey = DX11_TEXTURE_PATH);

	TRIANGLE2RAY GetHeight(const CDx11GameObject* obj);

	Vec3 GetTex(const Vec3 vPos, int iIndexs);


};

DX11_END
