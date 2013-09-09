
#ifndef _BreadAnimationAction_h_
#define _BreadAnimationAction_h_

#include "BreadClientDefinition.h"

namespace Bread
{
	class cEntityModel;

	/*
		@date 2011.07.11
		@auth prodongi
		@desc 
		@todo 
	*/
	class cAnimationAction
	{
	public:
		cAnimationAction();
		virtual ~cAnimationAction();
		virtual void initialize(cEntityModel* /*entity*/) {};
		virtual bool action(float /*elapsedtime*/, cEntityModel* /*entity*/) pure;
		virtual void terminate(cEntityModel* /*entity*/) {};
		propertyConstFunc(BUID, Buid, m_buid)

	protected:
		BUID m_buid;
	};
}



#endif

