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
		// @brief ���� ���� -z���� �ٶ󺸰� �ֱ� ������ x���� �ٶ󺸵��� �ϴ� ������
		setRevisionRadian(-cMath::HALFPI);
		setSpeed(100.0f);
	}
	/*
	*/
	cEntityActor::~cEntityActor()
	{
	}
}