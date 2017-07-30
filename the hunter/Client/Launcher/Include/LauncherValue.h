#pragma once

#include "Dx11Value.h"

#undef new

#if defined(DEBUG) | defined(_DEBUG1)
#pragma comment(lib, "Dx11Engine_Debug")
#else
#pragma comment(lib, "Dx11Engine")
#endif // defined(DEBUG) | defined(_DEBUG)

#pragma comment(lib, "libfbxsdk")

#pragma comment(lib, "DirectXTex")