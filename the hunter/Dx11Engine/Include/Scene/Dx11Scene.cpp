#include "Dx11Scene.h"
#include "Dx11Layer.h"
#include "../Component/Dx11Script.h"
#include "../Component/Dx11Camera.h"
#include "../Component/Dx11Transform.h"
#include "../GameObject/Dx11GameObject.h"
#include "../Component/Dx11LightDir.h"
#include "../Component/Dx11LightPoint.h"
#include "../Component/Dx11LightSpot.h"
#include "../Component/Dx11Renderer.h"
#include "../Component/Dx11Material.h"
#include "../Rendering//Dx11RenderManager.h"

DX11_USING

CDx11Scene::CDx11Scene()	:
	m_pSky(NULL)
{
	SetTypeName<CDx11Scene>();
}

CDx11Scene::~CDx11Scene()
{
	DX11_GET_SINGLE(CDx11RenderManager)->ClearRenderList();
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
	CDx11Layer* pLayer = CreateLayer("DefaultLayer");
	SAFE_RELEASE(pLayer);

	pLayer = CreateLayer("UILayer");
	SAFE_RELEASE(pLayer);

	// 기본 카메라를 생성한다.
	m_pMainCamera = CreateCamera("MainCamera", DX11_PI / 3.f, 
		0.1f, 1000.f);
	m_strMainCamera = "MainCamera";
	
	// 트랜스폼을 얻어와서 기본 카메라의 위치를 설정해준다.
	CDx11Transform*	pTr = m_pMainCamera->GetTransform();
	pTr->SetWorldPos(0.f, 0.f, -10.f);
	// 얻어왔으니 릴리즈
	SAFE_RELEASE(pTr);

	// UI 카메라 생성
	CDx11GameObject*	pCamera = CreateCamera("UICamera", 0.f, 1000.f);
	SAFE_RELEASE(pCamera);

	return true;
}

void CDx11Scene::Input(float fTime)
{
	vector<CDx11Script*>::iterator	iter1;
	vector<CDx11Script*>::iterator	iter1End = m_vecScript.end();

	for (iter1 = m_vecScript.begin(); iter1 != iter1End;)
	{
		if (!(*iter1)->GetEnable())
		{
			++iter1;
			continue;
		}

		(*iter1)->Input(fTime);

		if (!(*iter1)->GetAlive())
		{
			SAFE_RELEASE((*iter1));
			iter1 = m_vecScript.erase(iter1);
			continue;
		}

		else
			++iter1;
	}
	//씬의 Input 함수가 호출됬을 때 모든 레이어의 Input 함수를 호출한다.

	vector<CDx11Layer*>::iterator	iter;
	vector<CDx11Layer*>::iterator	iterEnd = m_vecLayer.end();
	//for-loop 돌때 함수 호출횟수를 줄이기 위해 end()값을 받아놓고 쓴다.

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
		//CDx11Transform*	pTr = m_pSky->GetTransform();
		//pTr->RotateWorld(DX11_PI / 6.f, fTime, AXIS_Y);
		//SAFE_RELEASE(pTr);
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

	DX11_GET_SINGLE(CDx11RenderManager)->Render(fTime);
}

CDx11Layer * CDx11Scene::CreateLayer(const string & strTag)
{
	CDx11Layer* pLayer = new CDx11Layer;

	if (!pLayer->Init())
	{
		SAFE_RELEASE(pLayer);
		return NULL;
	}

	pLayer->SetTag(strTag);
	pLayer->SetScene(this);


	//pLayer를 리턴할 것이므로 참조 카운트를 하나 증가시켜준다.
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
			// 마찬가지로 참조 카운트 1증가
			(*iter)->AddRef();
			return *iter;
		}
	}
	return NULL;
}

CDx11GameObject * CDx11Scene::CreateCamera(const string & strTag, float fAngle, float fNear, float fFar)
{
	CDx11GameObject*	pCameraObj = FindCamera(strTag);
	if (pCameraObj)
	{
		SAFE_RELEASE(pCameraObj);
		return NULL;
	}

	// 카메라를 찾고 없으면 생성
	pCameraObj = CDx11GameObject::Create(strTag);
	CDx11Camera*	pCamera = (CDx11Camera*)pCameraObj->AddComponent<CDx11Camera>();
	pCamera->SetProjection(fAngle, fNear, fFar);
	SAFE_RELEASE(pCamera);
	
	pCameraObj->AddRef();
	m_mapCamera.insert(make_pair(strTag, pCameraObj));

	return pCameraObj;
}

CDx11GameObject * CDx11Scene::CreateCamera(const string & strTag, float fNear, float fFar)
{
	CDx11GameObject*	pCameraObj = FindCamera(strTag);
	if (pCameraObj)
	{
		SAFE_RELEASE(pCameraObj);
		return NULL;
	}

	// 카메라를 찾고 없으면 생성
	pCameraObj = CDx11GameObject::Create(strTag);
	CDx11Camera*	pCamera = (CDx11Camera*)pCameraObj->AddComponent<CDx11Camera>();
	pCamera->SetOrthoProjection(fNear, fFar);
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

	// 카메라를 찾고 없으면 그냥 리턴
	if (!pCam)
		return;

	// 있으면 메인카메라를 해당 카메라로 변경
	SAFE_RELEASE(m_pMainCamera);

	m_strPrevCamera = m_strMainCamera;
	m_strMainCamera = strKey;
	m_pMainCamera = pCam;
}

void CDx11Scene::PrevCamera()
{
	// 이전 카메라를 찾아온다.
	CDx11GameObject*	pCam = FindCamera(m_strPrevCamera);

	// 카메라를 찾고 없으면 그냥 리턴
	if (!pCam)
		return;

	// 있으면 메인카메라를 해당 카메라로 변경
	SAFE_RELEASE(m_pMainCamera);

	string strKey = m_strPrevCamera;
	m_strPrevCamera = m_strMainCamera;
	m_strMainCamera = strKey;

	m_pMainCamera = pCam;
}

CDx11GameObject * CDx11Scene::FindGameObjectFromTag(const string & strTag)
{
	vector<CDx11Layer*>::iterator	iter;
	vector<CDx11Layer*>::iterator	iterEnd = m_vecLayer.end();

	for (iter = m_vecLayer.begin(); iter != iterEnd; ++iter)
	{
		// 각 레이어를 모두 순회해서 해당 태그의 게임오브젝트를 찾아낸다.
		// 레이어마다의 FindGameObject함수를 호출해준다.
		CDx11GameObject*	pObj = (*iter)->FindGameObjectFromTag(strTag);
		if (pObj)
			return pObj;
	}
	return NULL;
}

list<class CDx11GameObject*>* CDx11Scene::FindGameObjectsFromTag(const string & strTag)
{
	m_FindList.clear();

	vector<CDx11Layer*>::iterator	iter;
	vector<CDx11Layer*>::iterator	iterEnd = m_vecLayer.end();

	for (iter = m_vecLayer.begin(); iter != iterEnd; ++iter)
	{
		// 각 레이어마다 모두 순회해서 해당 태그를 가진 모든 게임오브젝트의 리스트의 포인터를 받아온다.
		// 마찬가지로 레이어마다의 FindGameObjects함수를 호출해준다.
		list<CDx11GameObject*>* pFindList = (*iter)->FindGameObjectsFromTag(strTag);
		
		// 받아온 pFindList를 순회하여 m_FindList에 푸쉬백
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

//	조명도 트랜스폼을 사용하기위해 게임오브젝트로 만든다.
	CDx11GameObject*	pLightObj = CDx11GameObject::Create(strTag);
	CDx11Light*			pLight = NULL;

	switch (eType)
	{
	case LT_DIR:
		pLight = pLightObj->AddComponent<CDx11LightDir>(strTag);
		break;
	case LT_POINT:
		pLight = pLightObj->AddComponent<CDx11LightPoint>(strTag);
		break;
	case LT_SPOT:
		pLight = pLightObj->AddComponent<CDx11LightSpot>(strTag);
		break;
	}

	SAFE_RELEASE(pLight);

//	디폴트 레이어에 AddObject 해주고, 리스트에 푸쉬백
	m_vecLayer[0]->AddObject(pLightObj);
	m_LightList.push_back(pLightObj);

	pLightObj->AddRef();
	return pLightObj;
}

void CDx11Scene::CreateSky(const string & strTexKey, TCHAR * pFileName, const string & strPathKey)
{
	if (!m_pSky) 
	{
		m_pSky = CDx11GameObject::Create("Sky");

		CDx11Transform*	pTr = m_pSky->GetTransform();
		pTr->Scaling(3000.f, 3000.f, 3000.f);
		SAFE_RELEASE(pTr);
	}

	CDx11Renderer* pRenderer = m_pSky->AddComponent<CDx11Renderer>("SkyRenderer");
	pRenderer->SetShader(SKY_SHADER);
	pRenderer->SetMesh("SphereMesh");
	pRenderer->CreateMaterial(strTexKey, pFileName, 0, strPathKey);
	pRenderer->SetRenderState(DX11_RS_CULL_FRONTFACE);
	pRenderer->SetRenderState(DX11_RS_DEPTH_LESSEQUAL);
	SAFE_RELEASE(pRenderer);

	m_pSky->SetScene(this);
}
