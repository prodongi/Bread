
#ifndef _BreadEntityAvatar_h_
#define _BreadEntityAvatar_h_

#include "BreadEntityPlayer.h"

/*
	BreadEntityAvatar.h
	Copyright (c) prodongi. All rights resserved.

	date : 2012-06-dd
	purpose : �ڱ� �ڽ�
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