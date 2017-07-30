#pragma once
#include "../Dx11Object.h"

DX11_BEGIN

enum CORNER_TYPE
{
	LEFT_TOP,
	RIGHT_TOP,
	LEFT_BOT,
	RIGHT_BOT
};

enum QUAD_RELATION_FRUSTRUM
{
	FRUSTRUM_OUT,
	FRUSTRUM_PARTIALLY_IN,
	FRUSTRUM_COMPLETELY_IN
};

class DX11_DLL QuadTree :
	public CDx11Object
{

private:
	friend class CDx11RenderManager;
	friend class CDx11CollisionManager;
	friend class CDx11SceneManager;

private:
	QuadTree();
	QuadTree(QuadTree* pParent);
	~QuadTree();

private:
	// ���� ��ǥ
	Vec3	m_vLeftTop;
	Vec3	m_vRightTop;
	Vec3	m_vLeftBot;
	Vec3	m_vRightBot;
	Vec3	m_vCenter;

	float	m_fXLen;
	float	m_fZLen;

	// �ø� ����
	bool	m_bCulling;

	// ���ϵ�
	QuadTree*	m_pChild[4];

	// �θ�
	QuadTree*	m_pParent;

	// ����Ʈ���� ����Ʈ
	vector<class CDx11GameObject*> m_QuadList;

	// ��� �� ������
	float	m_fRadius;

	// ���� ������Ʈ
	class CDx11GameObject*	m_pQTRender;

public:
	Vec3	GetCenter() const;
	float	GetRadius() const;
	QuadTree* GetChild(int i) const;
	bool	GetisCulling() const;
	CDx11GameObject* GetQTRender() const;

public:
	void SetNodeInfo(const Vec3& vLeftTop, const Vec3& vRightTop, const Vec3& vLeftBot, const Vec3& vRightBot);
	void SetParent(const Vec3& vLeftTop, const Vec3& vRightTop, const Vec3& vLeftBot, const Vec3& vRightBot);
	QuadTree* AddChild(const Vec3& vLeftTop, const Vec3& vRightTop, const Vec3& vLeftBot, const Vec3& vRightBot);
	bool SubDivide();
	bool BuildQuadTree();
	int IsInFrustrum();
	void FrustrumCulling();
	void PrintNode();
	bool IsInSquare(const Vec3& vMin, const Vec3& vMax);
	void AddQuadObj(class CDx11GameObject* pObj);
	void CheckNode(QuadTree* pNode, CDx11GameObject* pObj, const Vec3& vMin, const Vec3& vMax);
	void Collision(QuadTree* pNode, float fTime);
	void MakeTreeLoose(QuadTree* pNode);
	void MakeQTRender();
	void SetRenderFlag(QuadTree* pNode);
	void SetCulling(QuadTree* pNode, bool bCulling);

	void Reserve();
	void Clear();


public:
	Frustrum GetFrustrum() const;

};

DX11_END