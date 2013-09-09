
#ifndef _BreadAnimationAttack_h_
#define _BreadAnimationAttack_h_

#include "BreadAnimationAction.h"

namespace Bread
{
	/*
		@date 2011.08.17
		@auth prodongi
		@desc Attack action
		@todo 
	*/
	class cAnimationAttack : public cAnimationAction
	{
	public:
		cAnimationAttack();
		virtual ~cAnimationAttack();
		virtual void initialize(cEntityModel* /*entity*/) {};
		virtual bool action(float /*elapsedtime*/, cEntityModel* /*entity*/);
		virtual void terminate(cEntityModel* /*entity*/) {};
	};
}



#endif