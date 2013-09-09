
#ifndef _BreadMoveProxyManager_h_
#define _BreadMoveProxyManager_h_

/*
	BreadMoveProxyManager.h
	Copyright (c) prodongi. All rights resserved.

	date : 2012-08-27
	purpose : 
*/

#include "BreadDeclaration.h"
#include "BreadBucketMap.h"
#include "BreadMoveProxy.h"

#define _getMoveProxyMgr()	Bread::cMoveProxyManager::getInstance()

namespace Bread
{
	/*
	*/
	class cMoveProxyManager : public cBucketMap<BUID, cMoveProxy>
	{
		declareInstance(cMoveProxyManager)
		declareClassCreater(cMoveProxyManager)

	public:
		virtual void update(float elapsedtime);
		virtual bool add(sMoveProxyInfo const& info);
		virtual int renderDebugInfo(int y);
		void renderPath();

	public:		
	};
}

#endif