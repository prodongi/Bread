
/*
	BreadGameState.cpp
	Copyright (c) prodongi. All rights resserved.
*/

#include "stdafx.h"
#include "BreadStateManager.h"
#include "GameStateManager.h"
#include "LoadingState.h"
#include "LoginState.h"
#include "PlayState.h"

implementInstance(cGameStateManager)

/*
*/
cGameStateManager::cGameStateManager() : m_stateMgr(NULL)
{
	assignInstance(this)
}
/*
*/
cGameStateManager::~cGameStateManager()
{
	assignInstance(NULL)
}
/*
*/
bool cGameStateManager::initialize()
{
	assert(!m_stateMgr);
	m_stateMgr = new cStateManager;
	m_stateMgr->init();

	registerState();

	return true;
}
/*
*/
void cGameStateManager::registerState()
{
	assert(m_stateMgr);
	
	iBaseState* loginState = new cLoginState;
	m_stateMgr->registerState("login", loginState);

	iBaseState* playState = new cPlayState;
	m_stateMgr->registerState("play", playState);

	iBaseState* loadingState = new cLoadingState;
	m_stateMgr->registerState("loading", loadingState);
}
/*
*/
void cGameStateManager::finalize()
{
	if (m_stateMgr)
	{
		m_stateMgr->terminate();
		safeDelete(m_stateMgr);
	}
}
/*
*/
void cGameStateManager::update(float elapsedtime)
{
	if (m_stateMgr)
		m_stateMgr->update(elapsedtime);
}
/*
*/
void cGameStateManager::setState(char const* stateName, float delay, bool flush)
{
	m_stateMgr->setState(stateName, delay, flush);
}
/*
*/
void cGameStateManager::changeState(char const* stateName, float delay, bool flush)
{
	m_stateMgr->changeState(stateName, delay, flush);
}