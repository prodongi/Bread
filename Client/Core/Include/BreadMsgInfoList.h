
#ifndef _BreadMsgInfoList_h_
#define _BreadMsgInfoList_h_

#include "BreadVector3.h"
#include "BreadMsgInfo.h"
#include "BreadMsgType.h"

namespace Bread
{
#pragma pack(push, 1)

	/*
	*/
	struct sMsgCreateEntity : public sMsgInfo
	{
		sMsgCreateEntity() : sMsgInfo(MSG_CREATE_ENTITY) {}

		std_string m_filename;
		BUID m_buid;
		sVector3 m_pos;
		float m_radian;
		uint m_loadingType;
		uint m_shareType;
	};
	/*
	*/
	struct sMsgCreateAvatar : public sMsgInfo
	{
		sMsgCreateAvatar() : sMsgInfo(MSG_CREATE_AVATAR) {}

		std_string m_filename;
		BUID m_buid;
		sVector3 m_pos;
		float m_radian;
		uint m_loadingType;
		uint m_shareType;
	};
	/*
	*/
	struct sMsgCreateDecal : public sMsgInfo
	{
		sMsgCreateDecal() : sMsgInfo(MSG_CREATE_DECAL) {}

		std_string m_filename;
		BUID m_buid;
		sVector3 m_pos;
		float m_radian;
		uint m_loadingType;
		uint m_shareType;
	};
	/*
	*/
	struct sMsgAddFadeInEntity : public sMsgInfo
	{
		sMsgAddFadeInEntity() : sMsgInfo(MSG_ADD_FADEIN_ENTITY) {}

		BUID m_buid;
	};
	/*
	*/
	struct sMsgSetWeather : public sMsgInfo
	{
		sMsgSetWeather() : sMsgInfo(MSG_SET_WEATHER) {}
		sMsgSetWeather(int type) : sMsgInfo(MSG_SET_WEATHER), m_weatherType(type) {}

		int m_weatherType;
	};
	/*
	*/
	struct sMsgMove : public sMsgInfo
	{
		sMsgMove() : sMsgInfo(MSG_MOVE) {}
		~sMsgMove() {	m_path.clear();	}

		BUID m_buid;
		std::vector<sVector3> m_path;
		uint m_moveType;
		float m_speed;
	};

#pragma pack(pop)
}



#endif