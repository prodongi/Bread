
#ifndef _MsgInfoList_h_
#define _MsgInfoList_h_

/*
	MsgType.h
	Copyright (c) prodongi. All rights resserved.

	date : 2012-08-09
	purpose : 
*/

#include "BreadMsgInfoList.h"
#include "MsgType.h"

#pragma pack(push, 1)

/*
*/
struct sMsgGameState : public sMsgInfo
{
	sMsgGameState() : sMsgInfo(MSG_GAME_STATE), m_delay(0.0f), m_flush(false) {}

	// cmd(set, change)
	std::string m_cmd;
	std::string m_state;
	float m_delay;
	bool m_flush;
};

#pragma pack(pop)

#endif
