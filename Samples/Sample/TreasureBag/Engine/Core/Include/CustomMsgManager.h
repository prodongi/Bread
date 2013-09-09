
#ifndef _CustomMsgManager_h_
#define _CustomMsgManager_h_

/*
	CustomMsgManager.h
	Copyright (c) prodongi. All rights resserved.

	date : 2012-08-09
	purpose : 
*/

#include "BreadMsgManager.h"
#include "Definition.h"

class cCustomMsgManager : public cMsgManager
{
	declareCustomClassCreater(cCustomMsgManager)

public:
	virtual void initialize();

protected:
	virtual void initCallBackFunc();
};

#endif