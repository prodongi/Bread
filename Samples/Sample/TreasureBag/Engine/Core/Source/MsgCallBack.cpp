
/*
	MsgCallBack.cpp
	Copyright (c) prodongi. All rights resserved.
*/

#include "stdafx.h"
#include "GameStateManager.h"
#include "MsgCallBack.h"
#include "MsgInfoList.h"
#include "MsgType.h"

void funcMsgGameState(sMsgInfo* msg)
{
	sMsgGameState* _msg = dynamicCast<sMsgGameState*>(msg);
	if (!_msg)
		return ;

	if ("set" == _msg->m_cmd)
		_getGameStateMgr()->setState(_msg->m_state.c_str(), _msg->m_delay, _msg->m_flush);
	else if ("change" == _msg->m_cmd)
		_getGameStateMgr()->changeState(_msg->m_state.c_str(), _msg->m_delay, _msg->m_flush);
}