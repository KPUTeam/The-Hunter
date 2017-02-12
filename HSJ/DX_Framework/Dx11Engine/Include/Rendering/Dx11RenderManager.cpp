#include "Dx11RenderManager.h"
#include "Dx11DepthStencilState.h"
#include "Dx11RasterizerState.h"
#include "Dx11BlendState.h"
#include "../GameObject/Dx11GameObject.h"
#include "../Component/Dx11Renderer.h"
#include "../Scene/Dx11Scene.h"
#include "../Scene/Dx11SceneManager.h"
#include "../Component/Dx11Transform.h"

DX11_USING

DX11_DEFINITION_SINGLE(CDx11RenderManager)

CDx11RenderManager::CDx11RenderManager()
{
	for (int i = 0; i < RT_END; ++i)
	{
		m_RenderList[i].reserve(100);
	}
}

CDx11RenderManager::~CDx11RenderManager()
{
	Safe_Release_Map(m_mapState);
}

void CDx11RenderManager::AddObjCount()
{
	++m_iObjCount;
}

void CDx11RenderManager::ReleaseObjCount()
{
	--m_iObjCount;
}

void CDx11RenderManager::AddRenderObject(CDx11GameObject * pObj)
{
	CDx11Renderer*	pRenderer = (CDx11Renderer*)pObj->FindComponentFromType(CT_RENDERER);

	if (pRenderer)
	{
		CDx11Component*	pUI = pObj->FindComponentFromType(CT_UI);
		if (pUI)
		{
			SAFE_RELEASE(pUI);
			m_RenderList[RT_UI].push_back(pObj);
		}

		else
			m_RenderList[pRenderer->GetRenderType()].push_back(pObj);
	}

	else
		m_RenderList[RT_NORMAL].push_back(pObj);

	SAFE_RELEASE(pRenderer);
}

void CDx11RenderManager::ClearRenderList()
{
	for (int i = 0; i < RT_END; ++i)
	{
		m_RenderList[i].clear();
	}
}

bool CDx11RenderManager::Init()
{
	// 깊이 비교를 <=로 처리하는 상태를 하나 만들어준다.
	CDx11RenderState* pState = CreateDepthStencilState(DX11_RS_DEPTH_LESSEQUAL,
		TRUE, D3D11_DEPTH_WRITE_MASK_ALL,
		D3D11_COMPARISON_LESS_EQUAL);

	SAFE_RELEASE(pState);

	// Wireframe
	pState = CreateRasterizerState(DX11_RS_WIREFRAME, D3D11_FILL_WIREFRAME,
		D3D11_CULL_BACK);

	SAFE_RELEASE(pState);

	// Front Face Culling
	pState = CreateRasterizerState(DX11_RS_CULL_FRONTFACE,
		D3D11_FILL_SOLID, D3D11_CULL_FRONT);

	SAFE_RELEASE(pState);

	// Alpha Blending
	pState = CreateBlendState(DX11_RS_ALPHABLEND);

	SAFE_RELEASE(pState);

	return true;
}

void CDx11RenderManager::Render(float fTime)
{
	for (int i = 0; i < RT_END; ++i)
	{
		// 정렬한다.
		if (i == RT_NORMAL)
			sort(m_RenderList[i].begin(), m_RenderList[i].end(), CDx11RenderManager::SortDist);

		else if (i != RT_UI)
			sort(m_RenderList[i].begin(), m_RenderList[i].end(), CDx11RenderManager::SortDistInv);

		else
			sort(m_RenderList[i].begin(), m_RenderList[i].end(), CDx11RenderManager::SortUI);

		vector<CDx11GameObject*>::iterator	iter;
		vector<CDx11GameObject*>::iterator	iterEnd = m_RenderList[i].end();

		for (iter = m_RenderList[i].begin(); iter != iterEnd; ++iter)
		{
			(*iter)->Render(fTime);
		}
	}

	ClearRenderList();
}

CDx11DepthStencilState * CDx11RenderManager::CreateDepthStencilState(
	const string & strKey, BOOL bDepthEnable,
	D3D11_DEPTH_WRITE_MASK eDepthMask,
	D3D11_COMPARISON_FUNC eDepthFunc)
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
	BOOL bFrontCounterClockWise, BOOL bDepthClipEnable)
{
	CDx11RasterizerState*	pState = (CDx11RasterizerState*)FindRenderState(strKey);

	if (pState)
		return pState;

	pState = new CDx11RasterizerState;

	if (!pState->CreateRasterizerState(eFillMode, eCullMode,
		bFrontCounterClockWise, bDepthClipEnable))
	{
		SAFE_RELEASE(pState);
		return NULL;
	}

	pState->AddRef();

	m_mapState.insert(make_pair(strKey, pState));

	return pState;
}

CDx11BlendState * CDx11RenderManager::CreateBlendState(const string & strKey,
	D3D11_BLEND eSrcBlend, D3D11_BLEND eDestBlend, D3D11_BLEND_OP eBlendOp,
	D3D11_BLEND eSrcAlphaBlend, D3D11_BLEND eDestAlphaBlend,
	D3D11_BLEND_OP eAlphaBlendOp, UINT8 iWriteMask, BOOL bAlphaEnable,
	BOOL bIndependentEnable)
{
	CDx11BlendState*	pState = (CDx11BlendState*)FindRenderState(strKey);

	if (pState)
		return pState;

	pState = new CDx11BlendState;

	if (!pState->CreateBlendState(eSrcBlend, eDestBlend, eBlendOp,
		eSrcAlphaBlend, eDestAlphaBlend, eAlphaBlendOp, iWriteMask, bAlphaEnable,
		bIndependentEnable))
	{
		SAFE_RELEASE(pState);
		return NULL;
	}

	pState->AddRef();

	m_mapState.insert(make_pair(strKey, pState));

	return pState;
}

CDx11BlendState * CDx11RenderManager::CreateBlendState(const string & strKey,
	const D3D11_RENDER_TARGET_BLEND_DESC * pTargetBlend, UINT iCount,
	BOOL bAlphaEnable, BOOL bIndependentEnable)
{
	CDx11BlendState*	pState = (CDx11BlendState*)FindRenderState(strKey);

	if (pState)
		return pState;

	pState = new CDx11BlendState;

	if (!pState->CreateBlendState(pTargetBlend, iCount, bAlphaEnable,
		bIndependentEnable))
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

bool CDx11RenderManager::SortDist(CDx11GameObject * pObj1, CDx11GameObject * pObj2)
{
	CDx11Scene*	pScene = DX11_GET_SINGLE(CDx11SceneManager)->GetCurrentScene();

	CDx11GameObject*	pCameraObj = pScene->GetMainCamera();

	CDx11Transform*	pCamTr = pCameraObj->GetTransform();

	CDx11Transform*	pTr1 = pObj1->GetTransform();
	CDx11Transform*	pTr2 = pObj2->GetTransform();

	Vec3	vCamPos = pCamTr->GetWorldPos();
	Vec3	vPos1 = pTr1->GetWorldPos();
	Vec3	vPos2 = pTr2->GetWorldPos();

	float	fDist1 = vPos1.Distance(vCamPos);
	float	fDist2 = vPos2.Distance(vCamPos);

	SAFE_RELEASE(pTr1);
	SAFE_RELEASE(pTr2);

	SAFE_RELEASE(pCamTr);

	SAFE_RELEASE(pCameraObj);

	SAFE_RELEASE(pScene);

	if (fDist1 > fDist2)
		return true;

	return false;
}

bool CDx11RenderManager::SortDistInv(CDx11GameObject * pObj1, CDx11GameObject * pObj2)
{
	CDx11Scene*	pScene = DX11_GET_SINGLE(CDx11SceneManager)->GetCurrentScene();

	CDx11GameObject*	pCameraObj = pScene->GetMainCamera();

	CDx11Transform*	pCamTr = pCameraObj->GetTransform();

	CDx11Transform*	pTr1 = pObj1->GetTransform();
	CDx11Transform*	pTr2 = pObj2->GetTransform();

	Vec3	vCamPos = pCamTr->GetWorldPos();
	Vec3	vPos1 = pTr1->GetWorldPos();
	Vec3	vPos2 = pTr2->GetWorldPos();

	float	fDist1 = vPos1.Distance(vCamPos);
	float	fDist2 = vPos2.Distance(vCamPos);

	SAFE_RELEASE(pTr1);
	SAFE_RELEASE(pTr2);

	SAFE_RELEASE(pCamTr);

	SAFE_RELEASE(pCameraObj);

	SAFE_RELEASE(pScene);

	if (fDist1 < fDist2)
		return true;

	return false;
}

bool CDx11RenderManager::SortUI(CDx11GameObject * pObj1, CDx11GameObject * pObj2)
{
	CDx11Transform*	pTr1 = pObj1->GetTransform();
	CDx11Transform*	pTr2 = pObj2->GetTransform();

	Vec3	vPos1 = pTr1->GetWorldPos();
	Vec3	vPos2 = pTr2->GetWorldPos();

	SAFE_RELEASE(pTr1);
	SAFE_RELEASE(pTr2);

	if (vPos1.z > vPos2.z)
		return true;

	return false;
}
