
#include "BreadPostEffectBloomH.h"
#include "BreadPostEffectShaderList.h"
#include "BreadRenderer.h"


namespace Bread
{
	/*
	 *	
	 */
	bool cPostEffectBloomH::initShader()
	{
		if (!createShader<cPostEffectBloomHShader>(_T("posteffectbloomh.fx")))
			return false;
		return true;
	}
	/*
	 *	
	 */
	void cPostEffectBloomH::initShaderParam(uint /*width*/, uint height)
	{
		float offsetV = 1.0f/(float)height;
		float samplingV[13];
		for (int i = 0; i < 13; ++i)
		{
			samplingV[i] = (-6.0f + (float)i) * offsetV;
		}
		m_shader->setFloatArray(_T("samplingU"), samplingV, 13);
	}
	/*
	 *	
	 */
	void cPostEffectBloomH::render(cRenderToTextureBase* rtt, cTexture* preSceneTexture, sUiPlane const* screen)
	{
		cPostEffectBase::renderShader<cPostEffectBloomHShader>(rtt, preSceneTexture, screen);
	}
}