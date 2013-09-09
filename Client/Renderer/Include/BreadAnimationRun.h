
#ifndef _BreadAnimationRun_h_
#define _BreadAnimationRun_h_

#include "BreadAnimationAction.h"

namespace Bread
{
	/*
		@date 2011.08.17
		@auth prodongi
		@desc walk action
		@todo 
	*/
	class cAnimationRun : public cAnimationAction
	{
	public:
		cAnimationRun();
		virtual ~cAnimationRun();
		virtual void initialize(cEntityModel* /*entity*/) {};
		virtual bool action(float /*elapsedtime*/, cEntityModel* /*entity*/);
		virtual void terminate(cEntityModel* /*entity*/) {};
	};
}



#endif