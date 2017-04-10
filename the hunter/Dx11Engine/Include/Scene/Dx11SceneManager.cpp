#include "Dx11SceneManager.h"
#include "Dx11Scene.h"
#include "../Core/Dx11Device.h"

DX11_USING

DX11_DEFINITION_SINGLE(CDx11SceneManager)

CDx11SceneManager::CDx11SceneManager()	:
	m_pCurScene(NULL),
	m_pNextScene(NULL),
	m_pPrevScene(NULL)
{
	SetTypeName<CDx11SceneManager>();
}

CDx11SceneManager::~CDx11SceneManager()
{
	SAFE_RELEASE(m_pPrevScene);
	SAFE_RELEASE(m_pNextScene);
	SAFE_RELEASE(m_pCurScene);
}

CDx11Scene * CDx11SceneManager::GetCurrentScene()
{
	m_pCurScene->AddRef();

	return m_pCurScene;
}

bool CDx11SceneManager::Init()
{
	m_pCurScene = CreateScene();

	return true;
}

void CDx11SceneManager::Input(float fTime)
{
	m_pCurScene->Input(fTime);
}

int CDx11SceneManager::Update(float fTime)
{
	m_pCurScene->Update(fTime);

	return Change();
}

int CDx11SceneManager::LateUpdate(float fTime)
{
	m_pCurScene->LateUpdate(fTime);

	return Change();
}

void CDx11SceneManager::Collision(float fTime)
{
	m_pCurScene->Collision(fTime);
}

void CDx11SceneManager::Render(float fTime)
{
	DX11_GET_SINGLE(CDx11Device)->ClearTarget();

	m_pCurScene->Render(fTime);

	DX11_GET_SINGLE(CDx11Device)->Present();
}

CDx11Scene * CDx11SceneManager::CreateScene(const string & strTag)
{
	CDx11Scene*	pScene = new CDx11Scene;

	pScene->SetTag(strTag);

	if (!pScene->Init())
	{
		SAFE_RELEASE(pScene);
		return NULL;
	}

	return pScene;
}

void CDx11SceneManager::ChangeScene(CDx11Scene * pNext)
{
	m_pNextScene = pNext;
}

SCENE_CHANGE CDx11SceneManager::Change()
{
	if (m_pNextScene)
	{
		SAFE_RELEASE(m_pCurScene);
		m_pCurScene = m_pNextScene;
		m_pNextScene = NULL;
		return SC_CHANGE;
	}

	return SC_NONE;
}
