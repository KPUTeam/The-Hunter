#include "CoordinateArrowSC.h"
#include "Resources/Dx11Mesh.h"
#include "Resources/Dx11ResourcesManager.h"
#include "GameObject/Dx11GameObject.h"
#include "Component/Dx11Renderer.h"
#include "Component/Dx11Transform.h"

CCoordinateArrowSC::CCoordinateArrowSC()
{
}

CCoordinateArrowSC::CCoordinateArrowSC(const CCoordinateArrowSC & script)
{
	*this = script;
}


CCoordinateArrowSC::~CCoordinateArrowSC()
{
}

bool CCoordinateArrowSC::Init()
{
	if (!m_pGameObject)
		return false;

	// Y축 화살표
	CDx11Renderer* pRenderer =  m_pGameObject->AddComponent<CDx11Renderer>("CoordinateArrowRenderer");
	pRenderer->SetMesh("CubeMesh");
	pRenderer->SetShader("DefaultShader");
	pRenderer->SetMaterialDiffuseTex("Default");

	CDx11Transform* tr = pRenderer->GetTransform();
	tr->Scaling(0.2, 100, 0.2);

	SAFE_RELEASE(tr);
	SAFE_RELEASE(pRenderer);

	return true;
}

int CCoordinateArrowSC::Update(float fTime)
{
	return 0;
}

int CCoordinateArrowSC::LateUpdate(float fTime)
{
	return 0;
}

void CCoordinateArrowSC::Render(float fTime)
{
}

CCoordinateArrowSC * CCoordinateArrowSC::Clone()
{
	return new CCoordinateArrowSC(*this);
}