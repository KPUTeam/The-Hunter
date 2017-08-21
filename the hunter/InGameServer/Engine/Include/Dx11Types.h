
#pragma once

#define	_XM_NO_INTRINSICS_

#include <Windows.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <list>
#include <vector>
#include <unordered_map>
#include <process.h>
#include <conio.h>
#include <typeinfo.h>
#include <functional>
#include <crtdbg.h>
#include <atlcoll.h>
#include <algorithm>
#include "Resources\DirectXTex.h"

using namespace std;
using namespace DirectX;
using namespace DirectX::PackedVector;

#pragma comment(lib, "d3d11")
#pragma comment(lib, "d3dCompiler")

#if defined(_DEBUG1) | defined(DEBUG)
#pragma comment(lib, "DirectXTex")
#else
#pragma comment(lib, "DirectXTex")
#endif


#include "Dx11Macro.h"
#include "Dx11Flag.h"
#include "Key.h"
#include "Structure.h"
#include "Memory.h"


