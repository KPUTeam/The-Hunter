
#pragma once

#include "Dx11Types.h"

#define	CRTDBG_MAP_ALLOC
#define	new	new(_CLIENT_BLOCK, __FILE__, __LINE__)

#ifdef DX11_IMPORT
#define	DX11_DLL	__declspec(dllimport)
#else
#define	DX11_DLL	__declspec(dllexport)
#endif // DX11_IMPORT

// File Path Key
#define	ROOT_PATH		"RootPath"
#define	SHADER_PATH		"ShaderPath"
#define	TEXTURE_PATH		"TexturePath"

// Shader Key
#define	DEFAULT_SHADER	"DefaultShader"
#define	DEFAULT_TEX_SHADER	"DefaultTexShader"
#define SKY_SHADER	"SkyShader"


// Default Resource Key
#define	TRIANGLE_MESH	"TriangleMesh"
#define	RECT_TEX_MESH	"RectTexMesh"
#define SPHERE_MESH		"SphereMesh"

// Sampler Keys
#define	DEFAULT_SAMPLER	"DefaultSampler"

// PI
#define	DX11_PI		3.141592f

// StateKey
#define DX11_RS_DEPTH_LESSEQUAL "DepthLessEqual"
#define DX11_RS_WIREFRAME		"WireFrame"
#define DX11_RS_CULL_FRONTFACE	"CullFrontFace"
#define DX11_RS_ALPHABLEND		"AlphaBlend"


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

