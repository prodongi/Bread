
#ifndef _CustomClassCreater_h_
#define _CustomClassCreater_h_

#include "BreadClassCreater.h"

class cCustomClassCreater : public cClassCreater
{
public:
	cCustomClassCreater() {}
	virtual ~cCustomClassCreater() {}

	virtual cAnimationActionManager* newAnimationActionMgr();
	virtual cLuaManager* newLuaMgr();
	virtual cEntityManager* newEntityMgr();
	virtual cMsgManager* newMsgMgr();
};










#endif