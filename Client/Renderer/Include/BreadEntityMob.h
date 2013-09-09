
#ifndef _BreadEntityMob_h_
#define _BreadEntityMob_h_

#include "BreadEntityActor.h"

/*
	BreadEntityMob.h
	Copyright (c) prodongi. All rights resserved.

	date : 2012-06-dd
	purpose : 
*/

namespace Bread
{
	class cEntityMob : public cEntityActor
	{
		declareRTTI()

	public:
		cEntityMob();
		virtual ~cEntityMob();
	};
}



#endif