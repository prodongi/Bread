
#include "BreadStdHeader.h"
#include "BreadInputSystem.h"


namespace Bread
{
	static DIDEVICEOBJECTDATA theBufferedKeys[kisBufferSize];

	/*
	* constructor
	* cInputSystem
	*
	* Initializes the input system.
	*
	*/
	cInputSystem::cInputSystem(void)
	{ 
		m_hinstance				= NULL;
		m_directInput			= NULL;
		m_directInputKeyboard	= NULL;
		m_directInputMouse		= NULL;
		m_modifierState		= cInputTypedef::kismsNone;
		m_callBackMouseEvent = NULL;
	}


	/*
	* destructor
	* ~cInputSystem
	*
	* Add clean up code here.
	*
	*/
	cInputSystem::~cInputSystem(void)
	{ 
		m_justPressedKeys.clear();
		m_justUnPressedKeys.clear();
		m_pressedKeys.clear();

		safeRelease(m_directInput);
		safeRelease(m_directInputKeyboard);
		safeRelease(m_directInputMouse);

		/* uninitialize COM */
		::CoUninitialize();
	}


	/*
	* static
	* getModifierState
	*
	* Given a scan code, returns the modifier state.
	*
	*/
	cInputTypedef::iSModifierState cInputSystem::getModifierState(unsigned char inScanCode)
	{
		cInputTypedef::iSModifierState theState = cInputTypedef::kismsNone;

		if ((inScanCode == DIK_LCONTROL) ||
			(inScanCode == DIK_RCONTROL))
		{
			theState = cInputTypedef::kismsCtrl;
		}
		else
			if ((inScanCode == DIK_LMENU) ||
				(inScanCode == DIK_RMENU))
			{
				theState = cInputTypedef::kismsAlt;
			}
			else
				if ((inScanCode == DIK_LSHIFT) ||
					(inScanCode == DIK_RSHIFT))
				{
					theState = cInputTypedef::kismsShift;
				}

				return theState;
	}


	/*
	* public
	* Initialize
	*
	* Initializes the input system.
	*/
	bool cInputSystem::initialize(HINSTANCE hinstance, HWND hwnd)
	{
		HRESULT		theHResult;

		/* clear out some variables */
		this->m_modifierState		= cInputTypedef::kismsNone;
		this->m_justPressedKeys.clear();
		this->m_justUnPressedKeys.clear();
		this->m_pressedKeys.clear();

		/* set the application instance */
		this->m_hinstance = hinstance;

		/* set the window handle */
		this->m_hwnd = hwnd;

		/* initialize com (this probably wouldnt be here in your app) */
		theHResult = ::CoInitialize(NULL);

		if (SUCCEEDED(theHResult))
		{
			/* initialize directinput */
			theHResult = ::DirectInput8Create(this->m_hinstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&this->m_directInput, NULL);

			if (SUCCEEDED(theHResult))
			{
				createKeyboard();
				createMouse();				
			}
		}

		m_localeInfo.initialize(hwnd);

		return (SUCCEEDED(theHResult));
	}

	/*
	 *	
	 */
	void cInputSystem::createKeyboard()
	{
		HRESULT		theHResult;
		DIPROPDWORD	theBufferData;

		/* create our keyboard interface */
		theHResult = this->m_directInput->CreateDevice(GUID_SysKeyboard, &this->m_directInputKeyboard, NULL);

		if (SUCCEEDED(theHResult))
		{
			/* set the data format */
			theHResult = this->m_directInputKeyboard->SetDataFormat(&c_dfDIKeyboard);

			if (SUCCEEDED(theHResult))
			{
				/* set the cooperative level (your application will probably want to use (DISCL_FOREGROUND | DISCL_EXCLUSIVE) */
				theHResult = this->m_directInputKeyboard->SetCooperativeLevel(this->m_hwnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);

				if (SUCCEEDED(theHResult))
				{
					theBufferData.diph.dwSize		= sizeof(DIPROPDWORD);
					theBufferData.diph.dwHeaderSize	= sizeof(DIPROPHEADER);
					theBufferData.diph.dwObj		= 0;
					theBufferData.diph.dwHow		= DIPH_DEVICE;
					theBufferData.dwData			= kisBufferSize;

					/* set the size of the keyboard buffer */
					theHResult = this->m_directInputKeyboard->SetProperty(DIPROP_BUFFERSIZE, &theBufferData.diph);

					if (SUCCEEDED(theHResult))
					{
						/* acquire the keyboard */
						theHResult = this->m_directInputKeyboard->Acquire();
					}
				}
			}
		}
	}
	/*
	 *	
	 */
	void cInputSystem::createMouse()
	{
		ZeroMemory(m_mouseButtonDown, MOUSE_BUTTON_NUM);
		m_wheelDelta = 0;
		m_mouseX = m_mouseY = 0;
		m_mouseDx = m_mouseDy = 0;
		m_swapMouseButton = false;

		HRESULT hr = m_directInput->CreateDevice(GUID_SysMouse, &m_directInputMouse, NULL);
		if (SUCCEEDED(hr))
		{
			hr = m_directInputMouse->SetDataFormat(&c_dfDIMouse);
			if (SUCCEEDED(hr))
			{
				hr = m_directInputMouse->SetCooperativeLevel(m_hwnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);
				if (SUCCEEDED(hr))
				{
					DIPROPDWORD dipdw;
					dipdw.diph.dwSize		= sizeof (DIPROPDWORD);
					dipdw.diph.dwHeaderSize = sizeof (DIPROPHEADER);
					dipdw.diph.dwObj		= 0;
					dipdw.diph.dwHow		= DIPH_DEVICE;
					dipdw.dwData			= kisBufferSize;
					hr = m_directInputMouse->SetProperty(DIPROP_BUFFERSIZE, &dipdw.diph);
					if (SUCCEEDED(hr))
					{
						m_directInputMouse->Acquire();
					}
				}
			}
		}
	}

	bool cInputSystem::update()
	{
		updateKeyboard();
		updateMouse();
		return true;
	}


	/* 
	* public
	* Refresh
	*
	* Refreshes the input system, querying for new
	* inputs and filling the vector of events.
	*/
	bool cInputSystem::updateKeyboard(void)
	{
		static KeyListType::iterator	theIterator;

		HRESULT					theHResult = S_OK;

		/* make sure we have a direct input interface */
		if (this->m_directInputKeyboard)
		{
			DWORD					theNumKeys = kisBufferSize; 
			unsigned long			theLoop;

			/* clear the list of just pressed and unpressed keys */
			this->m_justPressedKeys.clear();
			this->m_justUnPressedKeys.clear();

			/* query the direct input buffer */
			theHResult = this->m_directInputKeyboard->GetDeviceData(sizeof(DIDEVICEOBJECTDATA), theBufferedKeys, &theNumKeys, 0); 

			if (SUCCEEDED(theHResult))
			{
				if (theHResult == DI_BUFFEROVERFLOW)
				{
					/* it's possible to have keyboard overflow.  in this case,
					* it would be wise to call GetDeviceState and
					* compare the keys that are down with what the vectors
					* have in them... adding the appropriate keys to the pressed
					* vector.  exercise for the reader =)
				 */
				}

				/* loop over the number of key inputs */
				for (theLoop = 0; theLoop < theNumKeys; theLoop++)
				{
					cInputKey theTempKey;
					cInputTypedef::iSModifierState theTempModifier;

					/* wrap the key in an cInputKey object */
					theTempKey.m_timeStamp = theBufferedKeys[theLoop].dwTimeStamp;
					theTempKey.m_scanCode  = (unsigned char)theBufferedKeys[theLoop].dwOfs;
					theTempKey.m_modifier  = this->m_modifierState;

					/* check if the key is a modifer */
					theTempModifier = cInputSystem::getModifierState((unsigned char)theBufferedKeys[theLoop].dwOfs);

					/* is the key is down? */
					if (theBufferedKeys[theLoop].dwData & 0x80)
					{
						/* set the modifier state if necessary */
						if (theTempModifier != cInputTypedef::kismsNone)
							this->m_modifierState |= theTempModifier;

						/* this key was just pressed... */
						this->m_justPressedKeys.push_back(theTempKey);

						/* ...as well as "pressed" */
						theIterator = std::find(this->m_pressedKeys.begin(), this->m_pressedKeys.end(), theTempKey);
						if (theIterator == this->m_pressedKeys.end())
							this->m_pressedKeys.push_back(theTempKey);
					}
					else
					{
						/* set the modifier state if necessary */
						if (theTempModifier != cInputTypedef::kismsNone)
						{
							this->m_modifierState ^= theTempModifier;
							theTempKey.m_modifier ^= theTempModifier;
						}

						/* this key was just released */
						this->m_justUnPressedKeys.push_back(theTempKey);

						/* erase this key from the pressed list, if we can find it. */
						theIterator = this->m_pressedKeys.begin();
						while (theIterator != this->m_pressedKeys.end())
						{
							if ((*theIterator).m_scanCode == theTempKey.m_scanCode)
								theIterator = this->m_pressedKeys.erase(theIterator);
							else
								++theIterator;
						}
					}
				}
			}
			else
			{
				if ((theHResult == DIERR_INPUTLOST) ||
					(theHResult == DIERR_NOTACQUIRED))
				{
					/* we lost the device, reaquire it. */
					theHResult = this->m_directInputKeyboard->Acquire();
				}
			}
		}

		return (SUCCEEDED(theHResult));
	}
	/*
	 *	
	 */
	bool cInputSystem::updateMouse()
	{
		HRESULT hr = S_OK;
		if (m_directInputMouse)
		{
			DWORD theNumKeys = kisBufferSize; 
			hr = m_directInputMouse->GetDeviceData(sizeof (DIDEVICEOBJECTDATA), theBufferedKeys, &theNumKeys, 0);
			if (SUCCEEDED(hr))
			{
				m_mouseDx = m_mouseDy = m_wheelDelta = 0;
				//
				memcpy(m_preMouseButtonDown, m_mouseButtonDown, MOUSE_BUTTON_NUM);

				for (DWORD i = 0; i < theNumKeys; ++i)
				{
					const DIDEVICEOBJECTDATA& od = theBufferedKeys[i];
					switch (od.dwOfs)
					{
					// middle button
					case DIMOFS_BUTTON2:
						if (od.dwData & 0x80)
							m_mouseButtonDown[MOUSE_MB] = true;
						else
							m_mouseButtonDown[MOUSE_MB] = false;
						break;
					case DIMOFS_BUTTON1:
					case DIMOFS_BUTTON0:
						if ((m_swapMouseButton && DIMOFS_BUTTON1 == od.dwOfs) ||
						   (!m_swapMouseButton && DIMOFS_BUTTON0 == od.dwOfs))
						{
							if (od.dwData & 0x80)
								m_mouseButtonDown[MOUSE_LB] = true;
							else
								m_mouseButtonDown[MOUSE_LB] = false;
						}
						if ((m_swapMouseButton && DIMOFS_BUTTON0 == od.dwOfs) ||
						   (!m_swapMouseButton && DIMOFS_BUTTON1 == od.dwOfs))
						{
							if (od.dwData & 0x80)
								m_mouseButtonDown[MOUSE_RB] = true;
							else
								m_mouseButtonDown[MOUSE_RB] = false;
						}
						break;
					case DIMOFS_X:
						m_mouseDx = od.dwData;
						break;
					case DIMOFS_Y:
						m_mouseDy = od.dwData;
						break;
					case DIMOFS_Z:
						m_wheelDelta = od.dwData;
						break;
					}
				}

				(*m_callBackMouseEvent)();
			}
			else
			{
				if (hr == DIERR_INPUTLOST || hr == DIERR_NOTACQUIRED)
				{
					hr = m_directInputMouse->Acquire();
				}
			}
		}
		return SUCCEEDED(hr);
	}
	/*
	 *	
	 */
	LRESULT CALLBACK cInputSystem::wndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		setMouseMove(false);
		setMouseWheel(false);

		int result = 1;
		result = m_localeInfo.wndProc(hWnd, msg, wParam, lParam);

		switch (msg)
		{
		case WM_MOUSEMOVE:
			setMouseMove(true);
			setMouseX((short)LOWORD(lParam));
			setMouseY((short)HIWORD(lParam));
			break;
		case WM_MOUSEWHEEL:
			// dinput으로 받으면 포커스가 없을때도 입력을 받으므로 윈도우 메세지로 휠 입력이 있는지 체크한다
			setMouseWheel(true);
			break;
		}
		return result;
	}
	/*
	 *	
	 */
	bool cInputSystem::isJustPressedKey(unsigned char key, cInputTypedef::iSModifierState modifierState)
	{
		cInputKey inputKey(key, modifierState);
		KeyListType::iterator it = std::find(m_justPressedKeys.begin(), m_justPressedKeys.end(), inputKey);
		return (it == m_justPressedKeys.end()) ? false : true;
	}
	/*
	 *	
	 */
	bool cInputSystem::isPressedKey(unsigned char key, cInputTypedef::iSModifierState modifierState)
	{
		cInputKey inputKey(key, modifierState);
		KeyListType::iterator it = std::find(m_pressedKeys.begin(), m_pressedKeys.end(), inputKey);
		return (it == m_pressedKeys.end()) ? false : true;
	}
	/*
	 *	
	 */
	bool cInputSystem::isJustUnPressedKey(unsigned char key, cInputTypedef::iSModifierState modifierState)
	{
		cInputKey inputKey(key, modifierState);
		KeyListType::iterator it = std::find(m_justUnPressedKeys.begin(), m_justUnPressedKeys.end(), inputKey);
		return (it == m_justUnPressedKeys.end()) ? false : true;
	}
	/*
	*/
	int cInputSystem::renderDebugInfo(int y)
	{
		return m_localeInfo.renderDebugInfo(y);
	}
	/*
	*/
	void cInputSystem::setMouseEventCallBack(callBackMouseEvent func)
	{
		m_callBackMouseEvent = func;
	}
}
































