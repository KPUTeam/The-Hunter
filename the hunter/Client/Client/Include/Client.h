
#pragma once

#include "resource.h"
#include "Dx11Value.h"
#include "ClientKey.h"

#if defined(DEBUG) | defined(_DEBUG1)
#pragma comment(lib, "Dx11Engine_Debug")
#else
#pragma comment(lib, "Dx11Engine")
#endif // defined(DEBUG) | defined(_DEBUG)

