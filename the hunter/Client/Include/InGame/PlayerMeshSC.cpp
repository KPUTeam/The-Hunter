#include "PlayerMeshSC.h"
#include "Resources/Dx11Mesh.h"
#include "Resources/Dx11ResourcesManager.h"
#include "Component/Dx11Renderer.h"
#include "GameObject/Dx11GameObject.h"


CPlayerMeshSC::CPlayerMeshSC()
{
}


CPlayerMeshSC::~CPlayerMeshSC()
{
}

bool CPlayerMeshSC::Init()
{
	// 매쉬 로딩
	CDx11Mesh*	pMesh = DX11_GET_SINGLE(CDx11ResourcesManager)->LoadMesh("PlayerMesh", L"gun1.FBX");
	SAFE_RELEASE(pMesh);

	// 렌더러를 생성한다.
	CDx11Renderer*	pRenderer = m_pGameObject->AddComponent<CDx11Renderer>("PlayerRenderer");
	pRenderer->SetMesh("PlayerMesh");
	pRenderer->SetShader(DEFAULT_BUMP_SHADER);
	SAFE_RELEASE(pRenderer);

	return true;
}

void CPlayerMeshSC::Input(float fTime)
{
}

int CPlayerMeshSC::Update(float fTime)
{
	return 0;
}

int CPlayerMeshSC::LateUpdate(float fTime)
{
	return 0;
}

void CPlayerMeshSC::Render(float fTime)
{
}

CPlayerMeshSC * CPlayerMeshSC::Clone()
{
	return new CPlayerMeshSC(*this);
}

void CPlayerMeshSC::OnCollisionEnter(CDx11Collider * pColl, float fTime)
{
}

void CPlayerMeshSC::OnCollision(CDx11Collider * pColl, float fTime)
{
}

void CPlayerMeshSC::OnCollisionExit(CDx11Collider * pColl, float fTime)
{
}
