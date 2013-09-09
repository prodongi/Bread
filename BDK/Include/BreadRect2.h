
#ifndef _BreadRect2_h_
#define _BreadRect2_h_

#include "BreadVector2.h"

namespace Bread
{
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
	 * \date 2009-08-22
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
	 * \todo 
	 *
	 * \bug 
	 *
	 */
	class cRect2
	{
	public:
		cRect2() : m_pos(0.0f, 0.0f), m_width(1.0f), m_height(1.0f) {}
		cRect2(float x, float y, float w, float h);
		~cRect2();

		inline cRect2 const& operator = (cRect2 const& rect)
		{
			m_pos = rect.m_pos;
			m_width = rect.m_width;
			m_height = rect.m_height;

			return *this;
		}
		template <class X, class Y>
		bool isIn(X x, Y y)
		{
			float _x = (float)x;
			float _y = (float)y;
			if (_x >= m_pos.x && _x <= m_pos.x + m_width &&
				_y >= m_pos.y && _y <= m_pos.y + m_height)
				return true;
			return false;
		}

		sVector2 m_pos;
		float m_width;
		float m_height;
	};
}








#endif