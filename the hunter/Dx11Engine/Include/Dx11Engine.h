
// Engine.h
// DLL 헤더 파일
// 클라이언트는 엔진 헤더를 인클루드하면 쓸 수 있다.

#pragma once
#include <fbxsdk.h>
#include "Dx11Types.h"

//디버그 관련 매크로
#define CRTDBG_MAP_ALLOC
#define new new(_CLIENT_BLOCK, __FILE__, __LINE__)

#ifdef DX11_IMPORT
#define	DX11_DLL	__declspec(dllimport)
#else
#define	DX11_DLL	__declspec(dllexport)
#endif // DX11_IMPORT


// File Path Key
#define	SOLUTION_PATH		"SolutionPath"
#define	SHADER_PATH			"ShaderPath"
#define	TEXTURE_PATH		"TexturePath"
#define	ENGINE_PATH			"EnginePath"
#define ENGINE_TEXTURE_PATH	"EngineTexturePath"
#define MESH_PATH			"MeshPath"

// Shader Key
#define	DEFAULT_SHADER		"DefaultShader"
#define	DEFAULT_TEX_SHADER	"DefaultTexShader"
#define DEFAULT_BUMP_SHADER	"DefaultBumpShader"
#define	SKY_SHADER			"SkyShader"
#define COLLIDER_SHADER		"ColliderShader"
#define	UI_SHADER			"UIShader"
#define EFFECT_SHADER		"EffectShader"

// Shader Key
#define	DEFAULT_SAMPLER	"DefaultSampler"

// Resource Key
#define	TRIANGLE_MESH	"TriangleMesh"
#define	PYRAMID_MESH	"PyramidMesh"
#define	RECT_TEX_MESH	"RectTexMesh"
#define	RECT_UI_MESH	"RectUIMesh"
#define	SPHERE_MESH		"SphereMesh"
#define EFFECT_MESH		"EffectCenterMesh"
#define EFFECT_LT_MESH	"EffectLTMesh"

// PI
#define DX11_PI		3.141592f

// State Key
#define	DX11_RS_DEPTH_LESSEQUAL	"DepthLessEqual"
#define	DX11_RS_WIREFRAME		"WireFrame"
#define	DX11_RS_CULL_FRONTFACE	"CullFrontFace"
#define	DX11_RS_CULL_NONE		"CullNone"
#define DX11_RS_ALPHABLEND		"AlphaBlend"
#define DX11_RS_DEPTH_DISABLE	"DepthDisable"


template <typename T>
void Safe_Delete_Map(T& p)
{
	T::iterator	iter = p.begin();

	while (iter != p.end())
	{
		SAFE_DELETE(iter->second);
		++iter;
	}

	p.clear();
}

template <typename T>
void Safe_Release_Map(T& p)
{
	T::iterator	iter = p.begin();

	while (iter != p.end())
	{
		SAFE_RELEASE(iter->second);
		++iter;
	}

	p.clear();
}


template <typename T>
void Safe_Delete_VecList(T& p)
{
	T::iterator	iter = p.begin();

	while (iter != p.end())
	{
		SAFE_DELETE((*iter));
		++iter;
	}

	p.clear();
}

template <typename T>
void Safe_Release_VecList(T& p)
{
	T::iterator	iter = p.begin();

	while (iter != p.end())
	{
		SAFE_RELEASE((*iter));
		++iter;
	}

	p.clear();
}