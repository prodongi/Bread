
/*
	BreadMoveProxyManager.cpp
	Copyright (c) prodongi. All rights resserved.
*/

#include "BreadMoveProxyManager.h"

namespace Bread
{
	implementInstance(cMoveProxyManager)

	/*
	*/
	cMoveProxyManager::cMoveProxyManager()
	{
		assignInstance(this)
	}
	/*
	*/
	cMoveProxyManager::~cMoveProxyManager()
	{
		assignInstance(NULL)
	}
	/*
	*/
	void cMoveProxyManager::update(float elapsedtime)
	{
		for (begin(); !isEnd(); next())
		{
			cMoveProxy* moveProxy = &getIterData();
			moveProxy->update(elapsedtime);
			if (moveProxy->isEnd())
			{
				if (removeNext())
					continue;
				else
					break;
			}
		}
	}
	/*
	*/
	bool cMoveProxyManager::add(sMoveProxyInfo const& info)
	{
		bool ret;
		if (is(info.m_buid))
		{
			cMoveProxy* moveProxy = &get(info.m_buid);
			ret = moveProxy->set(info);
		}
		else
		{
			cMoveProxy moveProxy;
			ret = moveProxy.set(info);
			if (ret)
				insert(info.m_buid, moveProxy);
		}
		return ret;
	}
	/*
	*/
	void cMoveProxyManager::renderPath()
	{
		for (begin(); !isEnd(); next())
		{
			getIterData().renderPath();
		}
	}
	/*
	*/
	int cMoveProxyManager::renderDebugInfo(int y)
	{
		return y;
	}
}