
#ifndef _BreadEntityActor_h_
#define _BreadEntityActor_h_

#include "BreadEntityModel.h"

/*
	BreadEntityActor.h
	Copyright (c) prodongi. All rights resserved.

	date : 2012-06-dd
	purpose : 
*/

namespace Bread
{
	class cEntityActor : public cEntityModel
	{
		declareRTTI()

	public:
		cEntityActor();
		virtual ~cEntityActor();
	};
}



#endif