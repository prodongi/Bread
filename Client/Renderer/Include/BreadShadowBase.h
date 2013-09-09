
#ifndef _BreadShadowBase_h_
#define _BreadShadowBase_h_

#include "BreadDeclaration.h"

namespace Bread
{
	/**
	* \ingroup Sample
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
	* \date 2010-03-05
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
	* \todo �� �׸��� Ŭ������ �������̽� Ŭ����
	*
	* \bug 
	*
	*/
	class cShadowBase
	{
	public:
		cShadowBase() {};
		virtual ~cShadowBase() {};
		virtual bool initialize(uint width, uint height) 
		{	m_width = width;	m_height = height;	return true;	}
		virtual void update(float /*elapsedtime*/)	{}
		virtual void render() {}
		virtual void beforeRender3D() {}
		virtual void afterRender3D() {}
		virtual void beforeRender2D() {}
		virtual void afterRender2D() {}
		virtual int renderDebugInfo(int y) {	return y;	}

	protected:
		uint m_width, m_height;
	};
}








#endif