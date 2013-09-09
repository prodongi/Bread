
#include <tchar.h>
#include "BreadApplication.h"
#include "BreadMemoryLeakHeader.h"
#include "BreadRenderOutputInfo.h"
#include "BreadExceptionHandler.h"
#include "BreadStringHelper.h"
#include "BreadGui.h"
#include "BreadUUIDManager.h"
#include "BreadInternString.h"
#include "BreadTimer.h"
#include "BreadProfile.h"
#include "BreadProfileManager.h"

namespace Bread
{
	cExceptionHandler exceptionHandler;

	static cApplication* application = NULL;

	LRESULT CALLBACK _wndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		assert(application && _T("application must be not NULL!"));
		return application->wndProc(hWnd, msg, wParam, lParam);
	}

	/*
	*/
	void mouseEventCallBack()
	{
		application->mouseEventCallBack();
	}

	/**
	 *	
	 */
	cApplication::cApplication() : m_quit(false), m_timer(NULL)
	{
		application = this;
	}
	/**
	 *	
	 */
	cApplication::~cApplication()
	{
	}
	/*
	*/
	void cApplication::initialize()
	{
		cMath::initRand();

		m_timer = new cTimer;
		m_timer->initialize();
	}
	/*
	 *	
	 */
	void cApplication::terminate()
	{
		_getCore()->finalize();
		_getCore()->release();
		_getStringHelper()->release();
		_getUUID()->release();
		_getInternString()->release();
		safeDelete(m_timer, true);
		cMath::clear();
	}
	/*
	 *	
	 */
	LRESULT CALLBACK cApplication::wndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		if (!_getCore()->wndProc(hWnd, msg, wParam, lParam))
			return 0;

		switch (msg)
		{
		case WM_DESTROY:	quit();		return 0;
		}
		return 1;
	}
	/*
	 *	
	 */
	void cApplication::setDebugLeakFlag()
	{
		setMemoryLeakDebugFlag();
		//_CrtSetBreakAlloc(21054);
	}
	/**
	 *	
	 */
	void cApplication::run(HINSTANCE hInstance)
	{
		initialize();
		initException();
		setDebugLeakFlag();	

		if (_getCore()->initialize(hInstance, _wndProc))
		{
			if (initApp())
			{
				MSG msg;
				while (!m_quit)
				{
					while (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
					{					
						TranslateMessage(&msg);
						DispatchMessage(&msg);
					}

					procedure();
				}
			}
		}

		terminate();
	}
	/**
	 *	
	 */
	bool cApplication::initApp()
	{
		if (!_getCore()->initPostProcessMgr())
			return false;
		// 카메라 추가

		return true;
	}
	/*
	 *	
	 */
	void cApplication::initException()
	{
		sExceptionMailInfo mailInfo;
		mailInfo.m_isSend = false;
		cExceptionHandler::setMailInfo(mailInfo);
	}
	/*
	 *	
	 */
	void cApplication::procedure()
	{
		PROFILE_RESET()

		updateTime();
		update(m_timer->getElapsedTime());
		render();
		mergeThread();
		fileWatching();

		m_fps.update();

		if (!_getEngineConfig()->getFullScreen())
			Sleep(1);
	}
	/*
	*/
	void cApplication::updateTime()
	{
		m_timer->update();
		//float curTime = (float)timeGetTime()/1000.0f;
		//m_elapsedtime = curTime - m_appTime;
		//m_appTime = curTime;
	}
	/**
	 *	
	 */
	void cApplication::update(float elapsedtime)
	{
		PROFILE(_T("update"));

		_getCore()->update(elapsedtime);
	}
	/*
	*/
	void cApplication::mergeThread()
	{
		PROFILE(_T("mergeThread"));

		_getCore()->mergeThread();
	}
	/*
	*/
	void cApplication::fileWatching()
	{
		PROFILE(_T("fileWatching"));

		_getCore()->fileWatching();
	}
	/**
	 *	
	 */
	void cApplication::render()
	{
		_getCore()->render(this);
	}
	/*
	 *	
	 */
	int cApplication::renderDebugInfo(int y)
	{
		TCHAR str[32];

		// face
		_stprintf(str, _T("face num: %d"), cRenderOutputInfo::getFaceNumber());
		_getFontMgr()->drawD3DText(10, y+=16, str);
		// vertex num
		_stprintf(str, _T("vertex num: %d"), cRenderOutputInfo::getVertexNumber());
		_getFontMgr()->drawD3DText(10, y+=16, str);
		// draw primitive
		_stprintf(str, _T("dp num: %d"), cRenderOutputInfo::getDpNumber());
		_getFontMgr()->drawD3DText(10, y+=16, str);

		return y;
	}
	/*
	*/
	void cApplication::renderFps()
	{
		TCHAR str[32];
		_stprintf(str, _T("fps : %.2f"), m_fps.getFps());
		_getFontMgr()->drawD3DText(10, 5, str);
	}
	/*
	*/
	void cApplication::mouseEventCallBack()
	{
	}
}




















