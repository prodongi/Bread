
#ifndef _BreadAnimationEvent_h_
#define _BreadAnimationEvent_h_

#include "BreadDeclaration.h"

namespace Bread
{
	/*
	*/
	struct sAnimationEvent
	{
		int type;
	};
	/*
	*/
	struct sAnimationEventMotion
	{
		~sAnimationEventMotion()
		{
			m_event.clear();
		}
		std::vector<sAnimationEvent> m_event;
	};
	/*
		@date 2011.07.11
		@auth prodongi
		@desc 
		@todo 
	*/
	class cAnimationEvent
	{
	public:
		cAnimationEvent();
		~cAnimationEvent();
		void initialize(int motionNum);
		void finalize();
		void copy(cAnimationEvent const* srcData);
		void add(int motion, char const* type);

	private:
		int getType(char const* type);

	private:
		enum	{	TYPE_SOUND	};

	private:
		std::vector<sAnimationEventMotion> m_motion;
	};
}



#endif