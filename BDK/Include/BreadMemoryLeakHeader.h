
#ifndef _BreadMemoryLeakHeader_h_
#define _BreadMemoryLeakHeader_h_

/// MFC에서는 충돌됨
#ifdef _DEBUG

#ifndef _AFXDLL
	#define _CRTDBG_MAP_ALLOC
	#include <crtdbg.h>

	/*
		_NORMAL_BLOCK : 표준 블럭(프로그램이 할당한 보통 메모리 블럭)
		_CLIENT_BLOCK : MFC(MFC 프로그램이 소멸자를 필요로 하는 개체에 대해 사용하는 특별한 메모리 블록)
		_CRT_BLOCK : c runtime(CRT 라이브러리가 자체 용도에 맞게 할당한 메모리 블록)
	*/
	#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif

#endif

namespace Bread
{
	void setMemoryLeakDebugFlag();
}

#endif