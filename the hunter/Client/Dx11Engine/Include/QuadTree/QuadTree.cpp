#include "QuadTree.h"

#include "../Core/Dx11Debug.h"
#include "../Component/Dx11Camera.h"
#include "../Scene/Dx11SceneManager.h"
#include "../Scene/Dx11Scene.h"
#include "../Scene/Dx11Layer.h"
#include "../GameObject/Dx11GameObject.h"
#include "../Component/Dx11Transform.h"

#include "../Component/Dx11ColliderAABB.h"

#include "../Core/Dx11CollisionManager.h"
#include "../Component/Dx11Renderer.h"
#include "../Component/Dx11UI.h"
#include "../Component/Dx11ColliderSphere.h"

DX11_USING

QuadTree::QuadTree() :
	m_vLeftTop(VEC3ZERO),
	m_vRightTop(VEC3ZERO),
	m_vLeftBot(VEC3ZERO),
	m_vRightBot(VEC3ZERO),
	m_vCenter(VEC3ZERO),
	m_fXLen(0),
	m_fZLen(0),
	m_bCulling(true),
	m_pParent(NULL),
	m_pQTRender(NULL)
{
	for (int i = 0; i < 4; ++i)
	{
		m_pChild[i] = NULL;
	}
}

QuadTree::QuadTree(QuadTree * pParent)
{
	// �ڽ� ��� ������
	for (int i = 0; i < 4; ++i)
	{
		m_pChild[i] = NULL;
	}

	m_bCulling = true;
	m_fRadius = 0.f;
	m_pParent = pParent;
}


QuadTree::~QuadTree()
{
	for (int i = 0; i < 4; ++i)
	{
		SAFE_DELETE(m_pChild[i]);
	}

	m_QuadList.clear();
	SAFE_RELEASE(m_pQTRender);
}

Vec3 QuadTree::GetCenter() const
{
	return m_vCenter;
}

float QuadTree::GetRadius() const
{
	return m_fRadius;
}

QuadTree * QuadTree::GetChild(int i) const
{
	return m_pChild[i];
}

bool QuadTree::GetisCulling() const
{
	return m_bCulling;
}

CDx11GameObject * QuadTree::GetQTRender() const
{
	m_pQTRender->AddRef();
	return m_pQTRender;
}


void QuadTree::SetNodeInfo(const Vec3 & vLeftTop, const Vec3 & vRightTop, const Vec3 & vLeftBot, const Vec3 & vRightBot)
{
	m_vLeftTop = vLeftTop;
	m_vRightTop = vRightTop;
	m_vLeftBot = vLeftBot;
	m_vRightBot = vRightBot;

	m_vCenter.x = (m_vLeftTop.x + m_vRightBot.x) / 2;
	m_vCenter.y = 0.f;
	m_vCenter.z = (m_vLeftTop.z + m_vRightBot.z) / 2;

	m_bCulling = false;
	m_fRadius = (m_vRightBot.x - m_vLeftTop.x) / 2;

	MakeQTRender();
}

void QuadTree::SetParent(const Vec3 & vLeftTop, const Vec3 & vRightTop, const Vec3 & vLeftBot, const Vec3 & vRightBot)
{
	// ��Ʈ�� �����ϰ� ����Ʈ���� ��������� ����
	SetNodeInfo(vLeftTop, vRightTop, vLeftBot, vRightBot);
	BuildQuadTree();
}

QuadTree * QuadTree::AddChild(const Vec3 & vLeftTop, const Vec3 & vRightTop, const Vec3 & vLeftBot, const Vec3 & vRightBot)
{
	QuadTree*	pChild = NULL;

	pChild = new QuadTree(this);
	pChild->SetNodeInfo(vLeftTop, vRightTop, vLeftBot, vRightBot);

	return pChild;
}

bool QuadTree::SubDivide()
{
	Vec3	TopCenter;
	Vec3	BotCenter;
	Vec3	LeftCenter;
	Vec3	RightCenter;

	TopCenter.x = m_vCenter.x;
	TopCenter.z = m_vLeftTop.z;

	BotCenter.x = m_vCenter.x;
	BotCenter.z = m_vRightBot.z;

	LeftCenter.x = m_vLeftTop.x;
	LeftCenter.z = m_vCenter.z;

	RightCenter.x = m_vRightBot.x;
	RightCenter.z = m_vCenter.z;


	if (m_fRadius <= 16.f)
		return false;


	m_pChild[LEFT_TOP] = AddChild(m_vLeftTop, TopCenter, LeftCenter, m_vCenter);
	m_pChild[RIGHT_TOP] = AddChild(TopCenter, m_vRightTop, m_vCenter, RightCenter);
	m_pChild[LEFT_BOT] = AddChild(LeftCenter, m_vCenter, m_vLeftBot, BotCenter);
	m_pChild[RIGHT_BOT] = AddChild(m_vCenter, RightCenter, BotCenter, m_vRightBot);

	return true;
}

bool QuadTree::BuildQuadTree()
{
	if (SubDivide())
	{
		m_pChild[LEFT_TOP]->BuildQuadTree();
		m_pChild[RIGHT_TOP]->BuildQuadTree();
		m_pChild[LEFT_BOT]->BuildQuadTree();
		m_pChild[RIGHT_BOT]->BuildQuadTree();
	}

	return true;
}

int QuadTree::IsInFrustrum()
{
	Frustrum fs = GetFrustrum();

	// ����Ʈ���� ��� �� �ȿ� ����ü�� �����ϴ� �� Ȯ��
	if (!fs.IsInSphereXZ(m_vCenter, m_fRadius * 2))
		return FRUSTRUM_OUT;

	// ����Ʈ���� �� ��ǥ�� ����ü�� ���ԵǴ��� Ȯ��
	bool	b[4];
	b[0] = fs.IsIn(m_vLeftTop);
	b[1] = fs.IsIn(m_vRightTop);
	b[2] = fs.IsIn(m_vLeftBot);
	b[3] = fs.IsIn(m_vRightBot);

	// ���� ���ԵǴ� ���
	if (b[0] + b[1] + b[2] + b[3] == 4)
		return FRUSTRUM_COMPLETELY_IN;

	// �Ϻθ� ���ԵǴ� ���
	return FRUSTRUM_PARTIALLY_IN;
}

Frustrum QuadTree::GetFrustrum() const
{
	CDx11Scene*	pScene = DX11_GET_SINGLE(CDx11SceneManager)->GetCurrentScene();
	CDx11GameObject* pCamObj = pScene->GetCamera();
	CDx11Camera*	pCamera = (CDx11Camera*)pCamObj->FindComponentFromType(CT_CAMERA);

	Frustrum fs = pCamera->GetFrustrum();

	SAFE_RELEASE(pCamera);
	SAFE_RELEASE(pCamObj);
	SAFE_RELEASE(pScene);

	return fs;
}


void QuadTree::FrustrumCulling()
{
	int ret = IsInFrustrum();

	switch (ret)
	{
	case FRUSTRUM_OUT:
		SetCulling(this, true);
		return;

	case FRUSTRUM_PARTIALLY_IN:
		// �Ϻθ� ���Ե������Ƿ� ������� �˻��� �ʿ�.
		m_bCulling = false;
		SetRenderFlag(this);
		break;

	case FRUSTRUM_COMPLETELY_IN:
		SetCulling(this, false);
		return;
	}

	// ������� �˻� ����
	for (int i = 0; i < 4; ++i)
	{
		if (m_pChild[i])
			m_pChild[i]->FrustrumCulling();
	}
}

void QuadTree::PrintNode()
{
	char pStr[80] = {};
	sprintf_s(pStr, "Center x : %lf, Center z : %lf, Radius : %lf\n", m_vCenter.x, m_vCenter.z, m_fRadius);
	DX11_GET_SINGLE(CDx11Debug)->OutputConsole(pStr);

	for (int i = 0; i < 4; ++i)
	{
		if (m_pChild[i]) m_pChild[i]->PrintNode();
	}
}

bool QuadTree::IsInSquare(const Vec3 & vMin, const Vec3 & vMax)
{
	// ������Ʈ�� ���� ��忡 ������ ���ԵǾ��ִ��� Ȯ���Ѵ�.

	if (vMin.x < m_vLeftBot.x || vMin.x > m_vRightTop.x)
		return false;
	if (vMin.z < m_vLeftBot.z || vMin.z > m_vRightTop.z)
		return false;
	if (vMax.x < m_vLeftBot.x || vMax.x > m_vRightTop.x)
		return false;
	if (vMax.z < m_vLeftBot.z || vMax.z > m_vRightTop.z)
		return false;

	return true;
}

void QuadTree::AddQuadObj(CDx11GameObject * pObj)
{
	// �ݸ��� ���Ǿ �� �˻��ؼ� �������� ���� ū ���� ã�´�.
	const list<CDx11Component*>*	pCollList = pObj->FindComponentsFromTypeName("CDx11ColliderSphere");

	list<CDx11Component*>::const_iterator	iter;
	list<CDx11Component*>::const_iterator	iterEnd = pCollList->end();

	// �������� ����ū ���Ǿ� �ݸ����� ����Ŵ
	CDx11ColliderSphere* pCollider = NULL;
	
	for (iter = pCollList->begin(); iter != iterEnd; ++iter)
	{
		CDx11ColliderSphere* iterCollider = NULL;

		iterCollider = (CDx11ColliderSphere*)(*iter);

		// ���� ��
		if (!pCollider)
		{
			// �������� �˻��ؼ� �� ũ�� ����ִ´�.
			if (pCollider->GetRadius() < iterCollider->GetRadius())
				pCollider = iterCollider;
		}

		// ���� ��
		else
			pCollider = iterCollider;
	}

	Vec3	vCenter;
	float	fXLen;
	float	fZLen;

	// ������ �׳� x z 1 1 �� �����Ѵ�.
	if (!pCollider)
	{
		CDx11Transform* pTr = pObj->GetTransform();
		vCenter = pTr->GetWorldPos();
		fXLen = 1.f;
		fZLen = 1.f;
		SAFE_RELEASE(pTr);
	}

	else
	{
		vCenter = pCollider->GetCenter();
		fXLen = pCollider->GetRadius();
		fZLen = pCollider->GetRadius();
	}
	SAFE_RELEASE(pCollider);


	Vec3	vMin;
	Vec3	vMax;

	vMin.x = vCenter.x - fXLen;
	vMin.y = 0.f;
	vMin.z = vCenter.z - fZLen;

	vMax.x = vCenter.x + fXLen;
	vMax.y = 0.f;
	vMax.z = vCenter.z + fZLen;

	// ��Ʈ ��忡�� �翬�� ���Ե� ���̹Ƿ� �ٷ� CheckNode�Լ��� ȣ��
	CheckNode(this, pObj, vMin, vMax);
}

void QuadTree::CheckNode(QuadTree * pNode, CDx11GameObject * pObj, const Vec3 & vMin, const Vec3 & vMax)
{
	for (int i = 0; i < 4; ++i)
	{
		if (pNode->m_pChild[i])
		{
			if (pNode->m_pChild[i]->IsInSquare(vMin, vMax))
			{
				// �ϳ��� true�� ���
				// �ش� ���ϵ� ����� ���ϵ带 �˻�
				CheckNode(pNode->m_pChild[i], pObj, vMin, vMax);
				return;
			}
		}
		else
			break;
	}

	// ��� false �� ���
	// �θ� ��忡 Ǫ����
	pNode->m_QuadList.push_back(pObj);
}

void QuadTree::Collision(QuadTree * pNode, float fTime)
{
	DX11_GET_SINGLE(CDx11CollisionManager)->Collision(fTime);

	// �ڽ��� ������
	if (pNode->m_pChild[0])
	{
		for (int i = 0; i < 4; ++i)
		{
			Collision(pNode->m_pChild[i], fTime);
		}
	}
}

void QuadTree::MakeTreeLoose(QuadTree * pNode)
{
	float fXLen = pNode->m_vCenter.x - pNode->m_vLeftTop.x;
	float fZLen = pNode->m_vCenter.z - pNode->m_vLeftBot.z;

	pNode->m_vLeftTop.x -= fXLen;
	pNode->m_vLeftTop.z += fZLen;

	pNode->m_vRightTop.x += fXLen;
	pNode->m_vRightTop.z += fZLen;

	pNode->m_vLeftBot.x -= fXLen;
	pNode->m_vLeftBot.z -= fZLen;

	pNode->m_vRightBot.x += fXLen;
	pNode->m_vRightBot.z -= fZLen;

	pNode->m_fRadius *= 2;

	if (pNode->m_pChild[0])
	{
		for (int i = 0; i < 4; ++i)
		{
			MakeTreeLoose(pNode->m_pChild[i]);
		}
	}
}

void QuadTree::MakeQTRender()
{
	m_pQTRender = CDx11GameObject::Create("QT");

	CDx11Transform*		pTr = m_pQTRender->GetTransform();
	pTr->SetWorldPos((m_vCenter.x + 400) / 4, (m_vCenter.z + 400) / 4, 0.f);
	pTr->Scaling(m_fRadius, m_fRadius, 1.f);
	SAFE_RELEASE(pTr);

	CDx11Renderer*	pRenderer = m_pQTRender->AddComponent<CDx11Renderer>("QTRenderer");
	pRenderer->SetMesh("UIMesh");
	pRenderer->SetShader("ColliderShader");
	pRenderer->SetRenderState(DX11_RS_DEPTH_DISABLE, RSF_DEPTHSTENCIL);
	pRenderer->SetRenderState(DX11_RS_WIREFRAME, RSF_RASTERIZER);
	SAFE_RELEASE(pRenderer);

	//CDx11UI*	pQTUI = m_pQTRender->AddComponent<CDx11UI>("QTUI");
	//SAFE_RELEASE(pQTUI);
}

void QuadTree::SetRenderFlag(QuadTree * pNode)
{
	CDx11Renderer*	pRenderer = (CDx11Renderer*)pNode->m_pQTRender->FindComponentFromType(CT_RENDERER);
	if (pNode->m_bCulling)
	{
		pRenderer->SetRenderState(DX11_RS_WIREFRAME, RSF_RASTERIZER);
	}
	else
	{
		pRenderer->SetRenderState(DX11_RS_SOLID, RSF_RASTERIZER);
	}
	SAFE_RELEASE(pRenderer);
}

void QuadTree::SetCulling(QuadTree * pNode, bool bCulling)
{
	pNode->m_bCulling = bCulling;
	SetRenderFlag(pNode);

	if (pNode->m_pChild[0])
	{
		for (int i = 0; i < 4; ++i)
		{
			SetCulling(pNode->m_pChild[i], bCulling);
		}
	}
}

void QuadTree::Reserve()
{
	m_QuadList.reserve(30);

	if (m_pChild[0])
	{
		for (int i = 0; i < 4; ++i)
		{
			m_pChild[i]->Reserve();
		}
	}
}

void QuadTree::Clear()
{
	if(m_QuadList.size() > 0)
		m_QuadList.clear();


	if (m_pChild[0])
	{
		for (int i = 0; i < 4; ++i)
		{
			m_pChild[i]->Clear();
		}
	}
}
