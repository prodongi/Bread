
#ifndef _BreadApplication_h_
#define _BreadApplication_h_

#include <Windows.h>
#include "BreadFps.h"
#include "BreadCore.h"

namespace Bread
{
	class cTimer;

	/**
	 * @class cApplication	
	 */
	class cApplication
	{
	public:
		cApplication();
		virtual ~cApplication();
		virtual LRESULT CALLBACK wndProc(HWND, UINT, WPARAM, LPARAM);
		virtual void mouseEventCallBack();
        void run(HINSTANCE hInstance);

		float getFps() const	{	return m_fps.getFps();	}
		void quit()				{	m_quit = true;			}

	protected:
		virtual void initialize();
		virtual bool initApp();
		virtual void initException();
		virtual void update(float elapsedtime);
		virtual void mergeThread();
		/// @brief 파일 감시자, 개발 할 때 파일에 변화가 있는지 체크해서 실시간 로딩에 쓰임
		virtual void fileWatching();
		virtual void beforeRender3D() {};
		virtual void afterRender3D() {};
		virtual void beforeRender2D() {};
		virtual void afterRender2D() {};
		virtual int  renderDebugInfo(int y);
		virtual void terminate();
		virtual void setDebugLeakFlag();
		virtual void render();
		void renderFps();
	private:
		void procedure();
		void updateTime();

	protected:
		bool m_quit;
		cFps m_fps;
		cTimer* m_timer;

	friend class cCore;
	};

	/*
	*/
	void mouseEventCallBack();
}







#endif










