#pragma once
#include "../Dx11Object.h"

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
	unordered_map<string, class CDx11GameObject*>	m_mapCamera;
	CDx11GameObject*		m_pMainCamera;
	list<class CDx11GameObject*>		m_FindList;
	list<class CDx11GameObject*>		m_LightList;
	CDx11GameObject*					m_pSky;

public:
	CDx11GameObject* GetMainCamera();
	list<class CDx11GameObject*>* GetLightList();

public:
	bool Init();
	void Input(float fTime);
	int Update(float fTime);
	int LateUpdate(float fTime);
	void Collision(float fTime);
	void Render(float fTime);

public:
	class CDx11Layer* CreateLayer(const string& strTag = "Layer");
	class CDx11Layer* FindLayer(const string& strTag);
	class CDx11GameObject* CreateCamera(const string& strTag, float fAngle,
		float fNear, float fFar);
	class CDx11GameObject* FindCamera(const string& strKey);
	void ChangeCamera(const string& strKey);
	class CDx11GameObject* FindObjectFromTag(const string& strTag);
	list<CDx11GameObject*>* FindObjectsFromTag(const string& strTag);
	class CDx11GameObject* CreateLight(LIGHT_TYPE eType, const string& strTag = "Light");
	void CreateSky(const string& strTexKey, TCHAR* pFileName,
		const string& strPathKey = TEXTURE_PATH);

public:
	template <typename T>
	T* CreateSceneScript(const string& strTag = "SceneScript")
	{
		T*	pScript = new T;

		pScript->SetTag(strTag);
		pScript->SetScene(this);

		if (!pScript->Init())
		{
			SAFE_RELEASE(pScript);
			return NULL;
		}

		pScript->AddRef();
		m_vecScript.push_back(pScript);

		return pScript;
	}
};

DX11_END
