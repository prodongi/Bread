
/*
	CustomMsgManager.cpp
	Copyright (c) prodongi. All rights resserved.
*/

#include "stdafx.h"
#include "CustomMsgManager.h"
#include "MsgCallBack.h"
#include "MsgType.h"

/*
*/
cCustomMsgManager::cCustomMsgManager()
{
}
/*
*/
cCustomMsgManager::~cCustomMsgManager()
{
}
/*
*/
void cCustomMsgManager::initialize()
{
	initCallBack(MSG_CUSTOM_END);
}
/*
*/
void cCustomMsgManager::initCallBackFunc()
{
	cMsgManager::initCallBackFunc();

	m_callBack[MSG_GAME_STATE] = funcMsgGameState;
}