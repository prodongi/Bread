
#include "stdafx.h"
#include "CustomAnimationActionManager.h"
#include "CustomAnimationActionEnum.h"
#include "AnimationDamage.h"

/*
*/
cCustomAnimationActionManager::cCustomAnimationActionManager()
{
}
/*
*/
cCustomAnimationActionManager::~cCustomAnimationActionManager()
{
}
/*
*/
cAnimationAction* cCustomAnimationActionManager::createAction(int type)
{
	cAnimationAction* action;
	action = cAnimationActionManager::createAction(type);
	if (action)
		return action;

	switch (type)
	{
	case ANI_DAMAGE:	return new cAnimationDamage;
	}

	assert(0 && _T("failed create animation action, invalid type"));
	return NULL;
}