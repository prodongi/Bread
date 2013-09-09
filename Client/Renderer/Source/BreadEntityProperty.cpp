
/*
	BreadEntityProperty.cpp
	Copyright (c) prodongi. All rights resserved.
*/

#include "BreadEntityProperty.h"

namespace Bread
{
	/*
	*/
	sEntityProperty::sEntityProperty() : m_radian(0.0f),
										 m_speed(0.0f)
	{
		m_position = sVector3(FLT_MAX, FLT_MAX, FLT_MAX);//sVector3::ZERO;
	}
}