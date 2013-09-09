
#include "BreadPostEffectBloomV.h"
#include "BreadPostEffectShaderList.h"
#include "BreadRenderer.h"


namespace Bread
{
	/*
	 *	
	 */
	bool cPostEffectBloomV::initShader()
	{
		if (!createShader<cPostEffectBloomVShader>(_T("posteffectbloomv.fx")))
			return false;
		return true;
	}
	/*
	 *	
	 */
	void cPostEffectBloomV::initShaderParam(uint /*width*/, uint height)
	{
		float offsetV = 1.0f/(float)height;
		float samplingV[13];
		for (int i = 0; i < 13; ++i)
		{
			samplingV[i] = (-6.0f + (float)i) * offsetV;
		}
		m_shader->setFloatArray(_T("samplingV"), samplingV, 13);
	}
	/*
	 *	
	 */
	void cPostEffectBloomV::render(cRenderToTextureBase* rtt, cTexture* preSceneTexture, sUiPlane const* screen)
	{
		cPostEffectBase::renderShader<cPostEffectBloomVShader>(rtt, preSceneTexture, screen);
	}
}