#include "TankUI.h"
#include "Resources/Dx11Mesh.h"
#include "Resources/Dx11ResourcesManager.h"
#include "GameObject/Dx11GameObject.h"
#include "Component/Dx11Renderer.h"
#include "Component/Dx11Transform.h"
#include "Scene/Dx11Scene.h"
#include "Scene/Dx11layer.h"
#include "Component/Dx11Terrain.h"
#include "Component/Dx11ColliderSphere.h"
#include "Component/Dx11ColliderOBB.h"
#include "Component/Dx11UIBar.h"
#include "Component/Dx11BillBoard.h"
#include "Component/DX11FSM.h"
#include "Component/Dx11ColliderFrustrum.h"
#include "Core/Dx11Input.h"
#include "TankCameraSC.h"


CTankUISC::CTankUISC()
{
}

CTankUISC::CTankUISC(const CTankUISC & script)
{
}

CTankUISC::~CTankUISC()
{
}

bool CTankUISC::Init()
{
	return true;
}

int CTankUISC::Update(float fTime)
{
	return 0;
}

int CTankUISC::LateUpdate(float fTime)
{
	return 0;
}

void CTankUISC::Render(float fTime)
{
}

CTankUISC * CTankUISC::Clone()
{
	return new CTankUISC(*this);
}