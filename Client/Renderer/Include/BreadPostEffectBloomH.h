
#ifndef _BreadPostEffectBloomH_h_
#define _BreadPostEffectBloomH_h_

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
	 * \date 2010-03-23
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
	class cPostEffectBloomH : public cPostEffectBase
	{
	public:
		cPostEffectBloomH() {}
		virtual ~cPostEffectBloomH() {}
		virtual void render(cRenderToTextureBase* rtt, cTexture* preSceneTexture, sUiPlane const* screen);

	protected:
		virtual bool initShader();
		virtual void initShaderParam(uint width, uint height);
	};
}








#endif