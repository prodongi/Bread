
#ifndef _BreadMemoryLeakHeader_h_
#define _BreadMemoryLeakHeader_h_

/// MFC������ �浹��
#ifdef _DEBUG

#ifndef _AFXDLL
	#define _CRTDBG_MAP_ALLOC
	#include <crtdbg.h>

	/*
		_NORMAL_BLOCK : ǥ�� ��(���α׷��� �Ҵ��� ���� �޸� ��)
		_CLIENT_BLOCK : MFC(MFC ���α׷��� �Ҹ��ڸ� �ʿ�� �ϴ� ��ü�� ���� ����ϴ� Ư���� �޸� ���)
		_CRT_BLOCK : c runtime(CRT ���̺귯���� ��ü �뵵�� �°� �Ҵ��� �޸� ���)
	*/
	#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif

#endif

namespace Bread
{
	void setMemoryLeakDebugFlag();
}

#endif