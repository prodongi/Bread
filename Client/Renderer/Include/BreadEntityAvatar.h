
#ifndef _BreadEntityAvatar_h_
#define _BreadEntityAvatar_h_

#include "BreadEntityPlayer.h"

/*
	BreadEntityAvatar.h
	Copyright (c) prodongi. All rights resserved.

	date : 2012-06-dd
	purpose : 자기 자신
*/

namespace Bread
{
	class cEntityAvatar : public cEntityPlayer
	{
		declareRTTI()

	public:
		cEntityAvatar();
		virtual ~cEntityAvatar();
	};
}



#endif