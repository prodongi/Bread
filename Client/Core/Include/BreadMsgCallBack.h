
#ifndef _BreadMsgCallBack_h_
#define _BreadMsgCallBack_h_

namespace Bread
{
	struct sMsgInfo;

	void funcMsgCreateEntity(sMsgInfo* msg);	/// @brief MSG_CREATE_ENTITY
	void funcMsgCreateAvatar(sMsgInfo* msg);	/// @brief MSG_CREATE_AVATAR
	void funcMsgCreateDecal(sMsgInfo* msg);		/// @brief MSG_CREATE_DECAL
	void funcMsgAddFadeInEntity(sMsgInfo* msg);	/// @brief MSG_ADD_FADEIN_ENTITY
	void funcMsgSetWeather(sMsgInfo* msg);		/// @brief MSG_SET_WEATHER
	void funcMsgMove(sMsgInfo* msg);
}












#endif