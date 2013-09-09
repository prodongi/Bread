
#ifndef _BreadEntityPlayer_h_
#define _BreadEntityPlayer_h_

#include "BreadEntityActor.h"

/*
	BreadEntityPlayer.h
	Copyright (c) prodongi. All rights resserved.

	date : 2012-06-dd
	purpose : 
*/

namespace Bread
{
	class cEntityPlayer : public cEntityActor
	{
		declareRTTI()

	public:
		cEntityPlayer();
		virtual ~cEntityPlayer();
	};
}



#endif