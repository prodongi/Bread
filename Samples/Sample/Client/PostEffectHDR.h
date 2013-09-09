
#ifndef _PostEffectHDR_h_
#define _PostEffectHDR_h_

#include "BreadUiPrimitive.h"
#include "BreadRenderToTexture.h"

namespace Bread
{
	class cShader;
	class cTexture;
}

/**
 * \ingroup Sample
 *
 *
 * \par requirements
 * win98 or later\n
 * win2k or later\n
 * MFC\n
 *
 * \version 1.0
 * first version
 *
 * \date 2010-02-03
 *
 * \author Prodongi
 *
 * \par license
 * This code is absolutely free to use and modify. The code is provided "as is" with
 * no expressed or implied warranty. The author accepts no liability if it causes
 * any damage to your computer, causes your pet to fall ill, increases baldness
 * or makes your car start emitting strange noises when you start it up.
 * This code has no bugs, just undocumented features!
 * 
 * \todo 
 *
 * \bug 
 *
 */
class cHDRLuminance
{
public:
	cHDRLuminance();
	~cHDRLuminance();
	bool initialize(int rttNum, uint width, uint height);
	void render(cTexture* originalTexture);
	cRenderToTextureBase* getRtt(int num) const		{	return m_rttList + num;	}

private:
	void renderGrayScaleDownSample(cShader* shader, cTexture* originalTexture);
	void renderDownSample(cShader* shader);

private:
	int m_rttNum;
	cRenderToTextureMSAA* m_rttList;
	sUiPlane m_screen;
	D3DXVECTOR4 m_greyScaleOffset[4];
	D3DXVECTOR4 m_downSampleOffset[9];
};
/*
 *	
 */
class cHDRPostProcess
{
public:
	cHDRPostProcess();
	~cHDRPostProcess();
	bool initialize(uint width, uint height);
	void render(cTexture* originalTexture);
	int renderDebugInfo(int y);
	cRenderToTextureBase* getRtt(int type) const	{	return m_rttList + type;	}
	float getGaussMultiplier() const			{	return m_gaussMultiplier;	}

	enum	{	BRIGHT_PASS, DOWN_SAMPLE, BLUR_HORIZONTAL, BLUR_VERTICAL, PASS_NUM	};

private:
	void renderBrightPass(cShader* shader, cTexture* originalTexture);
	void renderDownSample(cShader* shader);
	void renderBlurHorizontal(cShader* shader);
	void renderBlurVertical(cShader* shader);
	float ComputeGaussianValue( float x, float mean, float std_deviation );

private:
	cRenderToTextureMSAA* m_rttList;
	float m_brightThreshold;
	float m_gaussMultiplier;
	float m_gaussMean;
	float m_gaussStdDev;
	sUiPlane m_screen;
	D3DXVECTOR4 m_brightPassOffset[4];
	D3DXVECTOR4 m_downSampleOffset[16];
	float m_bloomWeight[9];
	float m_bloomOffset[9];
};
/*
 *	
 */
class cPostEffectHDR
{
public:
	cPostEffectHDR();
	~cPostEffectHDR();
	bool initialize(int luminanceRttNum, uint width, uint height);
	void render(cTexture* originalTexture);
	int renderDebugInfo(int y);
	cTexture*	getFinalRttTexture() const				{	return m_rttFinal->getTexture();					}
	sUiPlane const*		getFinalScreen() const			{	return &m_screen;									}
	cTexture*	getLuminanceRttTexture(int num) const	{	return m_luminance->getRtt(num)->getTexture();		}
	cTexture*	getPostProcessRttTexture(int type)		{	return m_postProcess->getRtt(type)->getTexture();	}

private:
	void renderFinal(cTexture* originalTexture);

private:
	cHDRLuminance* m_luminance;
	cHDRPostProcess* m_postProcess;
	cRenderToTextureBase* m_rttFinal;
	sUiPlane m_screen;
	float m_exposure;
};







#endif