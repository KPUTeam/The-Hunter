#pragma once
#include "../Dx11Object.h"

DX11_BEGIN

class DX11_DLL CDx11SceneManager :
	public CDx11Object
{
	DX11_SINGLE_DECLARE(CDx11SceneManager)

private:
	class CDx11Scene*	m_pScene;
	class CDx11Scene*	m_pPrev;
	class CDx11Scene*	m_pNext;

public:
	CDx11Scene* GetCurrentScene();

public:
	bool Init();
	int Update(float fTime);
	int LateUpdate(float fTime);
	void AddCollision();
	void Render(float fTime);
	CDx11Scene* CreateScene(const string& strTag = "DefaultScene");
	void Destroy();
	void ReplaceScene(CDx11Scene* pScene);

private:
	int ChangeScene();
};

DX11_END
