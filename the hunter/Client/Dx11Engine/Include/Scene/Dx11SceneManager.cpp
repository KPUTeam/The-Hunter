#include "Dx11SceneManager.h"
#include "Dx11Scene.h"
#include "../Rendering/Dx11RenderManager.h"

DX11_USING

DX11_SINGLE_DEFINITION(CDx11SceneManager)

CDx11SceneManager::CDx11SceneManager() :
	m_pScene(NULL),
	m_pPrev(NULL),
	m_pNext(NULL)
{
	SetTypeName<CDx11SceneManager>();
}

CDx11SceneManager::~CDx11SceneManager()
{
	//Destroy();
	SAFE_RELEASE(m_pPrev);
	SAFE_RELEASE(m_pNext);
	SAFE_RELEASE(m_pScene);
}

CDx11Scene * CDx11SceneManager::GetCurrentScene()
{
	m_pScene->AddRef();
	return m_pScene;
}

bool CDx11SceneManager::Init()
{
	m_pScene = CreateScene();

	return true;
}

int CDx11SceneManager::Update(float fTime)
{
	m_pScene->Update(fTime);

	return ChangeScene();
}

int CDx11SceneManager::LateUpdate(float fTime)
{
	m_pScene->LateUpdate(fTime);

	return ChangeScene();
}

void CDx11SceneManager::AddCollision()
{
	m_pScene->AddCollision();
}

void CDx11SceneManager::Render(float fTime)
{
	m_pScene->Render(fTime);
	DX11_GET_SINGLE(CDx11RenderManager)->Render(fTime);
}

CDx11Scene * CDx11SceneManager::CreateScene(const string & strTag)
{
	CDx11Scene*	pScene = new CDx11Scene;

	pScene->SetTag(strTag);
	pScene->SetName("Scene");

	if (!pScene->Init())
	{
		SAFE_RELEASE(pScene);
		return NULL;
	}

	return pScene;
}

void CDx11SceneManager::Destroy()
{
	m_pScene->Destroy();
}

void CDx11SceneManager::ReplaceScene(CDx11Scene * pScene)
{
	m_pNext = pScene;
}

int CDx11SceneManager::ChangeScene()
{
	if (m_pNext)
	{
		if (m_pPrev)
		{
			m_pScene = m_pNext;
			m_pNext = NULL;
		}

		else
		{
			SAFE_RELEASE(m_pScene);
			m_pScene = m_pNext;
			m_pNext = NULL;
		}

		return MSCF_CHANGE;
	}

	return MSCF_NONE;
}
