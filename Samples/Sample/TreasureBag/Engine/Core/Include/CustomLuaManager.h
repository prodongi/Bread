
#ifndef _CustomLuaManager_h_
#define _CustomLuaManager_h_

/*
	CustomLuaManager.h
	Copyright (c) prodongi. All rights resserved.

	date : 2012-06-05
	purpose : 
*/

#include "BreadLuaManager.h"
#include "Definition.h"

class cCustomLuaManager : public cLuaManager
{
	declareCustomClassCreater(cCustomLuaManager)

protected:
	virtual void registerPredefineMembers();
};

#endif
