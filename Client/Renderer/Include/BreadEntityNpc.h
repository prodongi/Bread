
#ifndef _BreadEntityNpc_h_
#define _BreadEntityNpc_h_

#include "BreadEntityActor.h"

/*
	BreadEntityNpc.h
	Copyright (c) prodongi. All rights resserved.

	date : 2012-06-dd
	purpose : 
*/

namespace Bread
{
	class cEntityNpc : public cEntityActor
	{
		declareRTTI()

	public:
		cEntityNpc();
		virtual ~cEntityNpc();
	};
}



#endif