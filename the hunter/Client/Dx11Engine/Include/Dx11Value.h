
#pragma once

#pragma comment(lib, "libfbxsdk-md")
#include <fbxsdk.h>
#include "Dx11Types.h"

#ifdef _DEBUG1
#define	CRTDBG_MAP_ALLOC
#define	new	new(_CLIENT_BLOCK, __FILE__, __LINE__)
#endif

// Render Target Max
#define	DX11_RENDER_TARGET_MAX	8

#define	DX11_PI		3.141592f

#pragma warning(disable : 4251)
#pragma warning(disable : 4786)