
#include "BreadPostEffectToneMap.h"
#include "BreadPostEffectShaderList.h"
#include "BreadRenderer.h"


namespace Bread
{
	bool cPostEffectToneMap::initShader()
	{
		if (!createShader<cPostEffectToneMapShader>(_T("posteffecttonemap.fx")))
			return false;
		return true;
	}
	/*
	 *	
	 */
	void cPostEffectToneMap::render(cRenderToTextureBase* rtt, cTexture* preSceneTexture, sUiPlane const* screen)
	{
		cPostEffectBase::renderShader<cPostEffectToneMapShader>(rtt, preSceneTexture, screen);
	}
}