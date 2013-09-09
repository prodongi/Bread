
#ifndef _BreadcInputSystem_h_
#define _BreadcInputSystem_h_

#include <Windows.h>
#include <direct.h>
#include <dinput.h>

#include "BreadDeclaration.h"
#include "BreadInputTypedef.h"
#include "BreadInputKey.h"
#include "BreadInputLocaleInfo.h"

namespace Bread
{
	/* the size of the keyboard buffer */
	#define kisBufferSize	256

#ifndef WM_MOUSEWHEEL
	#define WM_MOUSEWHEEL 0x020A
#endif

	typedef void (*callBackMouseEvent)();

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
	 * \date 2009-08-31
	 *
	 * \author Seegert
	 *
	 * \par license
	 * This code is absolutely free to use and modify. The code is provided "as is" with
	 * no expressed or implied warranty. The author accepts no liability if it causes
	 * any damage to your computer, causes your pet to fall ill, increases baldness
	 * or makes your car start emitting strange noises when you start it up.
	 * This code has no bugs, just undocumented features!
	 * 
	 * \todo gpg3권 1.13에서 갖고 왔습니다.
	 *
	 * \bug 
	 *
	 */
	class cInputSystem
	{
	public:
		cInputSystem(void);
		~cInputSystem(void);

		static cInputTypedef::iSModifierState		getModifierState(unsigned char inScanCode);

		inline const KeyListType&	getJustPressedKeys(void)	{ return this->m_justPressedKeys;	}
		inline const KeyListType&	getPressedKeys(void)		{ return this->m_pressedKeys;		}
		inline const KeyListType&	getJustUnPressedKeys(void)	{ return this->m_justUnPressedKeys; }
		bool isLButtonDown() const	{	return m_mouseButtonDown[MOUSE_LB];	}
		bool isMButtonDown() const	{	return m_mouseButtonDown[MOUSE_MB];	}
		bool isRButtonDown() const	{	return m_mouseButtonDown[MOUSE_RB];	}
		bool isJustLButtonDown() const
		{
			if (!m_preMouseButtonDown[MOUSE_LB] && m_mouseButtonDown[MOUSE_LB])	
				return true;
			return false;
		}
		bool isJustMButtonDown() const
		{
			if (!m_preMouseButtonDown[MOUSE_MB] && m_mouseButtonDown[MOUSE_MB])	
				return true;
			return false;
		}
		bool isJustRButtonDown() const
		{
			if (!m_preMouseButtonDown[MOUSE_RB] && m_mouseButtonDown[MOUSE_RB])	
				return true;
			return false;
		}
		bool isLButtonUp() const	
		{
			if (m_preMouseButtonDown[MOUSE_LB] && !m_mouseButtonDown[MOUSE_LB])	return true;
			return false;
		}
		bool isMButtonUp() const	
		{	
			if (m_preMouseButtonDown[MOUSE_MB] && !m_mouseButtonDown[MOUSE_MB])	return true;
			return false;	
		}
		bool isRButtonUp() const	
		{	
			if (m_preMouseButtonDown[MOUSE_RB] && !m_mouseButtonDown[MOUSE_RB])	return true;
			return false;	
		}

		bool isJustPressedKey(unsigned char key, cInputTypedef::iSModifierState modifierState = cInputTypedef::kismsNone);
		bool isPressedKey(unsigned char key, cInputTypedef::iSModifierState modifierState = cInputTypedef::kismsNone);
		bool isJustUnPressedKey(unsigned char key, cInputTypedef::iSModifierState modifierState = cInputTypedef::kismsNone);

		bool initialize(HINSTANCE instance, HWND hwnd);
		bool update();
		int renderDebugInfo(int y);
		void setMouseEventCallBack(callBackMouseEvent func);

		LRESULT CALLBACK wndProc(HWND, UINT, WPARAM, LPARAM);

		propertyFunc(bool, SwapMouseButton, m_swapMouseButton)
		propertyFunc(int, MouseX, m_mouseX)
		propertyFunc(int, MouseY, m_mouseY)
		propertyFunc(bool, MouseMove, m_mouseMove)
		propertyFunc(bool, MouseWheel, m_mouseWheel)
		int getMouseDx() const		{	return m_mouseDx;		}
		int getMouseDy() const		{	return m_mouseDy;		}
		int getWheelDelta() const	{	return m_wheelDelta;	}
		WORD getCodePage() const	{	return m_localeInfo.getCodePage();	}

	private:
		void createKeyboard();
		void createMouse();
		bool updateKeyboard();
		bool updateMouse();

	private:							
		KeyListType					m_justPressedKeys;		/* vector of keys just pressed */
		KeyListType					m_pressedKeys;			/* vector of keys pressed */
		KeyListType					m_justUnPressedKeys;	/* vector of keys just unpressed */

		HINSTANCE					m_hinstance;			/* the instance of this application */
		HWND						m_hwnd;					/* the handle to the window */

		IDirectInput*				m_directInput;			/* our directinput interface */
		IDirectInputDevice*			m_directInputKeyboard;	/* our directinput keyboard interface */

		cInputTypedef::iSModifierState	m_modifierState;	/* the current modifier state */

		enum
		{
			MOUSE_LB,	/// left
			MOUSE_MB,	/// middle
			MOUSE_RB,	/// right
			MOUSE_BUTTON_NUM
		};
		IDirectInputDevice* m_directInputMouse;
		bool m_mouseButtonDown[MOUSE_BUTTON_NUM];
		bool m_preMouseButtonDown[MOUSE_BUTTON_NUM];
		int m_wheelDelta;
		int m_mouseX, m_mouseY;
		int m_mouseDx, m_mouseDy;
		bool m_swapMouseButton;
		bool m_mouseMove;
		bool m_mouseWheel;
		cInputLocaleInfo m_localeInfo;
		callBackMouseEvent m_callBackMouseEvent;
	};
}








#endif