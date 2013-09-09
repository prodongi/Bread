
#ifndef _LoginState_h_
#define _LoginState_h_

/*
	BreadGameState.h
	Copyright (c) prodongi. All rights resserved.

	date : 2012-08-09
	purpose : 
*/

#include "BreadBaseState.h"

class cLoginState : public iBaseState
{
public:
	cLoginState();
	virtual ~cLoginState();

	virtual void enter(const char* prevState);
	virtual void update(float elapsedtime);
	virtual void exit(const char* nextState);
	virtual void override(const char* nextState);
	virtual void resume(const char* prevState);
};

#endif