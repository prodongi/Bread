
#include "BreadPostEffectScaleQuart.h"
#include "BreadPostEffectShaderList.h"
#include "BreadRenderer.h"


namespace Bread
{
	bool cPostEffectScaleQuart::initShader()
	{
		if (!createShader<cPostEffectScaleQuartShader>(_T("posteffectscalequart.fx")))
			return false;
		return true;
	}
	/*
	 *	
	 */
	void cPostEffectScaleQuart::render(cRenderToTextureBase* rtt, cTexture* preSceneTexture, sUiPlane const* screen)
	{
		cPostEffectBase::renderShader<cPostEffectScaleQuartShader>(rtt, preSceneTexture, screen);
	}
}