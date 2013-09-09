
#include "BreadMsgCallBack.h"
#include "BreadMsgManager.h"
#include "BreadMsgType.h"

namespace Bread
{	
	/*
	*/
	cMsgManager::cMsgManager()
	{
	}
	/*
	*/
	cMsgManager::~cMsgManager()
	{
	}
	/*
	*/
	void cMsgManager::initialize()
	{
		initCallBack(MSG_END);
	}
	/*
	*/
	void cMsgManager::initCallBackFunc()
	{
		assert(m_callBack);
		
		m_callBack[MSG_CREATE_ENTITY]		= funcMsgCreateEntity;
		m_callBack[MSG_CREATE_AVATAR]		= funcMsgCreateAvatar;
		m_callBack[MSG_CREATE_DECAL]		= funcMsgCreateDecal;
		m_callBack[MSG_ADD_FADEIN_ENTITY]	= funcMsgAddFadeInEntity;
		m_callBack[MSG_SET_WEATHER]			= funcMsgSetWeather;
		m_callBack[MSG_MOVE]				= funcMsgMove;
	}
}