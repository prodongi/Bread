
#ifndef _BreadEntityProperty_h_
#define _BreadEntityProperty_h_

/*
	BreadEntityProperty.h
	Copyright (c) prodongi. All rights resserved.

	date : 2012-06-dd
	purpose : 
*/

#include "BreadDeclaration.h"
#include "BreadVector3.h"

namespace Bread
{
	struct sEntityProperty
	{
		sEntityProperty();

		BUID m_buid;		
		sVector3 m_position;
		/// @brief πÊ«‚
		float m_radian;
		float m_speed;
	};
}

#endif