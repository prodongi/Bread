
#ifndef _BreadAnimationDead_h_
#define _BreadAnimationDead_h_

#include "BreadAnimationAction.h"

namespace Bread
{
	/*
		@date 2011.08.17
		@auth prodongi
		@desc Dead action
		@todo 
	*/
	class cAnimationDead : public cAnimationAction
	{
	public:
		cAnimationDead();
		virtual ~cAnimationDead();
		virtual void initialize(cEntityModel* /*entity*/) {};
		virtual bool action(float /*elapsedtime*/, cEntityModel* /*entity*/);
		virtual void terminate(cEntityModel* /*entity*/) {};
	};
}



#endif