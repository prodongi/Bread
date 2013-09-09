
#include "BreadClassCreater.h"
#include "BreadShadowManager.h"
#include "BreadGui.h"
#include "BreadRenderer.h"
#include "BreadLuaManager.h"
#include "BreadSceneManager.h"
#include "BreadLightManager.h"
#include "BreadShadowManager.h"
#include "BreadPostProcessManager.h"
#include "BreadEnvironment.h"
#include "BreadMsgManager.h"
#include "BreadEntityFadeManager.h"
#include "BreadEntityManager.h"
#include "BreadAnimationActionManager.h"
#include "BreadBatchRenderManager.h"
#include "BreadDebugRenderManager.h"
#include "BreadAnimationEventManager.h"
#include "BreadAnimationManager.h"
#include "BreadFileSystem.h"
#include "BreadMeshCreater.h"
#include "BreadMoveProxyManager.h"

namespace Bread
{
	/*
	 *	
	 */
	cShadowManager* cClassCreater::newShadowMgr()						{	return new cShadowManager;				}
	cGui* cClassCreater::newGui()										{	return new cGui;						}
	cRenderer* cClassCreater::newRenderer()								{	return new cRenderer;					}
	cLuaManager* cClassCreater::newLuaMgr()								{	return new cLuaManager;					}
	cPostProcessManager* cClassCreater::newPostProcessMgr()				{	return new cPostProcessManager;			}
	cSceneManager* cClassCreater::newSceneMgr()							{	return new cSceneManager;				}
	cLightManager* cClassCreater::newLightMgr()							{	return new cLightManager;				}
	cEnvironment* cClassCreater::newEnvironment()						{	return new cEnvironment;				}
	cMsgManager* cClassCreater::newMsgMgr()								{	return new cMsgManager;					}
	cEntityFadeManager* cClassCreater::newEntityFadeMgr()				{	return new cEntityFadeManager;			}
	cEntityManager* cClassCreater::newEntityMgr()						{	return new cEntityManager;				}
	cAnimationActionManager* cClassCreater::newAnimationActionMgr()		{	return new cAnimationActionManager;		}
	cBatchRenderManager* cClassCreater::newBatchRenderMgr()				{	return new cBatchRenderManager;			}
	cDebugRenderManager* cClassCreater::newDebugRenderMgr()				{	return new cDebugRenderManager;			}
	cAnimationEventManager* cClassCreater::newAnimationEventMgr()		{	return new cAnimationEventManager;		}
	cAnimationManager* cClassCreater::newAnimationMgr()					{	return new cAnimationManager;			}
	cFileSystem* cClassCreater::newFileSystem()							{	return new cFileSystem;					}
	cMeshCreater* cClassCreater::newMeshCreater()						{	return new cMeshCreater;				}
	cMoveProxyManager* cClassCreater::newMoveProxyMgr()					{	return new cMoveProxyManager;			}
}