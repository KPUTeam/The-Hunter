#include "Dx11Scene.h"
#include "Dx11Layer.h"
#include "../GameObject/Dx11GameObject.h"
#include "../Component/Dx11Camera.h"
#include "../Component/Dx11LightDir.h"
#include "../Component/Dx11LightPoint.h"
#include "../Component/Dx11Transform.h"
#include "../Component/Dx11Renderer.h"
#include "../Component/Dx11Material.h"
#include "../Core/Dx11Device.h"
#include "../Rendering/Dx11RenderManager.h"

DX11_USING

CDx11Scene::CDx11Scene() :
	m_pCamera(NULL),
	m_pSky(NULL)
{
	SetTypeName<CDx11Scene>();
}

CDx11Scene::~CDx11Scene()
{
	Safe_Release_Map(m_mapCamera);
	SAFE_RELEASE(m_pUICamera);
	SAFE_RELEASE(m_pSky);
	Safe_Release_VecList(m_LightList);
	Safe_Release_VecList(m_vecScript);
	SAFE_RELEASE(m_pCamera);
	Safe_Release_VecList(m_vecLayer);
}

CDx11GameObject * CDx11Scene::GetCamera()
{
	m_pCamera->AddRef();
	return m_pCamera;
}

bool CDx11Scene::Init()
{
	CDx11Layer*	pLayer = CreateLayer();

	SAFE_RELEASE(pLayer);

	m_pCamera = CreatePerspectiveCamera("MainCamera", Vec3(0, 0, -30), DX11_PI / 3.f, 0.05f, 1000.f);

	// 로컬 회전을 적용한다.
	//CDx11Transform* pTr = m_pCamera->GetTransform();

	//pTr->SetWorldRotX(DX11_PI / 3);

	//SAFE_RELEASE(pTr);

	RESOLUTION	rs = g_tRS[DX11_GET_SINGLE(CDx11Device)->GetResolutionType()];
	m_pUICamera = CreateOrthogonalCamera("UICamera", Vec3(0.f, 0.f, 0.f),
		(float)rs.iWidth, (float)rs.iHeight, 0.f, 1000.f);

	CDx11GameObject*	pLightObj = CreateLight(LT_DIR, "Sun");

	CDx11Transform*	pLightTr = pLightObj->GetTransform();
	// pLightTr->MoveWorld(Vec3(0,100,0));
	// pLightTr->SetWorldRotX(DX11_PI / 4.f);

	SAFE_RELEASE(pLightTr);
	SAFE_RELEASE(pLightObj);


	/*CDx11GameObject*	pLightObj = CreateLight(LT_POINT, "Sun");

	CDx11Transform*	pLightTr = pLightObj->GetTransform();

	pLightTr->SetWorldPos(0.f, 8.f, 0.f);

	SAFE_RELEASE(pLightTr);
	SAFE_RELEASE(pLightObj);*/

	//CreateSky("SnowSky", L"SnowSky.dds", "SnowSky");

	return true;
}

int CDx11Scene::Update(float fTime)
{
	if (m_pSky)
		m_pSky->Update(fTime);

	vector<CDx11Script*>::iterator	iter1;
	vector<CDx11Script*>::iterator	iter1End = m_vecScript.end();

	for (iter1 = m_vecScript.begin(); iter1 != iter1End; ++iter1)
	{
		(*iter1)->Update(fTime);
	}

	vector<CDx11Layer*>::iterator	iter;
	vector<CDx11Layer*>::iterator	iterEnd = m_vecLayer.end();

	for (iter = m_vecLayer.begin(); iter != iterEnd; )
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
		}

		else
			++iter;
	}

	list<CDx11GameObject*>::iterator	iter2;
	list<CDx11GameObject*>::iterator	iter2End = m_LightList.end();

	for (iter2 = m_LightList.begin(); iter2 != iter2End; ++iter2)
	{
		(*iter2)->Update(fTime);
	}

	m_pCamera->Update(fTime);

	return 0;
}

int CDx11Scene::LateUpdate(float fTime)
{
	vector<CDx11Layer*>::iterator	iter;
	vector<CDx11Layer*>::iterator	iterEnd = m_vecLayer.end();

	for (iter = m_vecLayer.begin(); iter != iterEnd; )
	{
		if (!(*iter)->GetEnable())
		{
			++iter;
			continue;
		}

		// (*iter)->UpdateMatrix();

		if (!(*iter)->GetAlive())
		{
			SAFE_RELEASE((*iter));
			iter = m_vecLayer.erase(iter);
		}

		else
			++iter;
	}
	if (m_pSky)
		m_pSky->LateUpdate(fTime);

	vector<CDx11Script*>::iterator	iter1;
	vector<CDx11Script*>::iterator	iter1End = m_vecScript.end();

	for (iter1 = m_vecScript.begin(); iter1 != iter1End; ++iter1)
	{
		(*iter1)->LateUpdate(fTime);
	}

	for (iter = m_vecLayer.begin(); iter != iterEnd; )
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
		}

		else
			++iter;
	}

	list<CDx11GameObject*>::iterator	iter2;
	list<CDx11GameObject*>::iterator	iter2End = m_LightList.end();

	for (iter2 = m_LightList.begin(); iter2 != iter2End; ++iter2)
	{
		(*iter2)->LateUpdate(fTime);
	}

	m_pCamera->LateUpdate(fTime);

	return 0;
}

void CDx11Scene::AddCollision()
{
	vector<CDx11Layer*>::iterator	iter;
	vector<CDx11Layer*>::iterator	iterEnd = m_vecLayer.end();

	for (iter = m_vecLayer.begin(); iter != iterEnd; ++iter)
	{
		(*iter)->AddCollision();
	}
}

void CDx11Scene::Render(float fTime)
{
	if (m_pSky)
		DX11_GET_SINGLE(CDx11RenderManager)->AddSky(m_pSky);

	vector<CDx11Script*>::iterator	iter1;
	vector<CDx11Script*>::iterator	iter1End = m_vecScript.end();

	for (iter1 = m_vecScript.begin(); iter1 != iter1End; ++iter1)
	{
		(*iter1)->Render(fTime);
	}

	vector<CDx11Layer*>::iterator	iter;
	vector<CDx11Layer*>::iterator	iterEnd = m_vecLayer.end();

	for (iter = m_vecLayer.begin(); iter != iterEnd; )
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
		}

		else
			++iter;
	}

}

CDx11Layer * CDx11Scene::CreateLayer(const string & strTag)
{
	CDx11Layer*	pLayer = new CDx11Layer;

	pLayer->SetTag(strTag);
	pLayer->SetName("Layer");

	if (!pLayer->Init())
	{
		SAFE_RELEASE(pLayer);
		return NULL;
	}

	pLayer->SetScene(this);
	pLayer->AddRef();

	m_vecLayer.push_back(pLayer);

	return pLayer;

	return nullptr;
}

CDx11Layer * CDx11Scene::FindLayer(const string & strTag)
{
	vector<class CDx11Layer*>::iterator	iter;
	vector<class CDx11Layer*>::iterator	iterEnd = m_vecLayer.end();

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

CDx11GameObject * CDx11Scene::CreateLight(LIGHT_TYPE eLightType, const string & strTag)
{
	CDx11GameObject*	pLightObj = CDx11GameObject::Create(strTag);

	switch (eLightType)
	{
	case LT_DIR:
	{
		CDx11LightDir*	pLight = pLightObj->AddComponent<CDx11LightDir>(strTag);
		pLight->SetLightDir(Vec3(0.5, -1, 0.5));
		SAFE_RELEASE(pLight);
	}
	break;
	case LT_POINT:
	{
		CDx11LightPoint* pLight = pLightObj->AddComponent<CDx11LightPoint>(strTag);
		SAFE_RELEASE(pLight);
	}
	break;
	case LT_SPOT:
		break;
	}

	pLightObj->AddRef();
	m_LightList.push_back(pLightObj);

	return pLightObj;
}

const list<CDx11GameObject*>* CDx11Scene::GetLightList()
{
	return &m_LightList;
}

void CDx11Scene::Destroy()
{
	vector<class CDx11Layer*>::iterator	iter;
	vector<class CDx11Layer*>::iterator	iterEnd = m_vecLayer.end();

	for (iter = m_vecLayer.begin(); iter != iterEnd; ++iter)
	{
		(*iter)->Destroy();
		SAFE_RELEASE((*iter));
	}

	m_vecLayer.clear();
}

CDx11GameObject * CDx11Scene::CreateSky(const string & strTag,
	TCHAR * pFileName, const string & strTexKey, const string & strPathKey)
{
	SAFE_RELEASE(m_pSky);

	m_pSky = CDx11GameObject::Create(strTag);

	CDx11Transform* pTransform = m_pSky->GetTransform();

	pTransform->Scaling(5000.f, 5000.f, 5000.f);

	SAFE_RELEASE(pTransform);

	CDx11Renderer* pRenderer = m_pSky->AddComponent<CDx11Renderer>();

	pRenderer->SetShader("SkyShader");
	pRenderer->SetMesh("ColliderSphere");

	pRenderer->CreateMaterial(strTexKey, pFileName, strPathKey);

	pRenderer->SetRenderState(DX11_RS_FRONTFACECULL, RSF_RASTERIZER);
	pRenderer->SetRenderState(DX11_RS_DEPTH_LESSEQUAL, RSF_DEPTHSTENCIL);

	SAFE_RELEASE(pRenderer);

	m_pSky->SetScene(this);

	return m_pSky;
}

CDx11GameObject * CDx11Scene::CreatePerspectiveCamera(const string & strKey,
	const Vec3 & vPos, float fViewAngle, float fNear, float fViewDist)
{
	CDx11GameObject* pCamera = FindCamera(strKey);

	if (pCamera)
		return pCamera;

	pCamera = CDx11GameObject::Create(strKey, OCT_NORMAL);

	// 카메라 위치 설정
	CDx11Transform*	pTr = pCamera->GetTransform();

	pTr->SetWorldPos(vPos);

	SAFE_RELEASE(pTr);

	CDx11Camera*	pCam = pCamera->AddComponent<CDx11Camera>(strKey);

	pCam->SetCameraType(CT_PERSPECTIVE);
	pCam->SetProjection(fViewAngle, fNear, fViewDist);

	SAFE_RELEASE(pCam);

	pCamera->AddRef();

	m_mapCamera.insert(make_pair(strKey, pCamera));

	return pCamera;
}

CDx11GameObject * CDx11Scene::CreateOrthogonalCamera(const string & strKey,
	const Vec3 & vPos, float fWidth, float fHeight, float fNear, float fViewDist)
{
	CDx11GameObject* pCamera = FindCamera(strKey);

	if (pCamera)
		return pCamera;

	pCamera = CDx11GameObject::Create(strKey, OCT_NORMAL);

	// 카메라 위치 설정
	CDx11Transform*	pTr = pCamera->GetTransform();

	pTr->SetWorldPos(vPos);

	SAFE_RELEASE(pTr);

	CDx11Camera*	pCam = pCamera->AddComponent<CDx11Camera>(strKey);

	pCam->SetCameraType(CT_ORTHOGONAL);
	pCam->SetOrthoProjection(fWidth, fHeight, fNear, fViewDist);

	SAFE_RELEASE(pCam);

	pCamera->AddRef();

	m_mapCamera.insert(make_pair(strKey, pCamera));

	return pCamera;
}

CDx11GameObject * CDx11Scene::FindCamera(const string & strKey)
{
	unordered_map<string, class CDx11GameObject*>::iterator	iter = m_mapCamera.find(strKey);

	if (iter == m_mapCamera.end())
		return NULL;

	iter->second->AddRef();

	return iter->second;
}

CDx11GameObject * CDx11Scene::FindGameObject(const string & strTag)
{
	vector<class CDx11Layer*>::iterator	iter;
	vector<class CDx11Layer*>::iterator	iterEnd = m_vecLayer.end();

	for (iter = m_vecLayer.begin(); iter != iterEnd; ++iter)
	{
		CDx11GameObject*	pObj = (*iter)->FindObjectFromTag(strTag);

		if (pObj)
			return pObj;
	}

	return NULL;
}

void CDx11Scene::AddScript(CDx11Script * pScript)
{
	pScript->SetScene(this);
	pScript->AddRef();
	m_vecScript.push_back(pScript);
}

