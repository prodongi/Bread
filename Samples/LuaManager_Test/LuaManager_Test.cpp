// LuaManager_Test.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "BreadLuaManager.h"
#include "BreadDeclaration.h"
#include "BreadSingleton.h"

int cpp_func(int arg1, int arg2)
{
	return arg1 + arg2;
}

class cLuaManagerSingleton : public Bread::cLuaManager
{
	DECLARE_SINGLETON(cLuaManagerSingleton)

public:

	virtual ~cLuaManagerSingleton() {}
};

IMPLEMENT_SINGLETON(cLuaManagerSingleton)

cLuaManagerSingleton::cLuaManagerSingleton()
{
}

int _tmain(int argc, _TCHAR* argv[])
{
	
	cLuaManagerSingleton::getSingleton()->init();
	cLuaManagerSingleton::getSingleton()->registerFunc("cpp_func", cpp_func);
	cLuaManagerSingleton::getSingleton()->doFile("Script\\sample1.lua");

	int a = cLuaManagerSingleton::getSingleton()->getTableValue<int>("t", "b");
	char* h = cLuaManagerSingleton::getSingleton()->getTableValue<char*>("t", "h");
	cLuaManagerSingleton::getSingleton()->release();

	return 0;
}

