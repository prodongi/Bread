/*
	BreadMemoryLeakHeader.cpp
	Copyright (c) prodongi. All rights resserved.
*/

#include "BreadMemoryLeakHeader.h"

namespace Bread
{
	/*
	*/
	void setMemoryLeakDebugFlag()
	{
#ifdef _DEBUG
#ifndef _AFXDLL
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);// | _CRTDBG_CHECK_ALWAYS_DF);
#endif
#endif
	}
}