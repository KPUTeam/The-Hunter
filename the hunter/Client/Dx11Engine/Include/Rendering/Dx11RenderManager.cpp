#include "Dx11RenderManager.h"
#include "../Core/Dx11Device.h"
#include "Dx11RasterizerState.h"
#include "Dx11DepthStencilState.h"
#include "Dx11BlendState.h"
#include "../Component/Dx11Renderer.h"
#include "../GameObject/Dx11GameObject.h"
#include "../Scene/Dx11SceneManager.h"
#include "../Scene/Dx11Scene.h"
#include "../Component/Dx11Transform.h"
#include "../Component/Dx11UI.h"
#include "../Component/Dx11Camera.h"
#include "../Component/Dx11ColliderSphere.h"

DX11_USING

DX11_SINGLE_DEFINITION(CDx11RenderManager)

CDx11RenderManager::CDx11RenderManager()
{
}

CDx11RenderManager::~CDx11RenderManager()
{
	Safe_Release_Map(m_mapRS);
}

bool CDx11RenderManager::Init()
{
	// WireFrame
	CDx11RasterizerState* pState = CreateRasterizerState(DX11_RS_WIREFRAME,
		D3D11_FILL_WIREFRAME);

	SAFE_RELEASE(pState);

	// Front Face Culling
	pState = CreateRasterizerState(DX11_RS_FRONTFACECULL,
		D3D11_FILL_SOLID, D3D11_CULL_BACK, TRUE);

	SAFE_RELEASE(pState);

	// None Culling
	pState = CreateRasterizerState(DX11_RS_NONECULL,
		D3D11_FILL_SOLID, D3D11_CULL_NONE, TRUE);

	SAFE_RELEASE(pState);


	// Depth Function LessEqual
	CDx11DepthStencilState* pDepthState = CreateDepthStencilState(DX11_RS_DEPTH_LESSEQUAL,
		true, D3D11_DEPTH_WRITE_MASK_ALL, D3D11_COMPARISON_LESS_EQUAL);

	SAFE_RELEASE(pDepthState);

	// Z Disable
	pDepthState = CreateDepthStencilState(DX11_RS_DEPTH_DISABLE,
		false);

	SAFE_RELEASE(pDepthState);

	// Alpha Blend
	CDx11BlendState*	pAlphaState = CreateBlendState(DX11_RS_ALPHABLEND);

	SAFE_RELEASE(pAlphaState);

	for (int i = 0; i < RG_END; ++i)
	{
		m_vecRender[i].reserve(500);
	}

	return true;
}

void CDx11RenderManager::AddObject(CDx11GameObject * pObj)
{
	CDx11Renderer*	pRenderer = (CDx11Renderer*)pObj->FindComponentFromTypeName("CDx11Renderer");

	if (!pRenderer)
		return;

	// 현제 카메라의 프러스텀을 가져온다.
	CDx11Scene*	pScene = DX11_GET_SINGLE(CDx11SceneManager)->GetCurrentScene();
	CDx11GameObject* pCamObj = pScene->GetCamera();
	CDx11Camera*	pCamera = (CDx11Camera*)pCamObj->FindComponentFromType(CT_CAMERA);

	Frustrum* fs = pCamera->GetFrustrum();

	SAFE_RELEASE(pCamera);
	SAFE_RELEASE(pCamObj);
	SAFE_RELEASE(pScene);

	// 콜리더 스피어를 모두 찾아온다.
	const list<CDx11Component*>*	pCollList = pObj->FindComponentsFromTypeName("CDx11ColliderSphere");

	float fRadius = 0.f;
	Vec3 vPos = {};

	// 콜리더 스피어를 가지고 있으면
	if (pCollList->size() != 0)
	{
		// 반지름이 가장 큰 콜리더 스피어를 찾고
		list<CDx11Component*>::const_iterator	iter;
		list<CDx11Component*>::const_iterator	iterEnd = pCollList->end();

		for (iter = pCollList->begin(); iter != iterEnd; ++iter)
		{
			CDx11ColliderSphere* iterCollider = (CDx11ColliderSphere*)(*iter);

			if (fRadius < iterCollider->GetRadius())
			{
				fRadius = iterCollider->GetRadius();

				CDx11GameObject* pObj = (*iter)->GetGameObject();
				CDx11Transform* pTr = pObj->GetTransform();
				vPos = pTr->GetWorldPos();

				SAFE_RELEASE(pObj);
				SAFE_RELEASE(pTr);
			}
		}

		// 절두체 컬링을 검사한다.
		// 실패하면 나간다.
		if (fs->IsInSphereXZ(vPos, fRadius) == false)
		{
			SAFE_RELEASE(pRenderer);
			return;
		}
	}

	if (pRenderer->IsBlendRender())
	{
		if (pRenderer->IsRender2D())
			m_vecRender[RG_UI].push_back(pObj);

		else
		{
			m_vecRender[RG_ALPHA3].push_back(pObj);
		}
	}

	else
	{
		m_vecRender[RG_NORMAL].push_back(pObj);
	}

	SAFE_RELEASE(pRenderer);
}

void CDx11RenderManager::AddSky(CDx11GameObject * pObj)
{
	m_vecRender[RG_SKY].push_back(pObj);
}

void CDx11RenderManager::Render(float fTime)
{
	for (int i = 0; i < RG_END; ++i)
	{
		if (m_vecRender[i].empty())
			continue;

		if (i == RG_SKY || i == RG_NORMAL)
		{
			sort(m_vecRender[i].begin(), m_vecRender[i].end(),
				CDx11RenderManager::SortNormal);
		}

		else if (i == RG_UI)
		{
			sort(m_vecRender[i].begin(), m_vecRender[i].end(),
				CDx11RenderManager::SortUI);
		}

		else
		{
			sort(m_vecRender[i].begin(), m_vecRender[i].end(),
				CDx11RenderManager::SortAlpha);
		}

		for (size_t j = 0; j < m_vecRender[i].size(); ++j)
		{
			m_vecRender[i][j]->Render(fTime);
		}

		m_vecRender[i].clear();
	}
}

CDx11RasterizerState* CDx11RenderManager::CreateRasterizerState(const string & strKey,
	D3D11_FILL_MODE eFillMode, D3D11_CULL_MODE eCullMode, BOOL bClockWise,
	int iDepthBias, float fDepthBiasClamp, float fSlopeScaleDepthBias,
	bool bDepthClipEnable, bool bScissorEnable, bool bMultiSampleEnable,
	bool bAntialiasedEnable)
{
	CDx11RasterizerState*	pState = (CDx11RasterizerState*)FindRS(strKey);

	if (pState)
		return pState;

	pState = new CDx11RasterizerState;

	if (!pState->CreateRasterizerState(eFillMode, eCullMode, bClockWise,
		iDepthBias, fDepthBiasClamp, fSlopeScaleDepthBias, bDepthClipEnable,
		bScissorEnable, bMultiSampleEnable, bAntialiasedEnable))
	{
		SAFE_RELEASE(pState);
		return NULL;
	}

	pState->AddRef();
	m_mapRS.insert(make_pair(strKey, pState));

	return pState;
}

void CDx11RenderManager::SetFillMode(D3D11_FILL_MODE eFillMode)
{
	m_tRasterizer.FillMode = eFillMode;
}

CDx11DepthStencilState * CDx11RenderManager::CreateDepthStencilState(const string & strKey,
	bool bDepthEnable, D3D11_DEPTH_WRITE_MASK eWriteMask,
	D3D11_COMPARISON_FUNC eDepthFunc, bool bStencilEnable,
	UINT8 iReadMask, UINT8 iWriteMask)
{

	CDx11DepthStencilState*	pState = (CDx11DepthStencilState*)FindRS(strKey);

	if (pState)
		return pState;

	pState = new CDx11DepthStencilState;

	if (!pState->CreateDepthStencilState(bDepthEnable, eWriteMask, eDepthFunc,
		bStencilEnable, iReadMask, iWriteMask))
	{
		SAFE_RELEASE(pState);
		return NULL;
	}

	pState->AddRef();
	m_mapRS.insert(make_pair(strKey, pState));

	return pState;
}

CDx11RenderState * CDx11RenderManager::FindRS(const string & strKey)
{
	unordered_map<string, CDx11RenderState*>::iterator	iter = m_mapRS.find(strKey);

	if (iter == m_mapRS.end())
		return NULL;

	iter->second->AddRef();

	return iter->second;
}

CDx11BlendState * CDx11RenderManager::CreateBlendState(
	const string & strKey, BOOL bEnable, D3D11_BLEND eSrcBlend,
	D3D11_BLEND eDestBlend, D3D11_BLEND_OP eBlendOp,
	D3D11_BLEND eAlphaSrcBlend, D3D11_BLEND eAlphaDestBlend,
	D3D11_BLEND_OP eAlphaBlendOp, BOOL bAlphaToCoverage,
	BOOL bIndependentBlend, UINT8 iWriteMask)
{
	CDx11BlendState*	pState = (CDx11BlendState*)FindRS(strKey);

	if (pState)
		return pState;

	pState = new CDx11BlendState;

	if (!pState->CreateBlendState(bEnable, eSrcBlend, eDestBlend, eBlendOp,
		eAlphaSrcBlend, eAlphaDestBlend, eAlphaBlendOp, bAlphaToCoverage,
		bIndependentBlend, iWriteMask))
	{
		SAFE_RELEASE(pState);
		return false;
	}

	pState->CreateState();

	pState->AddRef();

	m_mapRS.insert(make_pair(strKey, pState));

	return pState;
}

bool CDx11RenderManager::AddBlendState(const string & strKey,
	BOOL bEnable, D3D11_BLEND eSrcBlend, D3D11_BLEND eDestBlend,
	D3D11_BLEND_OP eBlendOp, D3D11_BLEND eAlphaSrcBlend,
	D3D11_BLEND eAlphaDestBlend, D3D11_BLEND_OP eAlphaBlendOp,
	BOOL bAlphaToCoverage, BOOL bIndependentBlend, UINT8 iWriteMask)
{
	CDx11BlendState*	pState = (CDx11BlendState*)FindRS(strKey);

	if (!pState)
		return false;

	pState->AddTargetInfo(bEnable, eSrcBlend, eDestBlend, eBlendOp,
		eAlphaSrcBlend, eAlphaDestBlend, eAlphaBlendOp, iWriteMask);

	return true;
}

bool CDx11RenderManager::SortNormal(const CDx11GameObject * pObj1, const CDx11GameObject * pObj2)
{
	CDx11Scene*	pScene = DX11_GET_SINGLE(CDx11SceneManager)->GetCurrentScene();
	CDx11GameObject*	pCameraObj = pScene->GetCamera();
	CDx11Transform*	pCamTr = pCameraObj->GetTransform();

	Vec3	vCamPos = pCamTr->GetWorldPos();

	SAFE_RELEASE(pCamTr);
	SAFE_RELEASE(pCameraObj);
	SAFE_RELEASE(pScene);

	CDx11Transform*	pTr1 = pObj1->GetTransform();
	CDx11Transform*	pTr2 = pObj2->GetTransform();

	Vec3	vPos1 = pTr1->GetWorldPos();
	Vec3	vPos2 = pTr2->GetWorldPos();

	SAFE_RELEASE(pTr1);
	SAFE_RELEASE(pTr2);

	float	fDist1 = vPos1.Distance(vCamPos);
	float	fDist2 = vPos2.Distance(vCamPos);

	if (fDist1 > fDist2)
		return true;

	else
		return false;
}

bool CDx11RenderManager::SortAlpha(const CDx11GameObject * pObj1, const CDx11GameObject * pObj2)
{
	CDx11Scene*	pScene = DX11_GET_SINGLE(CDx11SceneManager)->GetCurrentScene();
	CDx11GameObject*	pCameraObj = pScene->GetCamera();
	CDx11Transform*	pCamTr = pCameraObj->GetTransform();

	Vec3	vCamPos = pCamTr->GetWorldPos();

	SAFE_RELEASE(pCamTr);
	SAFE_RELEASE(pCameraObj);
	SAFE_RELEASE(pScene);

	CDx11Transform*	pTr1 = pObj1->GetTransform();
	CDx11Transform*	pTr2 = pObj2->GetTransform();

	Vec3	vPos1 = pTr1->GetWorldPos();
	Vec3	vPos2 = pTr2->GetWorldPos();

	SAFE_RELEASE(pTr1);
	SAFE_RELEASE(pTr2);

	float	fDist1 = vPos1.Distance(vCamPos);
	float	fDist2 = vPos2.Distance(vCamPos);

	if (fDist1 < fDist2)
		return true;

	else
		return false;
}

bool CDx11RenderManager::SortUI(const CDx11GameObject * pObj1, const CDx11GameObject * pObj2)
{
	CDx11UI*	pUI1 = (CDx11UI*)pObj1->FindComponentFromType(CT_UI);
	CDx11UI*	pUI2 = (CDx11UI*)pObj2->FindComponentFromType(CT_UI);

	if (!pUI1) {
		return SortAlpha(pObj1, pObj2);
	}

	int	iZOrder1 = pUI1->GetZOrder();
	int	iZOrder2 = pUI2->GetZOrder();

	SAFE_RELEASE(pUI1);
	SAFE_RELEASE(pUI2);

	if (iZOrder1 < iZOrder2)
		return true;

	return false;
}
