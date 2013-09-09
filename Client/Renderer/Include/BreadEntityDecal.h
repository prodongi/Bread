
#ifndef _BreadEntityDecal_h_
#define _BreadEntityDecal_h_

/*
	BreadEntityDecal.h
	Copyright (c) prodongi. All rights resserved.

	date : 2012-12-18
	purpose : 
*/

#include "BreadEntityModel.h"

namespace Bread
{
	/*
		Deferred Decal
	*/
	class cEntityDecal : public cEntityModel
	{
		declareRTTI()

	public:
		cEntityDecal();
		virtual ~cEntityDecal();
		virtual bool isDecal() const	{	return true; }
	};
}

#endif