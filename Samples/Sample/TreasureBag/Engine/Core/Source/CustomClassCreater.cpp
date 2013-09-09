
#include "stdafx.h"
#include "CustomAnimationActionManager.h"
#include "CustomEntityManager.h"
#include "CustomClassCreater.h"
#include "CustomLuaManager.h"
#include "CustomMsgManager.h"

/*
*/
cAnimationActionManager* cCustomClassCreater::newAnimationActionMgr()
{
	return new cCustomAnimationActionManager;
}
/*
*/
cLuaManager* cCustomClassCreater::newLuaMgr()
{
	return new cCustomLuaManager;
}
/*
*/
cEntityManager* cCustomClassCreater::newEntityMgr()
{
	return new cCustomEntityManager;
}
/*
*/
cMsgManager* cCustomClassCreater::newMsgMgr()
{
	return new cCustomMsgManager;
}
