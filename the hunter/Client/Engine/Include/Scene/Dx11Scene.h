#pragma once
#include "../Dx11Object.h"
#include "../Component/Dx11Script.h"

DX11_BEGIN

class DX11_DLL CDx11Scene :
	public CDx11Object
{
private:
	friend class CDx11SceneManager;

private:
	CDx11Scene();
	~CDx11Scene();

private:
	vector<class CDx11Layer*>	m_vecLayer;
	vector<class CDx11Script*>	m_vecScript;
	list<class CDx11GameObject*>	m_LightList;
	unordered_map<string, class CDx11GameObject*>	m_mapCamera;
	class CDx11GameObject*		m_pCamera;
	class CDx11GameObject*		m_pUICamera;
	CDx11GameObject*			m_pSky;

public:
	CDx11GameObject* GetCamera();

public:
	bool Init();
	int Update(float fTime);
	int LateUpdate(float fTime);
	void AddCollision();
	void Render(float fTime);
	CDx11Layer*	CreateLayer(const string& strTag = "DefaultLayer");
	CDx11Layer* FindLayer(const string& strTag);
	CDx11GameObject* CreateLight(LIGHT_TYPE eLightType,
		const string& strTag = "Light");
	const list<CDx11GameObject*>* GetLightList();
	void Destroy();
	CDx11GameObject* CreateSky(const string& strTag,
		TCHAR* pFileName, const string& strTexKey,
		const string& strPathKey = DX11_TEXTURE_PATH);

	CDx11GameObject* CreatePerspectiveCamera(const string& strKey,
		const Vec3& vPos, float fViewAngle, float fNear, float fViewDist);
	CDx11GameObject* CreateOrthogonalCamera(const string& strKey,
		const Vec3& vPos, float fWidth, float fHeight, float fNear,
		float fViewDist);
	CDx11GameObject* FindCamera(const string& strKey);
	CDx11GameObject* FindGameObject(const string& strTag);

public:
	void AddScript(class CDx11Script* pScript);
	template <typename T>
	T*  AddScript(const string& strTag = "Script")
	{
		T*	p = new T;

		p->SetTag(strTag);
		p->SetScene(this);

		if (!p->Init())
		{
			SAFE_RELEASE(p);
			return NULL;
		}

		p->AddRef();

		m_vecScript.push_back(p);

		return p;
	}
};

DX11_END
