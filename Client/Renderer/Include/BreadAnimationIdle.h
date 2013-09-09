
#ifndef _BreadAnimationIdle_h_
#define _BreadAnimationIdle_h_

#include "BreadAnimationAction.h"

namespace Bread
{
	/*
		@date 2011.08.17
		@auth prodongi
		@desc walk action
		@todo 
	*/
	class cAnimationIdle : public cAnimationAction
	{
	public:
		cAnimationIdle();
		virtual ~cAnimationIdle();
		virtual void initialize(cEntityModel* /*entity*/) {};
		virtual bool action(float /*elapsedtime*/, cEntityModel* /*entity*/);
		virtual void terminate(cEntityModel* /*entity*/) {};
	};
}



#endif