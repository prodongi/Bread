
#ifndef _BreadAnimationEventManager_h_
#define _BreadAnimationEventManager_h_

#include "BreadBucketHash.h"
#include "BreadAnimationEvent.h"

#define _getAnimationEventMgr()	Bread::cAnimationEventManager::getInstance()

namespace Bread
{
	#define ANIMATIONEVENT_BUCKET_SIZE	10

	/*
		@date 2011.07.11
		@auth prodongi
		@desc 
		@todo 
	*/
	class cAnimationEventManager : public cBucketHash<BUID, cAnimationEvent*, ANIMATIONEVENT_BUCKET_SIZE>
	{
		declareInstance(cAnimationEventManager)
		declareClassCreater(cAnimationEventManager)
	};
}



#endif