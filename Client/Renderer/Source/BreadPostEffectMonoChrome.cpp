
#include "BreadPostEffectMonoChrome.h"
#include "BreadPostEffectShaderList.h"
#include "BreadRenderer.h"


namespace Bread
{
	bool cPostEffectMonoChrome::initShader()
	{
		if (!createShader<cPostEffectMonoChromeShader>(_T("posteffectmonochrome.fx")))
			return false;
		return true;
	}
	/*
	 *	
	 */
	void cPostEffectMonoChrome::render(cRenderToTextureBase* rtt, cTexture* preSceneTexture, sUiPlane const* screen)
	{
		cPostEffectBase::renderShader<cPostEffectMonoChromeShader>(rtt, preSceneTexture, screen);
	}
}