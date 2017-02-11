#pragma once
#include "../Dx11Object.h"

DX11_BEGIN

class DX11_DLL CDx11SceneManager :
	public CDx11Object
{
	DX11_DECLARE_SINGLE(CDx11SceneManager)

private:
	class CDx11Scene*		m_pCurScene;
	class CDx11Scene*		m_pNextScene;
	class CDx11Scene*		m_pPrevScene;

public:
	CDx11Scene* GetCurrentScene();

public:
	bool Init();
	void Input(float fTime);
	int Update(float fTime);
	int LateUpdate(float fTime);
	void Collision(float fTime);
	void Render(float fTime);
	class CDx11Scene* CreateScene(const string& strTag = "Scene");
};

DX11_END
