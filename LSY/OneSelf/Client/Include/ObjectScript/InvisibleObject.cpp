#include "InvisibleObject.h"
#include "Core/Dx11Input.h"
#include "Scene/Dx11Layer.h"
#include "GameObject/Dx11GameObject.h"
#include "Component/Dx11Transform.h"


CInvisibleObject::CInvisibleObject()
{
}


CInvisibleObject::~CInvisibleObject()
{
}

bool CInvisibleObject::Init()
{
	DX11_GET_SINGLE(CDx11Input)->CreateKey("RotXFront", 'X');
	DX11_GET_SINGLE(CDx11Input)->CreateKey("RotXBack", 'C');
	DX11_GET_SINGLE(CDx11Input)->CreateKey("RotYFront", 'Y');
	DX11_GET_SINGLE(CDx11Input)->CreateKey("RotYBack", 'U');
	DX11_GET_SINGLE(CDx11Input)->CreateKey("RotZFront", 'D');
	DX11_GET_SINGLE(CDx11Input)->CreateKey("RotZBack", 'A');
	DX11_GET_SINGLE(CDx11Input)->CreateKey("MoveFront", 'W');
	DX11_GET_SINGLE(CDx11Input)->CreateKey("MoveBack", 'S');

	return true;
}

void CInvisibleObject::Input(float fTime)
{
	if (DX11_GET_SINGLE(CDx11Input)->KeyDown("RotZBack") ||
		DX11_GET_SINGLE(CDx11Input)->KeyPush("RotZBack"))
	{
		m_pTransform->RotationWorld(DX11_PI, fTime, AXIS_Z);
	}

	if (DX11_GET_SINGLE(CDx11Input)->KeyDown("RotZFront") ||
		DX11_GET_SINGLE(CDx11Input)->KeyPush("RotZFront"))
	{
		m_pTransform->RotationWorld(-DX11_PI, fTime, AXIS_Z);
	}

	if (DX11_GET_SINGLE(CDx11Input)->KeyDown("RotXBack") ||
		DX11_GET_SINGLE(CDx11Input)->KeyPush("RotXBack"))
	{
		m_pTransform->RotationWorld(DX11_PI, fTime, AXIS_X);
	}

	if (DX11_GET_SINGLE(CDx11Input)->KeyDown("RotXFront") ||
		DX11_GET_SINGLE(CDx11Input)->KeyPush("RotXFront"))
	{
		m_pTransform->RotationWorld(-DX11_PI, fTime, AXIS_X);
	}

	if (DX11_GET_SINGLE(CDx11Input)->KeyDown("RotYBack") ||
		DX11_GET_SINGLE(CDx11Input)->KeyPush("RotYBack"))
	{
		m_pTransform->RotationWorld(DX11_PI, fTime, AXIS_Y);
	}

	if (DX11_GET_SINGLE(CDx11Input)->KeyDown("RotYFront") ||
		DX11_GET_SINGLE(CDx11Input)->KeyPush("RotYFront"))
	{
		m_pTransform->RotationWorld(-DX11_PI, fTime, AXIS_Y);
	}

	if (DX11_GET_SINGLE(CDx11Input)->KeyDown("MoveFront") ||
		DX11_GET_SINGLE(CDx11Input)->KeyPush("MoveFront"))
	{
		m_pTransform->MoveWorld(5.f, fTime, AXIS_Y);
	}

	if (DX11_GET_SINGLE(CDx11Input)->KeyDown("MoveBack") ||
		DX11_GET_SINGLE(CDx11Input)->KeyPush("MoveBack"))
	{
		m_pTransform->MoveWorld(-5.f, fTime, AXIS_Y);
	}
}

int CInvisibleObject::Update(float fTime)
{
	m_pTransform->RotationWorld(DX11_PI / 2, fTime, AXIS_Y);

	return 0;
}

int CInvisibleObject::LateUpdate(float fTime)
{
	return 0;
}

void CInvisibleObject::Render(float fTime)
{
}

CInvisibleObject * CInvisibleObject::Clone()
{
	return new CInvisibleObject(*this);
}
