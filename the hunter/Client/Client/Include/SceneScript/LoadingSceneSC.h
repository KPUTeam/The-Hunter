#pragma once

#include "Component/Dx11Script.h"

DX11_USING

class CLoadingSceneSc :
	public CDx11Script
{
public:
	CLoadingSceneSc();
	CLoadingSceneSc(const CLoadingSceneSc& script);
	virtual ~CLoadingSceneSc();

private:
	class CDx11GameObject*	pLoadingBar;
	class CDx11Transform* pLoadngBarTr;

	float fBarScale = 0.f;

	vector<string> strKey;
	vector<wchar_t*> cFileName;

	vector<string>::iterator iterKey;
	vector<wchar_t*>::iterator iterFileName;

public:
	void CreateFrameBar();
	void CreateLoadingBar();

public:
	virtual bool Init();
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Render(float fTime);
	virtual CLoadingSceneSc* Clone();

};

