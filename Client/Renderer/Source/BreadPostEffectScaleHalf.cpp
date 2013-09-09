
#include "BreadPostEffectScaleHalf.h"
#include "BreadPostEffectShaderList.h"
#include "BreadRenderer.h"


namespace Bread
{
	bool cPostEffectScaleHalf::initShader()
	{
		if (!createShader<cPostEffectScaleHalfShader>(_T("posteffectscalehalf.fx")))
			return false;
		return true;
	}
	/*
	 *	
	 */
	void cPostEffectScaleHalf::render(cRenderToTextureBase* rtt, cTexture* preSceneTexture, sUiPlane const* screen)
	{
		cPostEffectBase::renderShader<cPostEffectScaleHalfShader>(rtt, preSceneTexture, screen);
	}
}