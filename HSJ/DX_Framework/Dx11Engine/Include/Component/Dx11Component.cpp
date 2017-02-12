#include "Dx11Component.h"
#include "../GameObject/Dx11GameObject.h"

DX11_USING

CDx11Component::CDx11Component()	:
	m_pScene(NULL),
	m_pLayer(NULL),
	m_pGameObject(NULL),
	m_pTransform(NULL)
{
}

CDx11Component::CDx11Component(const CDx11Component & com)
{
	*this = com;
	m_iRefCount = 1;
}

CDx11Component::~CDx11Component()
{
}

void CDx11Component::SetScene(CDx11Scene * pScene)
{
	m_pScene = pScene;
}

void CDx11Component::SetLayer(CDx11Layer * pLayer)
{
	m_pLayer = pLayer;
}

void CDx11Component::SetGameObject(CDx11GameObject * pGameObject)
{
	m_pGameObject = pGameObject;
}

void CDx11Component::SetTransform(CDx11Transform * pTransform)
{
	m_pTransform = pTransform;
}

CDx11GameObject * CDx11Component::GetGameObject()
{
	m_pGameObject->AddRef();
	return m_pGameObject;
}

COMPONENT_TYPE CDx11Component::GetComponentType()
{
	return m_eComType;
}

bool CDx11Component::Init()
{
	return true;
}

void CDx11Component::Input(float fTime)
{
}

int CDx11Component::Update(float fTime)
{
	return 0;
}

int CDx11Component::LateUpdate(float fTime)
{
	return 0;
}

void CDx11Component::Render(float fTime)
{
}
