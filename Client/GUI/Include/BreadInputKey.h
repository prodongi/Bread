
#ifndef _BreadcInputKey_h_
#define _BreadcInputKey_h_

#include <Windows.h>

#include "BreadStdHeader.h"
#include "BreadInputTypedef.h"

namespace Bread
{
	/*
	* cInputKey
	*
	* This simple class is the scan code and modifier
	* of a key.
	*/

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
	 * \todo gpg3권 1.13에서 갖고옴
	 *
	 * \bug 
	 *
	 */
	class cInputKey
	{
	public:
		cInputKey(unsigned char inScanCode = 0, cInputTypedef::iSModifierState inModifier = cInputTypedef::kismsNone) 
			: m_timeStamp(0), m_scanCode(inScanCode), m_modifier(inModifier)
		{ }

		cInputKey& operator= (const cInputKey& inKey)
		{
			this->m_modifier  = inKey.m_modifier;
			this->m_scanCode  = inKey.m_scanCode;
			this->m_timeStamp = inKey.m_timeStamp;
			return *this;
		}

		inline bool operator== (const cInputKey& inThat) const
		{ 
			return ((this->m_modifier == inThat.m_modifier) && (this->m_scanCode == inThat.m_scanCode)); 
		}

		inline bool operator< (const cInputKey& inThat) const
		{ 
			unsigned short theFirst	 = this->pack();
			unsigned short theSecond = inThat.pack();
			return (theFirst < theSecond); 
		}

		inline unsigned short pack(void) const
		{
			unsigned short theResult;
			theResult = this->m_modifier;
			theResult = theResult << 8;
			theResult = theResult | this->m_scanCode;
			return theResult;
		}

		unsigned long					m_timeStamp;
		cInputTypedef::iSModifierState	m_modifier;
		unsigned char					m_scanCode;
	};

	/* typedefs */
	typedef std::vector <cInputKey> KeyListType;
}










#endif