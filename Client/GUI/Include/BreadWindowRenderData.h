
#ifndef _BreadWindowRenderData_h_
#define _BreadWindowRenderData_h_

#include "BreadD3DHeader.h"
#include "BreadDeclaration.h"
#include "BreadRect2.h"
#include "BreadUiPrimitive.h"
#include "BreadVertexBuffer.h"

namespace Bread
{
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
	class cWindowRenderData
	{
	public:
		cWindowRenderData();
		virtual ~cWindowRenderData();
		virtual void render(LPDIRECT3DDEVICE9 device);
		propertyFunc(D3DCOLOR, Color, m_color)

	protected:
		bool createVB(cRect2 const& posRect, cRect2 const& texRect, float texWidth, float texHeight);

	private:
		sVertexBuffer m_vb;
		D3DCOLOR m_color;
	};
}











#endif




