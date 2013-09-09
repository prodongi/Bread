/*
	BreadEntityActor.cpp
	Copyright (c) prodongi. All rights resserved.
*/

#include "BreadEntityActor.h"
#include "BreadMath.h"

namespace Bread
{
	implementRTTI(cEntityActor, cEntityModel)

	/*
	*/
	cEntityActor::cEntityActor()
	{
		// @brief 보통 모델이 -z축을 바라보고 있기 때문에 x축을 바라보도록 하는 보정값
		setRevisionRadian(-cMath::HALFPI);
		setSpeed(100.0f);
	}
	/*
	*/
	cEntityActor::~cEntityActor()
	{
	}
}