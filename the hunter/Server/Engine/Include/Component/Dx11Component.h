#pragma once
#include "../Dx11Object.h"

DX11_BEGIN

class DX11_DLL CDx11Component :
	public CDx11Object
{
private:
	friend class CDx11GameObject;

protected:
	CDx11Component();
	CDx11Component(const CDx11Component& com);
	virtual ~CDx11Component() = 0;

protected:
	class CDx11Transform*	m_pTransform;
	class CDx11GameObject*	m_pGameObject;
	class CDx11Scene*		m_pScene;
	class CDx11Layer*		m_pLayer;
	COMPONENT_TYPE			m_eComType;

public:
	virtual bool Init() = 0;
	virtual int Update(float fTime) = 0;
	virtual int LateUpdate(float fTime) = 0;
	virtual void Render(float fTime) = 0;
	virtual CDx11Component* Clone() = 0;
	virtual void OnCollisionEnter(class CDx11Collider* pMy, class CDx11Collider* pYour, float fTime);
	virtual void OnCollision(class CDx11Collider* pMy, class CDx11Collider* pYour, float fTime);
	virtual void OnCollisionExit(class CDx11Collider* pMy, class CDx11Collider* pYour, float fTime);

public:
	virtual void SetTransform(CDx11Transform* pTransform);
	void SetGameObject(CDx11GameObject* pGameObject);
	void SetScene(CDx11Scene* pScene);
	void SetLayer(CDx11Layer* pLayer);

private:
	void Destroy();

public:
	CDx11GameObject* GetGameObject();
	CDx11Transform* GetTransform();
	CDx11Scene* GetScene();
	CDx11Layer* GetLayer();
	COMPONENT_TYPE GetComponentType();
};

DX11_END
