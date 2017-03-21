#pragma once
#include "../Include.h"

#ifdef _MSC_VER
#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#ifndef _CONSOLE
#endif // _MSC_VER



class CMemoryLeak
{
public:
	CMemoryLeak()
	{
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

		// 콘솔 프로그램일 경우
#ifdef _CONSOLE
		_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
		_CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDOUT);
		_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_FILE);
		_CrtSetReportFile(_CRT_ERROR, _CRTDBG_FILE_STDOUT);
		_CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_FILE);
		_CrtSetReportFile(_CRT_ASSERT, _CRTDBG_FILE_STDOUT);

#define DEBUG_NORMALBLOCK	new (_NORMAL_BLOCK, __FILE__, __LINE__)
#ifdef  new
#undef	new
#endif //  new
#define new DEBUG_NORMALBLOCK

#else
		_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);
		_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG);
		_CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_DEBUG);
#endif

#ifdef malloc
#undef malloc
#endif // malloc
#define maloc(s) (_malloc_dbg(s, _NORMAL_BLOCK, __FILE__, __LINE))

	}
};

// 초기화를 생성자를 통해 자동으로 해주기 위해 전역으로 선언한다.
static CMemoryLeak MemoryLeak;

#endif
#endif

// 디버그 상태일 때만 사용할 수 있게 되어있다.
// #include "MemoryLeak.h"
// int main()
// {
//     BYTE* pData = new BYTE[1024];
//     ZeroMemory(pData, 1024 * sizeof(BYTE));
//     return 0;
// }