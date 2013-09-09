
#include "BreadPostEffectInverse.h"
#include "BreadPostEffectShaderList.h"
#include "BreadRenderer.h"


namespace Bread
{
	bool cPostEffectInverse::initShader()
	{
		if (!createShader<cPostEffectInverseShader>(_T("posteffectinverse.fx")))
			return false;
		return true;
	}
	/*
	 *	
	 */
	void cPostEffectInverse::render(cRenderToTextureBase* rtt, cTexture* preSceneTexture, sUiPlane const* screen)
	{
		cPostEffectBase::renderShader<cPostEffectInverseShader>(rtt, preSceneTexture, screen);
	}
}