
#ifndef _BreadMsgManager_h_
#define _BreadMsgManager_h_

#include "BreadDeclaration.h"
#include "BreadMsgManagerBase.h"
#include "BreadMsgInfoList.h"

namespace Bread
{
	/*
		@date 2010.00.00
		@auth prodongi
		@desc 
		@todo 
	*/
	class cMsgManager : public cMsgManagerBase
	{
		declareClassCreater(cMsgManager)

	public:
		virtual void initialize();

	protected:
		virtual void initCallBackFunc();
	};
}



#endif