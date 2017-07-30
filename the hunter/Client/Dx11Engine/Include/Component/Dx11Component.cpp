#include "Dx11Component.h"
#include "Dx11Transform.h"
#include "../GameObject/Dx11GameObject.h"
#include "../Scene/Dx11Scene.h"
#include "../Scene/Dx11Layer.h"

DX11_USING

CDx11Component::CDx11Component()	:
	m_pTransform(NULL),
	m_pGameObject(NULL),
	m_pScene(NULL),
	m_pLayer(NULL)
{
	SetTypeName<CDx11Component>();
}

CDx11Component::CDx11Component(const CDx11Component & com)
{
	*this = com;
	m_iRef = 1;
	m_pScene = NULL;
	m_pLayer = NULL;
	m_pGameObject = NULL;
	m_pTransform = NULL;
}

CDx11Component::~CDx11Component()
{
}

void CDx11Component::SetTransform(CDx11Transform * pTransform)
{
	m_pTransform = pTransform;
}

void CDx11Component::SetGameObject(CDx11GameObject * pGameObject)
{
	m_pGameObject = pGameObject;
}

void CDx11Component::SetScene(CDx11Scene * pScene)
{
	m_pScene = pScene;
}

void CDx11Component::SetLayer(CDx11Layer * pLayer)
{
	m_pLayer = pLayer;
}

void CDx11Component::Destroy()
{
	delete	this;
}

CDx11GameObject * CDx11Component::GetGameObject()
{
	m_pGameObject->AddRef();
	return m_pGameObject;
}

CDx11Transform * CDx11Component::GetTransform()
{
	m_pTransform->AddRef();
	return m_pTransform;
}

CDx11Scene * CDx11Component::GetScene()
{
	m_pScene->AddRef();
	return m_pScene;
}

CDx11Layer * CDx11Component::GetLayer()
{
	m_pLayer->AddRef();
	return m_pLayer;
}

COMPONENT_TYPE CDx11Component::GetComponentType()
{
	return m_eComType;
}

void CDx11Component::OnCollisionEnter(class CDx11Collider* pMy, class CDx11Collider* pYour, float fTime)
{
}

void CDx11Component::OnCollision(class CDx11Collider* pMy, class CDx11Collider* pYour, float fTime)
{
}

void CDx11Component::OnCollisionExit(class CDx11Collider* pMy, class CDx11Collider* pYour, float fTime)
{
}
