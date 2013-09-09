
#ifndef _BreadMsgManagerBase_h_
#define _BreadMsgManagerBase_h_

#include "BreadInline.h"
#include "BreadBucketVector.h"
#include "BreadMsgInfo.h"

namespace Bread
{
	typedef void (*MsgCallBack)(sMsgInfo* msg);
	/*
		@date 2010.00.00
		@auth prodongi
		@desc 
		@todo 
	*/
	class cMsgManagerBase
	{
	protected:
		cMsgManagerBase();
	public:
		~cMsgManagerBase();

	public:
		typedef cBucketVector<sMsgInfo*> bucket_msg;

	public:
		virtual void initialize();
		virtual void update(float elapsedtime);
		virtual void addBucket(sMsgInfo* msg);	/// @brief bucket에 넣어서 update에서 처리
		virtual void execute(sMsgInfo& msg);	/// @brief 즉시 처리

	protected:
		virtual void initCallBack(uint callBackNum);
		virtual void initCallBackFunc() = 0;
	
	protected:
		bucket_msg* m_msgList;
		uint m_callBackNum;
		MsgCallBack* m_callBack;
	};
}

#endif