
#include "BreadPostEffectSepia.h"
#include "BreadPostEffectShaderList.h"
#include "BreadRenderer.h"


namespace Bread
{
	bool cPostEffectSepia::initShader()
	{
		if (!createShader<cPostEffectSepiaShader>(_T("posteffectsepia.fx")))
			return false;
		return true;
	}
	/*
	 *	
	 */
	void cPostEffectSepia::render(cRenderToTextureBase* rtt, cTexture* preSceneTexture, sUiPlane const* screen)
	{
		cPostEffectBase::renderShader<cPostEffectSepiaShader>(rtt, preSceneTexture, screen);
	}
}