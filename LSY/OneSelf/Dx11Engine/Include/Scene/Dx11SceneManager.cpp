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

	return 0;
}

int CDx11SceneManager::LateUpdate(float fTime)
{
	m_pCurScene->LateUpdate(fTime);

	return 0;
}

void CDx11SceneManager::Collision(float fTime)
{
	m_pCurScene->Collision(fTime);
}

void CDx11SceneManager::Render(float fTime)
{
	DX11_GET_SINGLE(CDx11Device)->ClearTarget();

	m_pCurScene->Render(fTime);

	/*CDx11Mesh*	pMesh = DX11_GET_SINGLE(CDx11ResourcesManager)->FindMesh(TRIANGLE_MESH);
	CDx11Shader*	pShader = DX11_GET_SINGLE(CDx11ShaderManager)->FindShader(DEFAULT_SHADER);

	pShader->SetShader();
	pMesh->Render();

	SAFE_RELEASE(pShader);
	SAFE_RELEASE(pMesh);*/

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
