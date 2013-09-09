
#include "BreadPostEffectGBlurH.h"
#include "BreadPostEffectShaderList.h"
#include "BreadRenderer.h"


namespace Bread
{
	/*
	 *	
	 */
	bool cPostEffectGBlurH::initShader()
	{
		if (!createShader<cPostEffectGBlurHShader>(_T("posteffectgblurh.fx")))
			return false;
		return true;
	}
	/*
	 *	
	 */
	void cPostEffectGBlurH::initShaderParam(uint width, uint /*height*/)
	{
		float offsetU = 1.0f/(float)width;
		float samplingU[13];
		for (int i = 0; i < 13; ++i)
		{
			samplingU[i] = (-6.0f + (float)i) * offsetU;
		}
		m_shader->setFloatArray(_T("samplingU"), samplingU, 13);
	}
	/*
	 *	
	 */
	void cPostEffectGBlurH::render(cRenderToTextureBase* rtt, cTexture* preSceneTexture, sUiPlane const* screen)
	{
		cPostEffectBase::renderShader<cPostEffectGBlurHShader>(rtt, preSceneTexture, screen);
	}
}