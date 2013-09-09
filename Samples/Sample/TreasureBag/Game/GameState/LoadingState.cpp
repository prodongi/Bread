
/*
	LoadingState.cpp
	Copyright (c) prodongi. All rights resserved.
*/

#include "stdafx.h"
#include "LoadingState.h"

/*
*/
cLoadingState::cLoadingState()
{
}
/*
*/
cLoadingState::~cLoadingState()
{
}
/*
*/
void cLoadingState::enter(const char* prevState)
{
	unreferencedParameter(prevState);
}
/*
*/
void cLoadingState::update(float elapsedtime)
{
	unreferencedParameter(elapsedtime);
}
/*
*/
void cLoadingState::exit(const char* nextState)
{
	unreferencedParameter(nextState);
}
/*
*/
void cLoadingState::override(const char* nextState)
{
	unreferencedParameter(nextState);
}
/*
*/
void cLoadingState::resume(const char* prevState)
{
	unreferencedParameter(prevState);
}