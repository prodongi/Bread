
#ifndef _BreadClientDefinition_h_
#define _BreadClientDefinition_h_

#include "BreadStdHeader.h"
#include "BreadDefinition.h"

namespace Bread
{
	/*
	*	
	*/
	#define _getCore()				Bread::cCore::getSingleton()
	#define _getRenderer()			_getCore()->getRenderer()
	#define _getGui()				_getCore()->getGui()
	#define _getMsgMgr()			_getCore()->getMsgMgr()
	#define _getLuaMgr()			_getCore()->getLuaMgr()
	#define _getEngineConfig()		_getCore()->getEngineConfig()
	#define _getClassCreater()		_getCore()->getClassCreater()
	#define _getDirectoryInfo()		_getCore()->getDirectoryInfo()
	#define _getPostProcessMgr()	_getCore()->getPostProcessMgr()
	#define _getClassCreator()		_getCore()->getClassCreater()
	#define _getMsgMgr()			_getCore()->getMsgMgr()
	#define _getFontMgr()			_getGui()->getFontMgr()
	#define _getInputSystem()		_getGui()->getInputSystem()
	#define _getD3DSystem()			_getRenderer()->getD3DSystem()
	#define _getSceneMgr()			_getRenderer()->getSceneMgr()
	#define _getRenderPropertyMgr()	_getRenderer()->getRenderPropertyMgr()
	#define _getShadowMgr()			_getRenderer()->getShadowMgr()
	#define _getMainSceneOctree()	_getSceneMgr()->getMainOctree()
	#define _getPick()				_getSceneMgr()->getPick()
	#define _getTerrain()			_getSceneMgr()->getTerrain()
	#define _getEntityFadeMgr()		_getSceneMgr()->getEntityFadeMgr()
	#define _getStringHelper()		Bread::cStringHelper::getSingleton()
	#define _getUUID()				Bread::cUUIDManager::getSingleton()
	#define _getInternString()		Bread::cInternString::getSingleton()
	/*
	*/
	#define _renderPropBegin()		_getRenderPropertyMgr()->begin()
	#define _renderPropSet(a, b)	_getRenderPropertyMgr()->setProperty(a, b)
	#define _renderPropEnd()		_getRenderPropertyMgr()->end()
	/*
	*/
	enum SHARE_TYPE
	{
		ST_NONE = 0,
		ST_VB = (1 << 0),
		ST_IB = (1 << 1),
	};
}

#endif






