
#ifndef _BreadGuiManager_h_
#define _BreadGuiManager_h_

#include "BreadD3DHeader.h"
#include "BreadWindow.h"
#include "BreadInputSystem.h"
#include "BreadFontManager.h"

namespace Bread
{
	struct sMsgInfo;

	/**
	* \ingroup GUI
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
	* \date 2009-08-20
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
	* \todo 모든 윈도우의 좌표는 절대좌표이다.
	*
	* \bug 
	*
	*/
	class cGui
	{
		declareClassCreater(cGui)

	public:
		virtual bool initialize(LPDIRECT3DDEVICE9 device, HINSTANCE hInstance, HWND hWnd);
		virtual void clear();
		virtual void update(float elapsedtime);
		virtual void render();
		virtual int renderDebugInfo(int y);
		virtual void enterFocus(cWindow* win);
		virtual void leaveFocus();
		virtual void initConsole() {}
		virtual void msgProc(sMsgInfo* msg);
		template <class T>
		bool load(TCHAR const* filename)
		{
			char _filename[256];
			w2m(filename, _filename, 256)
			cWindowXmlSerialize<T> serialize;
			if (!serialize.load(_filename))
				return false;
			T* root = serialize.getRoot();
			root->createVB();
			m_windowList.add(root);
			return true;
		}

		LRESULT CALLBACK wndProc(HWND, UINT, WPARAM, LPARAM);

		LPDIRECT3DDEVICE9 getD3DDevice() const	{	return m_D3DDevice;	}
		cInputSystem* getInputSystem() const	{	return m_input;		}
		cFontManager* getFontMgr() const		{	return m_fontMgr;	}
		propertyFunc(cWindow*, OverWindow, m_overWindow)

	protected:
		void clearOverWindow()	{	m_overWindow = NULL;	}

	protected:
		cWindowList<cWindow> m_windowList;
		cWindow* m_focusWindow;
		cWindow* m_overWindow;
	private:
		LPDIRECT3DDEVICE9	m_D3DDevice;
		cInputSystem*		m_input;
		cFontManager*		m_fontMgr;
	};

}



#endif