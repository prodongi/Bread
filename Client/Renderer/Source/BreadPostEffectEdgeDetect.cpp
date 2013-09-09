
#include "BreadPostEffectEdgeDetect.h"
#include "BreadPostEffectShaderList.h"
#include "BreadRenderer.h"


namespace Bread
{
	/*
	 *	
	 */
	bool cPostEffectEdgeDetect::initShader()
	{
		if (!createShader<cPostEffectEdgeDetectShader>(_T("posteffectedgedetect.fx")))
			return false;
		return true;
	}
	/*
	 *	
	 */
	void cPostEffectEdgeDetect::initShaderParam(uint width, uint height)
	{
		float offset_u = 1.0f/(float)width;
		float offset_v = 1.0f/(float)height;
		float texelSampling[8];
		texelSampling[0] = 0.0f;		texelSampling[1] = offset_v;
		texelSampling[2] = offset_u;	texelSampling[3] = 0.0f;
		texelSampling[4] = 0.0f;		texelSampling[5] = -offset_v;
		texelSampling[6] = -offset_u;	texelSampling[7] = 0.0f;

		m_shader->setValue(_T("texelSampling"), (void*)texelSampling, 8 * sizeof (float));
	}
	/*
	 *	
	 */
	void cPostEffectEdgeDetect::render(cRenderToTextureBase* rtt, cTexture* preSceneTexture, sUiPlane const* screen)
	{
		cPostEffectBase::renderShader<cPostEffectEdgeDetectShader>(rtt, preSceneTexture, screen);
	}
}