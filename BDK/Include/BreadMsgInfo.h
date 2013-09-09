
#ifndef _BreadMsgInfo_h_
#define _BreadMsgInfo_h_

#include "BreadDefinition.h"

namespace Bread
{
#pragma pack(push, 1)

	/*
		@date 2010.09.29
		@auth prodongi
		@desc 
		@todo 
	*/
	struct sMsgInfo
	{
		enum {	SUCCESS, FAIL, NEXT	};

		sMsgInfo() : m_type(0), m_result(0) {}
		sMsgInfo(uint type) : m_type(type), m_result(0) {}
		virtual ~sMsgInfo() {}

		uint m_type; 
		uint m_result;
	};

#pragma pack(pop)
}



#endif