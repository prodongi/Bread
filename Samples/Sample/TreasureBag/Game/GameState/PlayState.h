
#ifndef _PlayState_h_
#define _PlayState_h_

/*
	BreadGameState.h
	Copyright (c) prodongi. All rights resserved.

	date : 2012-08-09
	purpose : 
*/

#include "BreadBaseState.h"

class cPlayState : public iBaseState
{
public:
	cPlayState();
	virtual ~cPlayState();

	virtual void enter(const char* prevState);
	virtual void update(float elapsedtime);
	virtual void exit(const char* nextState);
	virtual void override(const char* nextState);
	virtual void resume(const char* prevState);
};

#endif