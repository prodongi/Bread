
/*
	LoginState.cpp
	Copyright (c) prodongi. All rights resserved.
*/

#include "stdafx.h"
#include "LoginState.h"

/*
*/
cLoginState::cLoginState()
{
}
/*
*/
cLoginState::~cLoginState()
{
}
/*
*/
void cLoginState::enter(const char* prevState)
{
	unreferencedParameter(prevState);

	trace(_T("enter login state\n"));
}
/*
*/
void cLoginState::update(float elapsedtime)
{
	unreferencedParameter(elapsedtime);
}
/*
*/
void cLoginState::exit(const char* nextState)
{
	unreferencedParameter(nextState);
}
/*
*/
void cLoginState::override(const char* nextState)
{
	unreferencedParameter(nextState);
}
/*
*/
void cLoginState::resume(const char* prevState)
{
	unreferencedParameter(prevState);
}