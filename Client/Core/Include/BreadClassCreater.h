
#ifndef _BreadClassCreater_h_
#define _BreadClassCreater_h_


namespace Bread
{
	class cShadowManager;
	class cGui;
	class cRenderer;
	class cLuaManager;
	class cSceneManager;
	class cLightManager;
	class cShadowManager;
	class cPostProcessManager;
	class cEnvironment;
	class cMsgManager;
	class cEntityFadeManager;
	class cEntityManager;
	class cAnimationActionManager;
	class cBatchRenderManager;
	class cDebugRenderManager;
	class cAnimationEventManager;
	class cAnimationManager;
	class cFileSystem;
	class cMeshCreater;
	class cMoveProxyManager;
	
	/**
	 * \ingroup Core
	 *
	 *
	 * \par requirements
	 * win98 or later\n
	 * win2k or later\n
	 * MFC\n
	 *
	 * \version 1.0
	 * first version
	 *
	 * \date 2010-03-12
	 *
	 * \author Prodongi
	 *
	 * \par license
	 * This code is absolutely free to use and modify. The code is provided "as is" with
	 * no expressed or implied warranty. The author accepts no liability if it causes
	 * any damage to your computer, causes your pet to fall ill, increases baldness
	 * or makes your car start emitting strange noises when you start it up.
	 * This code has no bugs, just undocumented features!
	 * 
	 * \todo 엔진 외부에서 쉽게 클래스 상속관계를 설정하기 위한 클래스
	 *
	 * \bug 
	 *
	 */
	class cClassCreater
	{
	public:
		cClassCreater() {}
		virtual ~cClassCreater() {}
		virtual cShadowManager* newShadowMgr();
		virtual cGui* newGui();
		virtual cRenderer* newRenderer();
		virtual cLuaManager* newLuaMgr();
		virtual cPostProcessManager* newPostProcessMgr();
		virtual cSceneManager* newSceneMgr();
		virtual cLightManager* newLightMgr();
		virtual cEnvironment* newEnvironment();
		virtual cMsgManager* newMsgMgr();
		virtual cEntityFadeManager* newEntityFadeMgr();
		virtual cEntityManager* newEntityMgr();
		virtual cAnimationActionManager* newAnimationActionMgr();
		virtual cBatchRenderManager* newBatchRenderMgr();
		virtual cDebugRenderManager* newDebugRenderMgr();
		virtual cAnimationEventManager* newAnimationEventMgr();
		virtual cAnimationManager* newAnimationMgr();
		virtual cFileSystem* newFileSystem();
		virtual cMeshCreater* newMeshCreater();
		virtual cMoveProxyManager* newMoveProxyMgr();
	};
}












#endif