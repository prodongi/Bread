
#include "BreadAnimationEventManager.h"

namespace Bread
{
	implementInstance(cAnimationEventManager)

	/*
	*/
	cAnimationEventManager::cAnimationEventManager()
	{
		assignInstance(this)
	}
	/*
	*/
	cAnimationEventManager::~cAnimationEventManager()
	{
		assignInstance(NULL)
	}
}