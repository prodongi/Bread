
#include "BreadCore.h"
#include "BreadDebug.h"
#include "BreadGui.h"
#include "BreadRenderOutputInfo.h"
#include "BreadApplication.h"
#include "BreadSkyBox.h"
#include "BreadFileSystem.h"
#include "BreadFileWatcher.h"
#include "BreadDebugRenderManager.h"
#include "BreadPostProcessManager.h"
#include "BreadEnvironment.h"
#include "BreadProfileManager.h"
#include "BreadProfile.h"
#include "BreadBatchRenderManager.h"

namespace Bread
{
	implementSingleton(cCore)

	void profileRenderCallBack(int x, int y, TCHAR const* str)
	{
		_getFontMgr()->drawD3DText(x, y, str);
	}
	
	/**
	*	
	*/
	cCore::cCore() : m_gui(NULL), 
					 m_renderer(NULL),
					 m_luaMgr(NULL),
					 m_postProcessMgr(NULL),
					 m_classCreater(NULL),
					 m_msgMgr(NULL),
					 m_batchRenderMgr(NULL),
					 m_fileSystem(NULL),
					 m_fileWatcher(NULL),
					 m_isRender3D(true),
					 m_isRender2D(true)
	{
		cProfileNode::setRenderCallBack(&profileRenderCallBack);
	}
	/**
	 *	
	 */
	cCore::~cCore()
	{
	}
	/**
	 *	
	 */
	bool cCore::initialize(HINSTANCE hInstance, WNDPROC wndProc)
	{	
		assert(_getClassCreater() && _T("_getClassCreater() is NULL"));
		//
		m_fileSystem = _getClassCreater()->newFileSystem();
		m_fileSystem->initialize();
		//
		m_config.load(_T("BaseData\\engine.cfg"));
		//
		m_luaMgr = _getClassCreater()->newLuaMgr();
		m_luaMgr->init();
		//
		m_renderer = _getClassCreater()->newRenderer();
		m_renderer->initialize(hInstance, wndProc, &m_config);
		//
		m_gui = _getClassCreater()->newGui();
		m_gui->initialize(m_renderer->getD3DSystem()->getD3DDevice(), hInstance, m_renderer->getD3DSystem()->getHwnd());
		_getInputSystem()->setMouseEventCallBack(&mouseEventCallBack);
		//
		m_postProcessMgr = _getClassCreater()->newPostProcessMgr();
		//
		m_msgMgr = _getClassCreater()->newMsgMgr();
		m_msgMgr->initialize();
		//
		m_batchRenderMgr = _getClassCreater()->newBatchRenderMgr();
		m_batchRenderMgr->initialize();
		//
		m_screenShot.initialize();
		//
		m_debugRenderMgr = _getClassCreater()->newDebugRenderMgr();
		m_debugRenderMgr->initialize();
		//
		m_directoryInfo.initialize();
		//
		m_fileWatcher = new cFileWatcher;
		m_fileWatcher->initialize();

		return true;
	}
	/*
	 *	
	 */
	void cCore::finalize()
	{
		safeDelete(m_gui);
		safeDelete(m_renderer, true);
		safeDelete(m_luaMgr);
		safeDelete(m_classCreater);
		safeDelete(m_msgMgr);
		safeDelete(m_batchRenderMgr, true);
		safeDelete(m_postProcessMgr, true);
		safeDelete(m_fileSystem, true);
		safeDelete(m_fileWatcher);
		safeDelete(m_debugRenderMgr, true);		
	}
	/*
	 *	
	 */
	bool cCore::initPostProcessMgr()
	{
		if (m_postProcessMgr)
			return m_postProcessMgr->initialize((uint)m_config.getWidth(), (uint)m_config.getHeight());
		return false;
	}
	/*
	 *	
	 */
	bool cCore::beginScene(bool clear)
	{
		if (clear)
			cD3DSystem::getD3DDevice()->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, _getD3DSystem()->getClearColor(), 1.0f, 0L);
		return SUCCEEDED(cD3DSystem::getD3DDevice()->BeginScene());
	}
	/*
	 *	
	 */
	void cCore::endScene()
	{
		cD3DSystem::getD3DDevice()->EndScene();
	}
	/*
	 *	
	 */
	void cCore::present()
	{
		cD3DSystem::getD3DDevice()->Present(NULL, NULL, NULL, NULL);
	}
	/*
	 *	
	 */
	void cCore::render(cApplication* application)
	{
		{
			PROFILE(_T("render 3D"));
			render3D(application);
		}
		{
			PROFILE(_T("render 2D"));
			render2D(application);
		}
		renderProfile();
		present();
	}
	/*
	 *	
	 */
	void cCore::render3D(cApplication* application)
	{
		bool clearScene = true;
		if (!beginScene(clearScene))
			return ;

		if (m_postProcessMgr && m_postProcessMgr->is())
		{
			m_postProcessMgr->beginRttScene(true);
			render3DScene(application);
			m_postProcessMgr->endRttScene();

			m_postProcessMgr->renderHelper();
		}
		else
		{
			render3DScene(application);
		}

		m_debugRenderMgr->render3D();

		endScene();
	}
	/*
	*/
	void cCore::render3DScene(cApplication* application)
	{
		beforeRender3D(application);
		if (getIsRender3D())
		{
			m_renderer->render();
			m_batchRenderMgr->render3D();
		}
		afterRender3D(application);
	}
	/*
	 *	
	 */
	void cCore::render2D(cApplication* application)
	{
		if (!beginScene(false))
			return ;

		beforeRender2D(application);
		if (getIsRender2D())
		{
			if (m_postProcessMgr && m_postProcessMgr->is())
			{
				m_postProcessMgr->render2D();
				m_postProcessMgr->renderDebugScreen();
			}
			m_gui->render();
			m_batchRenderMgr->render2D();
		}
		afterRender2D(application);

		if (_getIsRender(RENDER_DEBUG_INFO))
			renderDebugInfo(application);

		m_debugRenderMgr->render2D();

		if (_getIsRender(RENDER_FPS))
			application->renderFps();

		endScene();
	}
	/*
	*/
	void cCore::renderProfile()
	{
		if (!_getIsRender(RENDER_PROFILE))
			return ;

		if (!beginScene(false))
			return ;

		int x = _getIsRenderValue1(RENDER_PROFILE);
		int y = _getIsRenderValue2(RENDER_PROFILE);
		PROFILE_RENDER(x, y);

		endScene();
	}
	/*
	 *	
	 */
	void cCore::beforeRender3D(cApplication* application)
	{
		application->beforeRender3D();
		m_renderer->beforeRender3D();
	}
	/*
	 *	
	 */
	void cCore::afterRender3D(cApplication* application)
	{
		application->afterRender3D();
		m_renderer->afterRender3D();
	}
	/*
	 *	
	 */
	void cCore::beforeRender2D(cApplication* application)
	{
		application->beforeRender2D();
		m_renderer->beforeRender2D();
	}
	/*
	 *	
	 */
	void cCore::afterRender2D(cApplication* application)
	{
		application->afterRender2D();
		m_renderer->afterRender2D();
	}
	/*
	 *	
	 */
	int cCore::renderDebugInfo(cApplication* application)
	{
		int y;
		y = application->renderDebugInfo(16);
		y = m_renderer->renderDebugInfo(y);
		y = m_postProcessMgr->renderDebugInfo(y);
		y = m_gui->renderDebugInfo(y);
		return y;
	}
	/*
	 *	
	 */
	void cCore::update(float elapsedtime)
	{
		{
			PROFILE(_T("clear list"));
			m_debugRenderMgr->clearList();
			m_batchRenderMgr->clearList();
		}
		{
			PROFILE(_T("batch render mgr"));
			m_batchRenderMgr->update(elapsedtime);
		}
		{
			PROFILE(_T("msgMgr"));
			m_msgMgr->update(elapsedtime);
		}
		{
			PROFILE(_T("gui"));
			m_gui->update(elapsedtime);
		}
		{
			PROFILE(_T("renderer"));
			m_renderer->update(elapsedtime);
		}
		{
			PROFILE(_T("postProcessMgr"));
			m_postProcessMgr->update(elapsedtime);
		}
		{
			PROFILE(_T("screenShot"));
			m_screenShot.update();
		}
	}
	/*
	*/
	void cCore::mergeThread()
	{
		m_renderer->mergeThread();
	}
	/*
	*/
	void cCore::fileWatching()
	{
		m_fileWatcher->update();
	}
	/*
	 *	
	 */
	LRESULT CALLBACK cCore::wndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		int result = 1;
		if (m_gui)
			result = (result) ? m_gui->wndProc(hWnd, msg, wParam, lParam) : result;
		if (m_renderer)
			result = (result) ? m_renderer->wndProc(hWnd, msg, wParam, lParam) : result;

		return result;
	}
}







