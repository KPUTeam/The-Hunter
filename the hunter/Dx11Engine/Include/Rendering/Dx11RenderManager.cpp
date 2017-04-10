#include "Dx11RenderManager.h"
#include "Dx11DepthStencilState.h"
#include "Dx11RasterizerState.h"
#include "Dx11BlendState.h"
#include "../Component/Dx11Renderer.h"
#include "../Scene/Dx11Scene.h"
#include "../Scene/Dx11SceneManager.h"
#include "../Component/Dx11Transform.h"
#include "../GameObject/Dx11GameObject.h"
#include "../Component/Dx11UI.h"

DX11_USING

DX11_DEFINITION_SINGLE(CDx11RenderManager)

CDx11RenderManager::CDx11RenderManager()	:
	m_iObjCount(0)
{
	for (int i = 0; i < RTF_END; ++i)
	{
		m_RenderList[i].reserve(100);
	}
}

CDx11RenderManager::~CDx11RenderManager()
{
	Safe_Release_Map(m_mapState);
}


void CDx11RenderManager::AddRenderObject(CDx11GameObject * pObj)
{
	CDx11Renderer*	pRenderer = (CDx11Renderer*)pObj->FindComponentFromType(CT_RENDERER);
		
	if (pRenderer)
	{
		// UI일 경우와 아닐 경우로 나눔
		CDx11Component* pUI = pObj->FindComponentFromType(CT_UI);
		if (pUI)
		{
			SAFE_RELEASE(pUI);
			m_RenderList[RTF_UI].push_back(pObj);
		}
		else
		{
			m_RenderList[pRenderer->GetRenderType()].push_back(pObj);
		}
	}
	else
		m_RenderList[RTF_NORMAL].push_back(pObj);

	SAFE_RELEASE(pRenderer);

}

void CDx11RenderManager::ClearRenderList()
{
	for (int i = 0; i < RTF_END; ++i)
	{
		m_RenderList[i].clear();
	}
}

bool CDx11RenderManager::Init()
{
	// 깊이 비교를 LESS_EQUAL로 처리하는 상태 생성
	CDx11RenderState* pState = CreateDepthStencilState(DX11_RS_DEPTH_LESSEQUAL,
		TRUE, D3D11_DEPTH_WRITE_MASK_ALL,
		D3D11_COMPARISON_LESS_EQUAL);
	SAFE_RELEASE(pState);

	// 와이어 프레임 상태 생성
	pState = CreateRasterizerState(DX11_RS_WIREFRAME, D3D11_FILL_WIREFRAME, D3D11_CULL_BACK);
	SAFE_RELEASE(pState);

	
	// 앞면 컬링
	pState = CreateRasterizerState(DX11_RS_CULL_FRONTFACE, D3D11_FILL_SOLID, D3D11_CULL_FRONT);
	SAFE_RELEASE(pState);

	// 논 컬링
	pState = CreateRasterizerState(DX11_RS_CULL_NONE, D3D11_FILL_SOLID, D3D11_CULL_NONE);
	SAFE_RELEASE(pState);


	// 알파 블렌딩
	pState = CreateBlendState(DX11_RS_ALPHABLEND);
	SAFE_RELEASE(pState);

	// 깊이 제거
	pState = CreateDepthStencilState(DX11_RS_DEPTH_DISABLE, TRUE,
		D3D11_DEPTH_WRITE_MASK_ZERO, D3D11_COMPARISON_ALWAYS);
	SAFE_RELEASE(pState);

	return true;
}

void CDx11RenderManager::Render(float fTime)
{
	for (int i = 0; i < RTF_END; ++i)
	{
		// 노말 일 때는 가까움 -> 먼 순으로 정렬
		if (i == RTF_NORMAL)
			sort(m_RenderList[i].begin(), m_RenderList[i].end(), CDx11RenderManager::SortAscending);
		// 알파인 경우는 먼 -> 가까움 으로 정렬
		else if (i != RTF_UI)
			sort(m_RenderList[i].begin(), m_RenderList[i].end(), CDx11RenderManager::SortDescending);
		// UI인 경우 z값으로 정렬
		else
		{
			// UI카메라로 변경후 소트해줌
			CDx11Scene*	pScene = DX11_GET_SINGLE(CDx11SceneManager)->GetCurrentScene();
			pScene->ChangeCamera("UICamera");
			SAFE_RELEASE(pScene);

			sort(m_RenderList[i].begin(), m_RenderList[i].end(), CDx11RenderManager::SortUI);
		}

		// 정렬을 끝내고 렌더링을 해준다.
		vector<CDx11GameObject*>::iterator	iter;
		vector<CDx11GameObject*>::iterator	iterEnd = m_RenderList[i].end();

		for (iter = m_RenderList[i].begin(); iter != iterEnd; ++iter)
		{
  			(*iter)->Render(fTime);
		}

		// UI카메라로 변경했다면,
		// 렌더링이 끝내고 다시 이전 카메라로 리셋한다.
		if (i == RTF_UI)
		{
			CDx11Scene* pScene = DX11_GET_SINGLE(CDx11SceneManager)->GetCurrentScene();
			pScene->PrevCamera();
			SAFE_RELEASE(pScene);
		}
	}


	// 렌더링 후 리스트를 비워준다.
	ClearRenderList();
}

CDx11DepthStencilState * CDx11RenderManager::CreateDepthStencilState(const string & strKey, BOOL bDepthEnable, 
	D3D11_DEPTH_WRITE_MASK eDepthMask, D3D11_COMPARISON_FUNC eDepthFunc)
{
	CDx11DepthStencilState*	pState = (CDx11DepthStencilState*)FindRenderState(strKey);

	if (pState)
		return pState;

	pState = new CDx11DepthStencilState;

	if (!pState->CreateDepthStencilState(bDepthEnable, eDepthMask,
		eDepthFunc))
	{
		SAFE_RELEASE(pState);
		return NULL;
	}

	pState->AddRef();
	m_mapState.insert(make_pair(strKey, pState));
	return pState;
}

CDx11RasterizerState * CDx11RenderManager::CreateRasterizerState(const string & strKey, 
	D3D11_FILL_MODE eFillMode, D3D11_CULL_MODE eCullMode, 
	BOOL bFrontCounterClockwise, BOOL bDepthClipEnable)
{
	CDx11RasterizerState*	pState = (CDx11RasterizerState*)FindRenderState(strKey);

	if (pState)
		return pState;

	pState = new CDx11RasterizerState;

	if (!pState->CreateRasterizerState(eFillMode, eCullMode,
		bFrontCounterClockwise, bDepthClipEnable))
	{
		SAFE_RELEASE(pState);
		return NULL;
	}

	pState->AddRef();
	m_mapState.insert(make_pair(strKey, pState));
	return pState;
}

CDx11BlendState * CDx11RenderManager::CreateBlendState(const string & strKey, 
	D3D11_BLEND eSrcBlend, D3D11_BLEND eDestBlend, 
	D3D11_BLEND_OP eBlendOp, D3D11_BLEND eSrcAlphaBlend, 
	D3D11_BLEND eDestAlphaBlend, D3D11_BLEND_OP eAlphaBlendOp, 
	UINT8 iWriteMask, BOOL bAlphaEnable, BOOL bIndependentEnable)
{
	CDx11BlendState*	pState = (CDx11BlendState*)FindRenderState(strKey);

	if (pState)
		return pState;

	pState = new CDx11BlendState;

	if (!pState->CreateBlendState(eSrcBlend, eDestBlend, eBlendOp,
		eSrcAlphaBlend, eDestAlphaBlend, eAlphaBlendOp, iWriteMask,
		bAlphaEnable, bIndependentEnable))
	{
		SAFE_RELEASE(pState);
		return NULL;
	}

	pState->AddRef();
	m_mapState.insert(make_pair(strKey, pState));
	return pState;
}

CDx11BlendState * CDx11RenderManager::CreateBlendState(const string & strKey, 
	const D3D11_RENDER_TARGET_BLEND_DESC * pTargetBlend, 
	UINT iCount, BOOL bAlphaEnable, BOOL bIndependentEnable)
{
	CDx11BlendState*	pState = (CDx11BlendState*)FindRenderState(strKey);

	if (pState)
		return pState;

	pState = new CDx11BlendState;

	if (!pState->CreateBlendState(pTargetBlend, iCount,	
		bAlphaEnable, bIndependentEnable))
	{
		SAFE_RELEASE(pState);
		return NULL;
	}

	pState->AddRef();
	m_mapState.insert(make_pair(strKey, pState));
	return pState;
}

CDx11RenderState * CDx11RenderManager::FindRenderState(const string & strKey)
{
	unordered_map<string, class CDx11RenderState*>::iterator	iter = m_mapState.find(strKey);

	if (iter == m_mapState.end())
		return NULL;

	iter->second->AddRef();
	
	return iter->second;
}

bool CDx11RenderManager::SortAscending(CDx11GameObject * pLeftObj, CDx11GameObject * pRightObj)
{
	CDx11Scene* pScene = DX11_GET_SINGLE(CDx11SceneManager)->GetCurrentScene();
	CDx11GameObject* pCameraObj = pScene->GetMainCamera();
	
	CDx11Transform* pCamTr = pCameraObj->GetTransform();
	CDx11Transform* pLeftTr = pLeftObj->GetTransform();
	CDx11Transform* pRightTr = pRightObj->GetTransform();

	Vec3 vCamPos = pCamTr->GetWorldPos();
	Vec3 vLeftPos = pLeftTr->GetWorldPos();
	Vec3 vRightPos = pRightTr->GetWorldPos();

	float fLeftDist = vLeftPos.Distance(vCamPos);
	float fRightDist = vRightPos.Distance(vCamPos);

	SAFE_RELEASE(pLeftTr);
	SAFE_RELEASE(pRightTr);
	SAFE_RELEASE(pCamTr);

	SAFE_RELEASE(pCameraObj);
	SAFE_RELEASE(pScene);

	if (fLeftDist < fRightDist)
		return true;

	return false;
}

bool CDx11RenderManager::SortDescending(CDx11GameObject * pLeftObj, CDx11GameObject * pRightObj)
{
	CDx11Scene* pScene = DX11_GET_SINGLE(CDx11SceneManager)->GetCurrentScene();
	CDx11GameObject* pCameraObj = pScene->GetMainCamera();

	CDx11Transform* pCamTr = pCameraObj->GetTransform();
	CDx11Transform* pLeftTr = pLeftObj->GetTransform();
	CDx11Transform* pRightTr = pRightObj->GetTransform();

	Vec3 vCamPos = pCamTr->GetWorldPos();
	Vec3 vLeftPos = pLeftTr->GetWorldPos();
	Vec3 vRightPos = pRightTr->GetWorldPos();

	float fLeftDist = vLeftPos.Distance(vCamPos);
	float fRightDist = vRightPos.Distance(vCamPos);

	SAFE_RELEASE(pLeftTr);
	SAFE_RELEASE(pRightTr);
	SAFE_RELEASE(pCamTr);

	SAFE_RELEASE(pCameraObj);
	SAFE_RELEASE(pScene);

	if (fLeftDist > fRightDist)
		return true;

	return false;
}

bool CDx11RenderManager::SortUI(CDx11GameObject * pLeftObj, CDx11GameObject * pRightObj)
{
	CDx11UI*	pUI1 = (CDx11UI*)pLeftObj->FindComponentFromType(CT_UI);
	CDx11UI*	pUI2 = (CDx11UI*)pRightObj->FindComponentFromType(CT_UI);

	int	iZOrder1 = pUI1->GetZOrder();
	int	iZOrder2 = pUI2->GetZOrder();

	SAFE_RELEASE(pUI1);
	SAFE_RELEASE(pUI2);

	if (iZOrder1 < iZOrder2)
		return true;

	return false;
}
