
#include "BreadPostEffectScaleOri.h"
#include "BreadPostEffectShaderList.h"
#include "BreadRenderer.h"


namespace Bread
{
	bool cPostEffectScaleOri::initShader()
	{
		if (!createShader<cPostEffectScaleOriShader>(_T("posteffectscaleori.fx")))
			return false;
		return true;
	}
	/*
	 *	
	 */
	void cPostEffectScaleOri::render(cRenderToTextureBase* rtt, cTexture* preSceneTexture, sUiPlane const* screen)
	{
		cPostEffectBase::renderShader<cPostEffectScaleOriShader>(rtt, preSceneTexture, screen);
	}
}