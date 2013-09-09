
#ifndef _AnimationDamage_h_
#define _AnimationDamage_h_

/*
	AnimationDamage.h
	Copyright (c) prodongi. All rights resserved.

	date : 2012-06-05
	purpose : 
*/

#include "BreadAnimationAction.h"

class cAnimationDamage : public cAnimationAction
{
public:
	cAnimationDamage();
	virtual ~cAnimationDamage();
	virtual void initialize(cEntityModel* /*entity*/) {};
	virtual bool action(float /*elapsedtime*/, cEntityModel* /*entity*/);
	virtual void terminate(cEntityModel* /*entity*/) {};
};

#endif