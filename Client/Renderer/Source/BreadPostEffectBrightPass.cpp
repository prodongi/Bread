
#include "BreadPostEffectBrightPass.h"
#include "BreadPostEffectShaderList.h"
#include "BreadRenderer.h"


namespace Bread
{
	/*
	 *	
	 */
	bool cPostEffectBrightPass::initShader()
	{
		if (!createShader<cPostEffectBrightPassShader>(_T("posteffectbrightpass.fx")))
			return false;
		return true;
	}
	/*
	 *	
	 */
	void cPostEffectBrightPass::render(cRenderToTextureBase* rtt, cTexture* preSceneTexture, sUiPlane const* screen)
	{
		cPostEffectBase::renderShader<cPostEffectBrightPassShader>(rtt, preSceneTexture, screen);
	}
}