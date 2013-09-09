
#include "stdafx.h"
#include "PostEffectHDR.h"
#include "Shader.h"
#include "BreadUberShaderManager.h"
#include "BreadGui.h"


/*
 *	
 */
cHDRLuminance::cHDRLuminance() : m_rttList(NULL)
{
}
/*
 *	
 */
cHDRLuminance::~cHDRLuminance()
{
	safeDeleteArray(m_rttList);
}
/*
 *	
 */
bool cHDRLuminance::initialize(int rttNum, uint width, uint height)
{
	assert(!m_rttList);
	m_rttNum = rttNum;
	m_rttList = new cRenderToTextureMSAA[rttNum];

	uint textureSize = 1;
	for (int n = 0; n < m_rttNum; ++n)
	{
		if (!m_rttList[n].initialize(0, textureSize, textureSize))
			return false;
		textureSize *= 3;
	}

	//
	float sU = 1.0f / (float)width;
	float sV = 1.0f / (float)height;
	m_greyScaleOffset[0] = D3DXVECTOR4( -0.5f * sU, 0.5f * sV, 0.0f, 0.0f );
	m_greyScaleOffset[1] = D3DXVECTOR4( 0.5f * sU, 0.5f * sV, 0.0f, 0.0f );
	m_greyScaleOffset[2] = D3DXVECTOR4( -0.5f * sU, -0.5f * sV, 0.0f, 0.0f );
	m_greyScaleOffset[3] = D3DXVECTOR4( 0.5f * sU, -0.5f * sV, 0.0f, 0.0f );

	return true;
}
/*
 *	
 */
void cHDRLuminance::render(cTexture* originalTexture)
{
	cUberShader* shader = _getUberShaderMgr()->get(_T("hdr.fx"));
	assert(shader);

	renderGrayScaleDownSample(shader, originalTexture);
	renderDownSample(shader);
}
/*
 *	
 */
void cHDRLuminance::renderGrayScaleDownSample(cShader* shader, cTexture* originalTexture)
{
	cRenderToTextureBase* rtt = m_rttList + m_rttNum - 1;
	
	shader->setTechnique(_T("TLuminance1"));
	shader->setTexture(_T("baseTexture"), originalTexture);
	shader->setVectorArray(_T("tcLumOffsets"), m_greyScaleOffset, 4);

	m_screen.set(0.0f, 0.0f, (float)rtt->getWidth(), (float)rtt->getHeight(), 0xffffffff);
	cRenderer::renderScreen(rtt, shader, &m_screen);
}
/*
 *	
 */
void cHDRLuminance::renderDownSample(cShader* shader)
{
	cRenderToTextureBase* srcRtt, *destRtt;
	for (int i = m_rttNum - 1; i > 0; --i)
	{
		srcRtt = m_rttList + i;
		destRtt = m_rttList + (i-1);

		float inv_w = cMath::inv((float)srcRtt->getWidth());
		float inv_h = cMath::inv((float)srcRtt->getHeight());

		int idx = 0;
		for( int x = -1; x < 2; x++ )
		{
			for( int y = -1; y < 2; y++ )
			{
				m_downSampleOffset[idx++] = D3DXVECTOR4((float)x * inv_w, (float)y * inv_h, 0.0f, 0.0f);
			}
		}

		shader->setTechnique(_T("TLuminance2"));
		shader->setTexture(_T("baseTexture"), srcRtt->getTexture());
		shader->setVectorArray(_T("tcDSOffsets"), m_downSampleOffset, 9);
			
		m_screen.set(0.0f, 0.0f, (float)destRtt->getWidth(), (float)destRtt->getHeight(), 0xffffffff);
		cRenderer::renderScreen(destRtt, shader, &m_screen);
	}
}




/*
 *	
 */
cHDRPostProcess::cHDRPostProcess() : m_rttList(NULL)
{
}
/*
 *	
 */
cHDRPostProcess::~cHDRPostProcess()
{
	safeDeleteArray(m_rttList);
}
/*
 *	
 */
bool cHDRPostProcess::initialize(uint width, uint height)
{
	m_rttList = new cRenderToTextureMSAA[PASS_NUM];
	// bright pass
	if (!m_rttList[BRIGHT_PASS].initialize(0, width/2, height/2))
		return false;
	if (!m_rttList[DOWN_SAMPLE].initialize(0, width/8, height/8))
		return false;
	if (!m_rttList[BLUR_HORIZONTAL].initialize(0, width/8, height/8))
		return false;
	if (!m_rttList[BLUR_VERTICAL].initialize(0, width/8, height/8))
		return false;

	m_brightThreshold = 0.4f;//8f;
	m_gaussMultiplier = 1.0f;//0.4f;
	m_gaussMean = 0.0f;
	m_gaussStdDev = 0.8f;

	//
	float sU = 1.0f/(float)width;
	float sV = 1.0f/(float)height;
	m_brightPassOffset[0] = D3DXVECTOR4( -0.5f * sU, 0.5f * sV, 0.0f, 0.0f );
	m_brightPassOffset[1] = D3DXVECTOR4( 0.5f * sU, 0.5f * sV, 0.0f, 0.0f );
	m_brightPassOffset[2] = D3DXVECTOR4( -0.5f * sU, -0.5f * sV, 0.0f, 0.0f );
	m_brightPassOffset[3] = D3DXVECTOR4( 0.5f * sU, -0.5f * sV, 0.0f, 0.0f );

	return true;
}
/*
 *	
 */
void cHDRPostProcess::render(cTexture* originalTexture)
{
	cUberShader* shader = _getUberShaderMgr()->get(_T("hdr.fx"));
	assert(shader);

	renderBrightPass(shader, originalTexture);
	renderDownSample(shader);
	renderBlurHorizontal(shader);
	renderBlurVertical(shader);
}
/*
 *	
 */
void cHDRPostProcess::renderBrightPass(cShader* shader, cTexture* originalTexture)
{
	cRenderToTextureBase* rtt = m_rttList + BRIGHT_PASS;
	shader->setTechnique(_T("TBrightPass"));
	shader->setTexture(_T("baseTexture"), originalTexture);
	shader->setFloat(_T("fBrightPassThreshold"), m_brightThreshold);
	shader->setVectorArray(_T("tcDownSampleOffsets"), m_brightPassOffset, 4);

	m_screen.set(0.0f, 0.0f, (float)rtt->getWidth(), (float)rtt->getHeight(), 0xffffffff);
	cRenderer::renderScreen(rtt, shader, &m_screen);
}
/*
 *	
 */
void cHDRPostProcess::renderDownSample(cShader* shader)
{
	cRenderToTextureBase* rtt = m_rttList + DOWN_SAMPLE;
	shader->setTechnique(_T("TDownSample"));
	shader->setTexture(_T("baseTexture"), m_rttList[BRIGHT_PASS].getTexture());

	float inv_w = cMath::inv((float)rtt->getWidth());
	float inv_h = cMath::inv((float)rtt->getHeight());
	int idx = 0;
	for( int i = -2; i < 2; i++ )
	{
		for( int j = -2; j < 2; j++ )
		{
			m_downSampleOffset[idx++] = D3DXVECTOR4(((float)i + 0.5f) * inv_w, ((float)j + 0.5f) * inv_h, 0.0f, 0.0f);
		}
	}
	shader->setVectorArray(_T("tcDownSampleOffsets"), m_downSampleOffset, 16);
	m_screen.set(0.0f, 0.0f, (float)rtt->getWidth(), (float)rtt->getHeight(), 0xffffffff);
	cRenderer::renderScreen(rtt, shader, &m_screen);
}
/*
 *	
 */
void cHDRPostProcess::renderBlurHorizontal(cShader* shader)
{
	cRenderToTextureBase* rtt = m_rttList + BLUR_HORIZONTAL;
	shader->setTechnique(_T("TBlurHorizontal"));
	shader->setTexture(_T("baseTexture"), m_rttList[DOWN_SAMPLE].getTexture());

	float inv_w = cMath::inv((float)rtt->getWidth());
	for( int i = 0; i < 9; i++ )
	{
		// Compute the offsets. We take 9 samples - 4 either side and one in the middle:
		//     i =  0,  1,  2,  3, 4,  5,  6,  7,  8
		//Offset = -4, -3, -2, -1, 0, +1, +2, +3, +4
		m_bloomOffset[i] = ((float)i - 4.0f) * inv_w;

		// 'x' is just a simple alias to map the [0,8] range down to a [-1,+1]
		float x = ((float)i - 0.4f) * 0.25f;

		// Use a gaussian distribution. Changing the standard-deviation
		// (second parameter) as well as the amplitude (multiplier) gives
		// distinctly different results.
		m_bloomWeight[i] = m_gaussMultiplier * ComputeGaussianValue( x, m_gaussMean, m_gaussStdDev );
	}
	shader->setFloatArray(_T("HBloomWeights"), m_bloomWeight, 9);
	shader->setFloatArray(_T("HBloomOffsets"), m_bloomOffset, 9);

	m_screen.set(0.0f, 0.0f, (float)rtt->getWidth(), (float)rtt->getHeight(), 0xffffffff);
	cRenderer::renderScreen(rtt, shader, &m_screen);
}
/*
 *	
 */
void cHDRPostProcess::renderBlurVertical(cShader* shader)
{
	cRenderToTextureBase* rtt = m_rttList + BLUR_VERTICAL;
	shader->setTechnique(_T("TBlurVertical"));
	shader->setTexture(_T("baseTexture"), m_rttList[BLUR_HORIZONTAL].getTexture());

	float inv_h = cMath::inv((float)rtt->getHeight());
	for( int i = 0; i < 9; i++ )
	{
		// Compute the offsets. We take 9 samples - 4 either side and one in the middle:
		//     i =  0,  1,  2,  3, 4,  5,  6,  7,  8
		//Offset = -4, -3, -2, -1, 0, +1, +2, +3, +4
		m_bloomOffset[i] = ( static_cast< float >( i ) - 4.0f ) * inv_h;

		// 'x' is just a simple alias to map the [0,8] range down to a [-1,+1]
		float x = ( static_cast< float >( i ) - 4.0f ) * 0.25f;

		// Use a gaussian distribution. Changing the standard-deviation
		// (second parameter) as well as the amplitude (multiplier) gives
		// distinctly different results.
		m_bloomWeight[i] = m_gaussMultiplier * ComputeGaussianValue( x, m_gaussMean, m_gaussStdDev );
	}
	shader->setFloatArray(_T("VBloomWeights"), m_bloomWeight, 9);
	shader->setFloatArray(_T("VBloomOffsets"), m_bloomOffset, 9);
	m_screen.set(0.0f, 0.0f, (float)rtt->getWidth(), (float)rtt->getHeight(), 0xffffffff);
	cRenderer::renderScreen(rtt, shader, &m_screen);
}
/*
 *	
 */
float cHDRPostProcess::ComputeGaussianValue( float x, float mean, float std_deviation )
{
    // The gaussian equation is defined as such:
    /*    
      -(x - mean)^2
      -------------
      1.0               2*std_dev^2
      f(x,mean,std_dev) = -------------------- * e^
      sqrt(2*pi*std_dev^2)
      
     */

    return ( 1.0f / sqrt( 2.0f * D3DX_PI * std_deviation * std_deviation ) )
        * expf( ( -( ( x - mean ) * ( x - mean ) ) ) / ( 2.0f * std_deviation * std_deviation ) );
}
/*
 *	
 */
int cHDRPostProcess::renderDebugInfo(int y)
{
	TCHAR str[NAME_LEN];
	_stprintf_s(str, NAME_LEN, _T("brightThreshold: %f"), m_brightThreshold);
	_getFontMgr()->drawD3DText(10, y+=16, str);
	_stprintf_s(str, NAME_LEN, _T("gaussMultiplier: %f"), m_gaussMultiplier);
	_getFontMgr()->drawD3DText(10, y+=16, str);
	_stprintf_s(str, NAME_LEN, _T("gaussMean: %f"), m_gaussMean);
	_getFontMgr()->drawD3DText(10, y+=16, str);
	_stprintf_s(str, NAME_LEN, _T("gauseStdDev: %f"), m_gaussStdDev);
	_getFontMgr()->drawD3DText(10, y+=16, str);

	return y;
}





/*
 *	
 */
cPostEffectHDR::cPostEffectHDR() : m_luminance(NULL), 
								   m_postProcess(NULL),
								   m_rttFinal(NULL)
{
}
/*
 *	
 */
cPostEffectHDR::~cPostEffectHDR()
{
	safeDelete(m_luminance);
	safeDelete(m_postProcess);
	safeDelete(m_rttFinal);
}
/*
 *	
 */
bool cPostEffectHDR::initialize(int luminanceRttNum, uint width, uint height)
{
	assert(!m_luminance);
	m_luminance = new cHDRLuminance;
	if (!m_luminance->initialize(luminanceRttNum, width, height))
		return false;

	assert(!m_postProcess);
	m_postProcess = new cHDRPostProcess;
	if (!m_postProcess->initialize(width, height))
		return false;

	m_rttFinal = new cRenderToTextureMSAA;
	if (!m_rttFinal->initialize(0, width, height))
		return false;

	m_screen.set(0.0f, 0.0f, (float)width, (float)height, 0xffffffff);
	m_exposure = 1.0f;//0.5f;

	return true;
}
/*
 *	
 */
void cPostEffectHDR::render(cTexture* originalTexture)
{
	m_luminance->render(originalTexture);
	m_postProcess->render(originalTexture);
	renderFinal(originalTexture);
}
/*
 *	
 */
void cPostEffectHDR::renderFinal(cTexture* originalTexture)
{
	cUberShader* shader = _getUberShaderMgr()->get(_T("hdr.fx"));
	if (!shader)
	{
		assert(shader);
		return ;
	}

	cRenderToTextureBase* rttLum = m_luminance->getRtt(0);
	cRenderToTextureBase* rttBlur = m_postProcess->getRtt(3);
	shader->setTechnique(_T("THDRFinal"));
	shader->setTexture(_T("originalTexture"), originalTexture);
	shader->setTexture(_T("lumTexture"), rttLum->getTexture());
	shader->setTexture(_T("bloomTexture"), rttBlur->getTexture());
	shader->setFloat(_T("g_rcp_bloom_tex_w"), 1.0f/(float)rttBlur->getWidth());
	shader->setFloat(_T("g_rcp_bloom_tex_h"), 1.0f/(float)rttBlur->getHeight());
	shader->setFloat(_T("fExposure"), m_exposure);
	shader->setFloat(_T("fGaussianScalar"), m_postProcess->getGaussMultiplier());
	cRenderer::renderScreen(m_rttFinal, shader, &m_screen);
}
/*
 *	
 */
int cPostEffectHDR::renderDebugInfo(int y)
{
	y += 16;

	TCHAR str[NAME_LEN];
	_getFontMgr()->drawD3DText(10, y+=16, _T("[HDR]"), cD3DFont::HEIGHT_14, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));

	_stprintf_s(str, NAME_LEN, _T("exposure: %f"), m_exposure);
	_getFontMgr()->drawD3DText(10, y+=16, str);

	y = m_postProcess->renderDebugInfo(y);

	return y;
}