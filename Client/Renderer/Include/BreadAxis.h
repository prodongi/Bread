
#ifndef _BreadAxis_h_
#define _BreadAxis_h_

#include "BreadD3DHeader.h"

namespace Bread
{
	struct sLineVertex;

	/**
	 * \ingroup Renderer
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
	 * \date 2009-09-05
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
	class cAxis
	{
	public:
		cAxis();
		~cAxis();

		friend class cRenderer;
	private:
		void initialize();
		void render();

	public:
		static bool m_isRender;
	private:
		sLineVertex* m_iv;
	};
}














#endif












