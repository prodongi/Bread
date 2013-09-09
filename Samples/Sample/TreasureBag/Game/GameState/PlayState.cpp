
/*
	PlayState.cpp
	Copyright (c) prodongi. All rights resserved.
*/

#include "stdafx.h"
#include "BreadSceneManager.h"
#include "BreadEnvironment.h"
#include "BreadWeather.h"
#include "PlayState.h"

/*
*/
cPlayState::cPlayState()
{
}
/*
*/
cPlayState::~cPlayState()
{
}
/*
*/
void cPlayState::enter(const char* prevState)
{
	trace(_T("enter play state\n"));
	unreferencedParameter(prevState);

	// create scene
	_getSceneMgr()->createScene(_T("plane.bsd"), 0);
	//_getSceneMgr()->createScene(_T("elf.bsd"), 0);

	// create avatar
	sMsgCreateAvatar* msgCe = new sMsgCreateAvatar;
	msgCe->m_filename = _T("skeleton.bmd");
	msgCe->m_buid = _T("avatar");
	msgCe->m_pos = sVector3(100.0f, 0.0f, -100.0f);//sVector3::ZERO;
	msgCe->m_radian = 0.0f;
	msgCe->m_shareType = 0;
	msgCe->m_loadingType = LOADING_BACKGROUND;
	_getMsgMgr()->addBucket(msgCe);

	// create decal
	sMsgCreateDecal* msgDecal = new sMsgCreateDecal;
	msgDecal->m_filename = _T("decal_test.bmd");
	msgDecal->m_buid = _T("decal_test");
	msgDecal->m_pos = sVector3(0.0, 0.0, 0.0f);
	msgDecal->m_radian = 0.0f;
	msgDecal->m_shareType = 0;
	msgDecal->m_loadingType = LOADING_BACKGROUND;
	_getMsgMgr()->addBucket(msgDecal);

	/// set weather
	_getMsgMgr()->addBucket(new sMsgSetWeather(cWeather::CLEAR));
}
/*
*/
void cPlayState::update(float elapsedtime)
{
	unreferencedParameter(elapsedtime);
}
/*
*/
void cPlayState::exit(const char* nextState)
{
	unreferencedParameter(nextState);
}
/*
*/
void cPlayState::override(const char* nextState)
{
	unreferencedParameter(nextState);
}
/*
*/
void cPlayState::resume(const char* prevState)
{
	unreferencedParameter(prevState);
}