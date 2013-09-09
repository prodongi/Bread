
#ifndef _BreadPostEffectNoShader_h_
#define _BreadPostEffectNoShader_h_

#include "BreadPostEffectBase.h"

namespace Bread
{
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
	 * \date 2010-03-24
	 *
	 * \author Administrator
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
	class cPostEffectNoShader : public cPostEffectBase
	{
	public:
		cPostEffectNoShader() {}
		virtual ~cPostEffectNoShader() {}
		virtual void render(cRenderToTextureBase* rtt, cTexture* sceneTexture, sUiPlane const* screen)
		{
			cRenderer::renderScreen(rtt, screen, sceneTexture);
		}
	protected:
		virtual bool initShader() {	return true;	}
	};
}








#endif