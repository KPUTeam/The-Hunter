#include "Dx11Scene.h"
#include "Dx11Layer.h"
#include "../Component/Dx11Script.h"
#include "../GameObject/Dx11GameObject.h"
#include "../Component/Dx11Camera.h"
#include "../Component/Dx11Transform.h"
#include "../Component/Dx11LightDir.h"
#include "../Component/Dx11LightPoint.h"
#include "../Component/Dx11Renderer.h"
#include "../Component/Dx11Material.h"

DX11_USING

CDx11Scene::CDx11Scene()	:
	m_pSky(NULL)
{
	SetTypeName<CDx11Scene>();
}

CDx11Scene::~CDx11Scene()
{
	SAFE_RELEASE(m_pSky);
	Safe_Release_VecList(m_LightList);
	SAFE_RELEASE(m_pMainCamera);
	Safe_Release_Map(m_mapCamera);
	Safe_Release_VecList(m_vecScript);
	Safe_Release_VecList(m_vecLayer);
}

CDx11GameObject * CDx11Scene::GetMainCamera()
{
	m_pMainCamera->AddRef();
	return m_pMainCamera;
}

list<class CDx11GameObject*>* CDx11Scene::GetLightList()
{
	return &m_LightList;
}

bool CDx11Scene::Init()
{
	CDx11Layer*	pLayer = CreateLayer("DefaultLayer");

	SAFE_RELEASE(pLayer);

	// 기본 카메라를 생성한다.
	m_pMainCamera = CreateCamera("MainCamera", DX11_PI / 3.f,0.1f, 1000.f);
	CDx11Transform*	pTr = m_pMainCamera->GetTransform();
	pTr->SetWorldPos(0.f, 15.f, -15.f);
	pTr->SetLocalRotX(30.f);
	SAFE_RELEASE(pTr);

	// LightDir
	/*CDx11GameObject*	pLightDir = CreateLight(LT_DIR, "LightDir");
	pTr = pLightDir->GetTransform();
	pTr->SetWorldRotX(DX11_PI / 4.f);
	SAFE_RELEASE(pTr);
	SAFE_RELEASE(pLightDir);*/


	// LightPoint
	CDx11GameObject*	pLightPoint = CreateLight(LT_POINT, "LightPoint");
	pTr = pLightPoint->GetTransform();
	pTr->SetWorldPos(0.f, 15.f, -15.f);
	SAFE_RELEASE(pTr);
	SAFE_RELEASE(pLightPoint);

	return true;
}

void CDx11Scene::Input(float fTime)
{
	vector<CDx11Layer*>::iterator	iter;
	vector<CDx11Layer*>::iterator	iterEnd = m_vecLayer.end();

	for (iter = m_vecLayer.begin(); iter != iterEnd; ++iter)
	{
		(*iter)->Input(fTime);
	}
}

int CDx11Scene::Update(float fTime)
{
	m_pMainCamera->Update(fTime);

	if (m_pSky)
	{
		m_pSky->Update(fTime);
	}

	vector<CDx11Script*>::iterator	iter1;
	vector<CDx11Script*>::iterator	iter1End = m_vecScript.end();

	for (iter1 = m_vecScript.begin(); iter1 != iter1End;)
	{
		if (!(*iter1)->GetEnable())
		{
			++iter1;
			continue;
		}

		(*iter1)->Update(fTime);

		if (!(*iter1)->GetAlive())
		{
			SAFE_RELEASE((*iter1));
			iter1 = m_vecScript.erase(iter1);
			continue;
		}

		else
			++iter1;
	}

	vector<CDx11Layer*>::iterator	iter;
	vector<CDx11Layer*>::iterator	iterEnd = m_vecLayer.end();

	for (iter = m_vecLayer.begin(); iter != iterEnd;)
	{
		if (!(*iter)->GetEnable())
		{
			++iter;
			continue;
		}

		(*iter)->Update(fTime);

		if (!(*iter)->GetAlive())
		{
			SAFE_RELEASE((*iter));
			iter = m_vecLayer.erase(iter);
			continue;
		}

		else
			++iter;
	}

	return 0;
}

int CDx11Scene::LateUpdate(float fTime)
{
	m_pMainCamera->LateUpdate(fTime);

	if (m_pSky)
		m_pSky->LateUpdate(fTime);

	vector<CDx11Script*>::iterator	iter1;
	vector<CDx11Script*>::iterator	iter1End = m_vecScript.end();

	for (iter1 = m_vecScript.begin(); iter1 != iter1End;)
	{
		if (!(*iter1)->GetEnable())
		{
			++iter1;
			continue;
		}

		(*iter1)->LateUpdate(fTime);

		if (!(*iter1)->GetAlive())
		{
			SAFE_RELEASE((*iter1));
			iter1 = m_vecScript.erase(iter1);
			continue;
		}

		else
			++iter1;
	}

	vector<CDx11Layer*>::iterator	iter;
	vector<CDx11Layer*>::iterator	iterEnd = m_vecLayer.end();

	for (iter = m_vecLayer.begin(); iter != iterEnd;)
	{
		if (!(*iter)->GetEnable())
		{
			++iter;
			continue;
		}

		(*iter)->LateUpdate(fTime);

		if (!(*iter)->GetAlive())
		{
			SAFE_RELEASE((*iter));
			iter = m_vecLayer.erase(iter);
			continue;
		}

		else
			++iter;
	}

	return 0;
}

void CDx11Scene::Collision(float fTime)
{
	vector<CDx11Layer*>::iterator	iter;
	vector<CDx11Layer*>::iterator	iterEnd = m_vecLayer.end();

	for (iter = m_vecLayer.begin(); iter != iterEnd; ++iter)
	{
		(*iter)->Collision(fTime);
	}
}

void CDx11Scene::Render(float fTime)
{
	m_pMainCamera->Render(fTime);

	if(m_pSky)
		m_pSky->Render(fTime);

	vector<CDx11Script*>::iterator	iter1;
	vector<CDx11Script*>::iterator	iter1End = m_vecScript.end();

	for (iter1 = m_vecScript.begin(); iter1 != iter1End;)
	{
		if (!(*iter1)->GetEnable())
		{
			++iter1;
			continue;
		}

		(*iter1)->Render(fTime);

		if (!(*iter1)->GetAlive())
		{
			SAFE_RELEASE((*iter1));
			iter1 = m_vecScript.erase(iter1);
			continue;
		}

		else
			++iter1;
	}

	vector<CDx11Layer*>::iterator	iter;
	vector<CDx11Layer*>::iterator	iterEnd = m_vecLayer.end();

	for (iter = m_vecLayer.begin(); iter != iterEnd;)
	{
		if (!(*iter)->GetEnable())
		{
			++iter;
			continue;
		}

		(*iter)->Render(fTime);

		if (!(*iter)->GetAlive())
		{
			SAFE_RELEASE((*iter));
			iter = m_vecLayer.erase(iter);
			continue;
		}

		else
			++iter;
	}
}

CDx11Layer * CDx11Scene::CreateLayer(const string & strTag)
{
	CDx11Layer*	pLayer = new CDx11Layer;

	if (!pLayer->Init())
	{
		SAFE_RELEASE(pLayer);
		return NULL;
	}

	pLayer->SetTag(strTag);
	pLayer->SetScene(this);

	pLayer->AddRef();
	m_vecLayer.push_back(pLayer);

	return pLayer;
}

CDx11Layer * CDx11Scene::FindLayer(const string & strTag)
{
	vector<CDx11Layer*>::iterator	iter;
	vector<CDx11Layer*>::iterator	iterEnd = m_vecLayer.end();

	for (iter = m_vecLayer.begin(); iter != iterEnd; ++iter)
	{
		if ((*iter)->GetTag() == strTag)
		{
			(*iter)->AddRef();
			return *iter;
		}
	}

	return NULL;
}

CDx11GameObject * CDx11Scene::CreateCamera(const string & strTag, 
	float fAngle, float fNear, float fFar)
{
	CDx11GameObject*	pCameraObj = FindCamera(strTag);

	if (pCameraObj)
	{
		SAFE_RELEASE(pCameraObj);
		return NULL;
	}

	pCameraObj	= CDx11GameObject::Create(strTag);

	CDx11Camera*	pCamera = (CDx11Camera*)pCameraObj->AddComponent<CDx11Camera>();

	pCamera->SetProjection(fAngle, fNear, fFar);

	SAFE_RELEASE(pCamera);

	pCameraObj->AddRef();
	m_mapCamera.insert(make_pair(strTag, pCameraObj));

	return pCameraObj;
}

CDx11GameObject * CDx11Scene::FindCamera(const string & strKey)
{
	unordered_map<string, class CDx11GameObject*>::iterator	iter = m_mapCamera.find(strKey);

	if (iter == m_mapCamera.end())
		return NULL;

	iter->second->AddRef();

	return iter->second;
}

void CDx11Scene::ChangeCamera(const string & strKey)
{
	CDx11GameObject*	pCam = FindCamera(strKey);

	if (!pCam)
		return;

	SAFE_RELEASE(m_pMainCamera);

	m_pMainCamera = pCam;
}

CDx11GameObject * CDx11Scene::FindObjectFromTag(const string & strTag)
{
	vector<CDx11Layer*>::iterator	iter;
	vector<CDx11Layer*>::iterator	iterEnd = m_vecLayer.end();

	for (iter = m_vecLayer.begin(); iter != iterEnd; ++iter)
	{
		CDx11GameObject*	pObj = (*iter)->FindObjectFromTag(strTag);

		if (pObj)
			return pObj;
	}

	return NULL;
}

list<CDx11GameObject*>* CDx11Scene::FindObjectsFromTag(const string & strTag)
{
	m_FindList.clear();

	vector<CDx11Layer*>::iterator	iter;
	vector<CDx11Layer*>::iterator	iterEnd = m_vecLayer.end();

	for (iter = m_vecLayer.begin(); iter != iterEnd; ++iter)
	{
		list<CDx11GameObject*>*	pFindList = (*iter)->FindObjectsFromTag(strTag);
		list<CDx11GameObject*>::iterator	iter1;
		list<CDx11GameObject*>::iterator	iter1End = pFindList->end();

		for (iter1 = pFindList->begin(); iter1 != iter1End; ++iter1)
		{
			m_FindList.push_back(*iter1);
		}
	}

	return &m_FindList;
}

CDx11GameObject * CDx11Scene::CreateLight(LIGHT_TYPE eType, const string & strTag)
{
	CDx11GameObject*	pLightObj = CDx11GameObject::Create(strTag);

	CDx11Light*	pLight = NULL;

	switch (eType)
	{
	case LT_DIR:
		pLight = pLightObj->AddComponent<CDx11LightDir>(strTag);
		break;
	case LT_POINT:
		pLight = pLightObj->AddComponent<CDx11LightPoint>(strTag);
		break;
	case LT_SPOT:
		break;
	}

	SAFE_RELEASE(pLight);

	m_vecLayer[0]->AddObject(pLightObj);

	m_LightList.push_back(pLightObj);

	pLightObj->AddRef();

	return pLightObj;
}

void CDx11Scene::CreateSky(const string & strTexKey, TCHAR * pFileName, 
	const string & strPathKey)
{
	//SAFE_RELEASE(m_pSky);
	if (!m_pSky)
	{
		m_pSky = CDx11GameObject::Create("Sky");

		CDx11Transform*	pTransform = m_pSky->GetTransform();

		pTransform->Scaling(3000.f, 3000.f, 3000.f);

		SAFE_RELEASE(pTransform);
	}

	CDx11Renderer*	pRenderer = m_pSky->AddComponent<CDx11Renderer>("SkyRenderer");

	pRenderer->SetShader(SKY_SHADER);
	pRenderer->SetMesh(SPHERE_MESH);

	pRenderer->CreateMaterial(strTexKey, pFileName, strPathKey);
	pRenderer->SetRenderState(DX11_RS_CULL_FRONTFACE);
	pRenderer->SetRenderState(DX11_RS_DEPTH_LESSEQUAL);

	SAFE_RELEASE(pRenderer);

	m_pSky->SetScene(this);
}
