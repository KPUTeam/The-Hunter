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

	// �⺻ ī�޶� �����Ѵ�.
	m_pMainCamera = CreateCamera("MainCamera", DX11_PI / 3.f, 
		0.1f, 1000.f);
	m_strMainCamera = "MainCamera";
	
	// Ʈ�������� ���ͼ� �⺻ ī�޶��� ��ġ�� �������ش�.
	CDx11Transform*	pTr = m_pMainCamera->GetTransform();
	pTr->SetWorldPos(0.f, 0.f, -10.f);
	// �������� ������
	SAFE_RELEASE(pTr);

	// UI ī�޶� ����
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
	//���� Input �Լ��� ȣ����� �� ��� ���̾��� Input �Լ��� ȣ���Ѵ�.

	vector<CDx11Layer*>::iterator	iter;
	vector<CDx11Layer*>::iterator	iterEnd = m_vecLayer.end();
	//for-loop ���� �Լ� ȣ��Ƚ���� ���̱� ���� end()���� �޾Ƴ��� ����.

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


	//pLayer�� ������ ���̹Ƿ� ���� ī��Ʈ�� �ϳ� ���������ش�.
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
			// ���������� ���� ī��Ʈ 1����
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

	// ī�޶� ã�� ������ ����
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

	// ī�޶� ã�� ������ ����
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

	// ī�޶� ã�� ������ �׳� ����
	if (!pCam)
		return;

	// ������ ����ī�޶� �ش� ī�޶�� ����
	SAFE_RELEASE(m_pMainCamera);

	m_strPrevCamera = m_strMainCamera;
	m_strMainCamera = strKey;
	m_pMainCamera = pCam;
}

void CDx11Scene::PrevCamera()
{
	// ���� ī�޶� ã�ƿ´�.
	CDx11GameObject*	pCam = FindCamera(m_strPrevCamera);

	// ī�޶� ã�� ������ �׳� ����
	if (!pCam)
		return;

	// ������ ����ī�޶� �ش� ī�޶�� ����
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
		// �� ���̾ ��� ��ȸ�ؼ� �ش� �±��� ���ӿ�����Ʈ�� ã�Ƴ���.
		// ���̾���� FindGameObject�Լ��� ȣ�����ش�.
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
		// �� ���̾�� ��� ��ȸ�ؼ� �ش� �±׸� ���� ��� ���ӿ�����Ʈ�� ����Ʈ�� �����͸� �޾ƿ´�.
		// ���������� ���̾���� FindGameObjects�Լ��� ȣ�����ش�.
		list<CDx11GameObject*>* pFindList = (*iter)->FindGameObjectsFromTag(strTag);
		
		// �޾ƿ� pFindList�� ��ȸ�Ͽ� m_FindList�� Ǫ����
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

//	���� Ʈ�������� ����ϱ����� ���ӿ�����Ʈ�� �����.
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

//	����Ʈ ���̾ AddObject ���ְ�, ����Ʈ�� Ǫ����
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
