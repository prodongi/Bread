
#ifndef _BreadCore_h_
#define _BreadCore_h_

#include <Windows.h>
#include "BreadSingleton.h"
#include "BreadRenderer.h"
#include "BreadLuaManager.h"
#include "BreadDeclaration.h"
#include "BreadEngineConfig.h"
#include "BreadClassCreater.h"
#include "BreadScreenShot.h"
#include "BreadDirectoryInfo.h"
#include "BreadMsgManager.h"


namespace Bread
{
	class cGui;
	class cApplication;
	class cPostProcessManager;
	class cFileSystem;
	class cFileWatcher;
	class cDebugRenderManager;
	class cBatchRenderManager;

	/**
	 * @class cCore	
	 */
	class cCore
	{
		declareSingleton(cCore)

	public:
		~cCore();
		bool initialize(HINSTANCE hInstance, WNDPROC wndProc);
		void finalize();
		bool initPostProcessMgr();
		void render(cApplication* application);
		void update(float elapsedtime);
		void mergeThread();
		void fileWatching();
		cGui* getGui() const				{	return m_gui;		}
		cRenderer* getRenderer() const		{	return m_renderer;	}
		cLuaManager* getLuaMgr() const		{	return m_luaMgr;	}
		cEngineConfig const* getEngineConfig() const	{	return &m_config;	}
		cDirectoryInfo const* getDirectoryInfo() const	{	return &m_directoryInfo;	}
		cPostProcessManager* getPostProcessMgr() const	{	return m_postProcessMgr;	}
		cMsgManager* getMsgMgr() const		{	return m_msgMgr;	}

		/// @param initialize보다 먼저 세팅되어야 합니다.
		propertyFunc(cClassCreater*, ClassCreater, m_classCreater)
		propertyConstFunc(bool, IsRender3D, m_isRender3D)
		propertyConstFunc(bool, IsRender2D, m_isRender2D)

	private:
		bool beginScene(bool clear);
		void endScene();
		void render2D(cApplication* application);
		void render3D(cApplication* application);
		void render3DScene(cApplication* application);
		void beforeRender3D(cApplication* application);
		void afterRender3D(cApplication* application);
		void beforeRender2D(cApplication* application);
		void afterRender2D(cApplication* application);
		int  renderDebugInfo(cApplication* application);
		void renderProfile();
		void present();
		LRESULT CALLBACK wndProc(HWND, UINT, WPARAM, LPARAM);

	private:
		cGui*		 m_gui;
		cRenderer*	 m_renderer;
		cLuaManager* m_luaMgr;
		cPostProcessManager* m_postProcessMgr;
		cEngineConfig m_config;
		cClassCreater* m_classCreater;
		cMsgManager* m_msgMgr;
		cFileSystem* m_fileSystem;
		cFileWatcher* m_fileWatcher;
		bool m_isRender3D;
		bool m_isRender2D;
		cScreenShot m_screenShot;
		cDirectoryInfo m_directoryInfo;
		cDebugRenderManager* m_debugRenderMgr;
		cBatchRenderManager* m_batchRenderMgr;

		friend class cApplication;
	};
}










#endif

















