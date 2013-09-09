
#include "BreadEntityFadeManager.h"
#include "BreadSceneManager.h"
#include "BreadEnvironment.h"
#include "BreadMsgCallBack.h"
#include "BreadMsgInfoList.h"
#include "BreadRenderer.h"
#include "BreadDebug.h"
#include "BreadGui.h"
#include "BreadMoveProxyManager.h"
#include "BreadEntityDecal.h"

namespace Bread
{
	/*
	*/
	void funcMsgCreateEntity(sMsgInfo* msg)
	{
		sMsgCreateEntity* _msg = dynamicCast<sMsgCreateEntity*>(msg);
		if (!_msg)
			return ;

		_getSceneMgr()->createEntity<cEntityModel>(_msg->m_filename, _msg->m_buid, _msg->m_pos, _msg->m_radian, _msg->m_loadingType, _msg->m_shareType);
	}
	/*
	*/
	void funcMsgCreateAvatar(sMsgInfo* msg)
	{
		sMsgCreateAvatar* _msg = dynamicCast<sMsgCreateAvatar*>(msg);
		if (!_msg)
			return ;

		_getSceneMgr()->createAvatar<cEntityAvatar>(_msg->m_filename, _msg->m_buid, _msg->m_pos, _msg->m_radian, _msg->m_loadingType, _msg->m_shareType);
	}
	/*
	*/
	void funcMsgCreateDecal(sMsgInfo* msg)
	{
		sMsgCreateDecal* _msg = dynamicCast<sMsgCreateDecal*>(msg);
		if (!_msg)
			return ;

		_getSceneMgr()->createEntity<cEntityDecal>(_msg->m_filename, _msg->m_buid, _msg->m_pos, _msg->m_radian, _msg->m_loadingType, _msg->m_shareType);
	}
	/*
	*/
	void funcMsgAddFadeInEntity(sMsgInfo* msg)
	{
		sMsgAddFadeInEntity* _msg = dynamicCast<sMsgAddFadeInEntity*>(msg);
		if (!_msg)
			return ;

		_getEntityFadeMgr()->addFadeIn(_msg->m_buid);
	}
	/*
	*/
	void funcMsgSetWeather(sMsgInfo* msg)
	{
		sMsgSetWeather* _msg = dynamicCast<sMsgSetWeather*>(msg);
		if (!_msg)
			return ;

		_getEnv()->setWeatherType(_msg->m_weatherType);
	}
	/*
	*/
	void funcMsgMove(sMsgInfo* msg)
	{
		sMsgMove* _msg = dynamicCast<sMsgMove*>(msg);
		if (!_msg)
			return ;

		sMoveProxyInfo moveProxyInfo;
		moveProxyInfo.m_buid = _msg->m_buid;
		moveProxyInfo.m_path = _msg->m_path;
		moveProxyInfo.m_moveType = _msg->m_moveType;
		moveProxyInfo.m_speed = _msg->m_speed;
		_getMoveProxyMgr()->add(moveProxyInfo);
	}
}