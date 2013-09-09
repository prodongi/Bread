
/*
	CustomLuaManager.cpp
	Copyright (c) prodongi. All rights resserved.
*/

#include "stdafx.h"
#include "CustomLuaManager.h"
#include "CustomAnimationActionEnum.h"

/*
*/
cCustomLuaManager::cCustomLuaManager()
{
}
/*
*/
cCustomLuaManager::~cCustomLuaManager()
{
}
/*
*/
void cCustomLuaManager::registerPredefineMembers()
{
	cLuaManager::registerPredefineMembers();

	registerPredefineMember("ANI_DAMAGE", ANI_DAMAGE);
}
