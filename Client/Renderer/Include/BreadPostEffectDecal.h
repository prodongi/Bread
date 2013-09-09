
#ifndef _BreadPostEffectDecal_h_
#define _BreadPostEffectDecal_h_

/*
	BreadPostEffectDecal.h
	Copyright (c) prodongi. All rights resserved.

	date : 2012-12-19
	purpose : 
*/

#include "BreadPostEffectBase.h"

namespace Bread
{
	/*
	*/
	class cPostEffectDecal : public cPostEffectBase
	{
	public:
		cPostEffectDecal();
		virtual ~cPostEffectDecal();
		virtual void render(cRenderToTextureBase* rtt, cTexture* preSceneTexture, sUiPlane const* screen);

	protected:
		virtual bool initShader();
	};
}

#endif
