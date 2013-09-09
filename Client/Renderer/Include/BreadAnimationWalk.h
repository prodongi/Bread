
#ifndef _BreadAnimationWalk_h_
#define _BreadAnimationWalk_h_

#include "BreadAnimationAction.h"

namespace Bread
{
	/*
		@date 2011.08.17
		@auth prodongi
		@desc walk action
		@todo 
	*/
	class cAnimationWalk : public cAnimationAction
	{
	public:
		cAnimationWalk();
		virtual ~cAnimationWalk();
		virtual void initialize(cEntityModel* /*entity*/) {};
		virtual bool action(float /*elapsedtime*/, cEntityModel* /*entity*/);
		virtual void terminate(cEntityModel* /*entity*/) {};
	};
}



#endif