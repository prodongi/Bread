
#ifndef _CustomAnimationActionManager_h_
#define _CustomAnimationActionManager_h_

#include "BreadAnimationActionManager.h"

class cCustomAnimationActionManager : public cAnimationActionManager
{
public:
	cCustomAnimationActionManager();
	virtual ~cCustomAnimationActionManager();

protected:
	virtual cAnimationAction* createAction(int type);
};

#endif