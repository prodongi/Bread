
#ifndef _BreadEntityProp_h_
#define _BreadEntityProp_h_

#include "BreadEntityActor.h"

/*
	BreadEntityProp.h
	Copyright (c) prodongi. All rights resserved.

	date : 2012-06-dd
	purpose : 
*/

namespace Bread
{
	class cEntityProp : public cEntityActor
	{
		declareRTTI()

	public:
		cEntityProp();
		virtual ~cEntityProp();
	};
}



#endif