#include "Monster.h"

#include "Resources/Dx11ResourcesManager.h"
#include "Resources/Dx11Texture.h"
#include "Resources/Dx11Mesh.h"

#include "GameObject/Dx11GameObject.h"
#include "Component/Dx11Renderer.h"
#include "Component/Dx11Transform.h"

#include "Scene/Dx11SceneManager.h"
#include "Scene/Dx11Layer.h"

CMonster::CMonster()
{
}

CMonster::CMonster(const CMonster & monster)	:
	CDx11Script(monster)
{
}

CMonster::~CMonster()
{
}

bool CMonster::Init()
{
	CreateMesh();

	// 렌더러를 생성한다.
	CDx11Renderer* pRenderer = m_pGameObject->AddComponent<CDx11Renderer>("MonsterRenderer");
	pRenderer->SetMesh("PyramidMesh");
	pRenderer->SetShader(DEFAULT_SHADER);
	SAFE_RELEASE(pRenderer);

	return true;
}

void CMonster::Input(float fTime)
{
}

int CMonster::Update(float fTime)
{
	Targeting(fTime);
	return 0;
}

int CMonster::LateUpdate(float fTime)
{
	return 0;
}

void CMonster::Render(float fTime)
{
}

CMonster * CMonster::Clone()
{
	return new CMonster(*this);
}

bool CMonster::CreateMesh()
{
	//피라미드 매쉬
	COLORVERTEX   tColorVtx2[5] =
	{
		{ Vec3(0.f, 0.75f, 0.f), Vec4(1.f, 0.f, 0.f, 1.f) },      //윗 꼭지점
		{ Vec3(0.5f, -0.5f, 0.5f), Vec4(0.f, 1.f, 0.f, 1.f) },
		{ Vec3(0.5f, -0.5f, -0.5f), Vec4(0.f, 0.f, 1.f, 1.f) },
		{ Vec3(-0.5f, -0.5f, -0.5f), Vec4(1.f, 1.f, 0.f, 1.f) },
		{ Vec3(-0.5f, -0.5f, 0.5f), Vec4(0.f, 1.f, 1.f, 1.f) }
	};
	UINT   iIndex2[18] = {
		0, 1, 2,
		0, 2, 3,
		0, 3, 4,
		0, 4, 1,
		1, 4, 3,
		3, 2, 1
	};

	CDx11Mesh*   pPyramidMesh = 
		DX11_GET_SINGLE(CDx11ResourcesManager)->
		CreateMesh("PyramidMesh", sizeof(COLORVERTEX), 5, tColorVtx2, sizeof(UINT), 18, iIndex2, DXGI_FORMAT_R32_UINT);
	SAFE_RELEASE(pPyramidMesh);

	return true;
}

bool CMonster::Targeting(float fTime)
{
	// 플레이어 오브젝트를 얻어온다.
	CDx11GameObject* pPlayer = m_pLayer->FindObjectFromTag("PlayerObject");
	CDx11Transform* pTr = pPlayer->GetTransform();
	Vec3 vPlayerPos = pTr->GetWorldPos();


	vPlayerPos -= m_pTransform->GetWorldPos();

	// m_vDir과 몬스터의 Y축을 내적
	float fDot = vPlayerPos.Dot(m_pTransform->GetWorldAxis(AXIS_Y));
	float fAngle = acos(fDot / vPlayerPos.Length());

	// 몬스터의 Y축과 m_vDir을 외적해서 회전 축을 구한다.
	Vec3 vRotation = m_pTransform->GetWorldAxis(AXIS_Y).Cross(vPlayerPos);

	m_pTransform->RotateWorld(fAngle, 0.5f, vRotation);
	m_pTransform->MoveWorld(3.f, fTime, AXIS_Y);

	SAFE_RELEASE(pPlayer);
	SAFE_RELEASE(pTr);

	return true;
}
