
#include "stdafx.h"
#include "PostEffectGammaCorrection.h"
#include "BreadRenderPropertyManager.h"
#include "BreadTexture.h"
#include "Bread2DTexture.h"

/*
 *	
 */
cPostEffectGammaCorrection::cPostEffectGammaCorrection() : m_rttLuminance(NULL),
														   m_shader(NULL),
														   m_gammaRampTexture(NULL)
{
}
/*
 *	
 */
cPostEffectGammaCorrection::~cPostEffectGammaCorrection()
{
	safeDelete(m_rttLuminance);
	safeDelete(m_shader);
	safeDelete(m_gammaRampTexture);
}
/*
 *	
 */
bool cPostEffectGammaCorrection::initialize(uint width, uint height)
{
	m_min = 0.0f;
	m_max = 1.0f;
	m_avg = 2.5f;
	m_darkAdjustDt = 0.15f;
	m_lightAdjustDt = 0.2f;
	m_contrastAdjustDt = 0.07f;
	m_bias = 0.8f;
	m_darkClamp = 0.3f;
	m_lightClamp = 0.4f;

	m_uiDebug.set(800.0f, 128.0f, 128.0f, 128.0f, 0xffffffff);
	m_uiCorrection.set(0.0f, 0.0f, (float)width, (float)height, 0xffffffff);

	if (!initRtt(width, height)) return false;
	if (!initShader()) return false;
	if (!initTexture()) return false;

	return true;
}
/*
 *	
 */
bool cPostEffectGammaCorrection::initRtt(uint width, uint height)
{
	assert(!m_rttLuminance);
	uint w = width/10;
	uint h = height/10;
	m_rttLuminance = new cRenderToTextureMSAA;
	if (!m_rttLuminance->initialize(0, w, h, false, D3DFMT_A8R8G8B8, TRUE))
		return false;

	m_uiLuminance.set(0.0f, 0.0f, (float)w, (float)h, 0xffffffff);

	return true;
}
/*
 *	
 */
bool cPostEffectGammaCorrection::initShader()
{
	assert(!m_shader);
	m_shader = new cGammaCorrectionShader;
	if (!m_shader->createEffect(_T("gammacorrection.fx"), cD3DSystem::getD3DDevice(), cShader::makeFlags()))
		return false;

	return true;
}
/*
 *	
 */
bool cPostEffectGammaCorrection::initTexture()
{
	// 64가 바뀌면 m_gammaRampTexture lock하는 부분 수정해야 된다
	m_gammaRampTexWidth = 64;
	m_gammaRampTexHeight = 64;
	
	m_gammaRampTexture = new cTexture;
	if (!m_gammaRampTexture->createTexture(TEXTURE_TYPE_2D, m_gammaRampTexWidth, m_gammaRampTexHeight, D3DFMT_A8R8G8B8))
		return false;
	
	return true;
}
/*
 *	
 */
void cPostEffectGammaCorrection::render(cTexture* originalTexture)
{
	renderLuminance(originalTexture);
	renderCorrection(originalTexture);
	renderScreen();
}
/*
 *	
 */
void cPostEffectGammaCorrection::renderLuminance(cTexture* originalTexture)
{
	m_shader->setTexture(m_shader->m_baseTex, originalTexture);
	m_shader->setTechnique(m_shader->m_tluminance);
	cRenderer::renderScreen(m_rttLuminance, m_shader, &m_uiLuminance);
}
/*
 *	
 */
void cPostEffectGammaCorrection::renderCorrection(cTexture* originalTexture)
{
	_getRenderPropertyMgr()->begin();
	_getRenderPropertyMgr()->setProperty(sRenderProperty::CULLMODE, D3DCULL_CCW);

	m_shader->setTexture(m_shader->m_baseTex, originalTexture);
	m_shader->setTexture(m_shader->m_gammaRampTex, m_gammaRampTexture);
	m_shader->setTechnique(m_shader->m_tcorrection);
	cRenderer::renderScreen(m_shader, &m_uiCorrection);

	_getRenderPropertyMgr()->end();
}
/*
 *	
 */
void cPostEffectGammaCorrection::renderScreen()
{
	m_uiDebug.set(800.0f, 128.0f, 128.0f, 128.0f, 0xffffffff);
	cRenderer::renderScreen(&m_uiDebug, m_rttLuminance->getTexture());

	m_uiDebug.set(800.0f, 256.0f, 128.0f, 128.0f, 0xffffffff);
	cRenderer::renderScreen(&m_uiDebug, m_gammaRampTexture);

	renderHistogram(10.0f, 500.0f, 30000.0f, m_histogram.m_lum);
	renderHistogram(300.0f, 500.0f, 1.0f, m_gammaRamp);
}
/*
 *	
 */
void cPostEffectGammaCorrection::update(float elapsedtime)
{
	// 최적화를 위해서 3개의 함수를 순차적으로 실행해도 된다.
	buildNormalizedHistogram();
	updateGammaValue(elapsedtime);
	setGammaRamp();
}
/*
 *	
 */
void cPostEffectGammaCorrection::buildNormalizedHistogram()
{
	// texture to histogram
	D3DSURFACE_DESC desc;
	D3DLOCKED_RECT lockRect;
	if (!m_rttLuminance->lockRect(&desc, &lockRect))
		return ;

	for (uint y = 0; y < desc.Height; ++y)
	{
		for (uint x = 0; x < desc.Width; ++x)
		{
			BYTE index = ((BYTE*)lockRect.pBits)[y*lockRect.Pitch + 4*x];
			m_histogram.m_lum[index] += 1.0f;
		}
	}

	if (!m_rttLuminance->unlockRect())
		return ;

	uint i;
	// normalize histogram
	float sum = 0;
	for (i = 0; i < 256; ++i)
		sum += m_histogram.m_lum[i];
	float invSum = cMath::inv(sum);
	for (i = 0; i < 256; ++i)
		m_histogram.m_lum[i] *= invSum;

	float threshold;
	// find min
	threshold = 0.05f;
	sum = 0;
	for (i = 0; i < 256; ++i)
	{
		sum += m_histogram.m_lum[i];
		if (sum >= threshold)
		{
			m_histogram.m_min = (float)i/255.0f;
			break;
		}
	}
	// find max
	threshold = 0.95f;
	sum = 0;
	for (i = 0; i < 256; ++i)
	{
		sum += m_histogram.m_lum[i];
		if (sum >= threshold)
		{
			m_histogram.m_max = (float)i/255.0f;
			break;
		}
	}
	// find average
	for (i = 0; i < 256; ++i)
	{
		m_histogram.m_avg += (float)i * m_histogram.m_lum[i];
	}
	m_histogram.m_avg /= 256.0f;
}
/*
 *	
 */
void cPostEffectGammaCorrection::updateGammaValue(float elapsedtime)
{
	const float noChangeLumThreshold = 0.01f;
	const float noChangeContThreshold = 0.01f;

	if (m_histogram.m_min < m_min - noChangeLumThreshold)
		m_min -= m_darkAdjustDt * elapsedtime;
	else if (m_histogram.m_min > m_min + noChangeLumThreshold)
		m_min += m_lightAdjustDt * elapsedtime;

	if (m_histogram.m_max < m_max - noChangeLumThreshold)
		m_max -= m_darkAdjustDt * elapsedtime;
	else if (m_histogram.m_max > m_max + noChangeLumThreshold)
		m_max += m_lightAdjustDt * elapsedtime;

	if (m_histogram.m_avg < m_avg - noChangeContThreshold)
		m_avg -= m_contrastAdjustDt * elapsedtime;
	else if (m_histogram.m_avg > m_min + noChangeLumThreshold)
		m_avg += m_contrastAdjustDt * elapsedtime;

	clamp(m_min, 0.0f, m_lightClamp);
	clamp(m_max, m_darkClamp, 1.0f);
	clamp(m_avg, m_min, m_max);
}
/*
 *	
 */
void cPostEffectGammaCorrection::setGammaRamp()
{
	if (!m_gammaRampTexture)
		return ;

	float dist = (m_avg - m_min)/(m_max - m_min);
	float p = 1.0f + dist * m_bias;

	uint i;
	for (i = 0; i < 256; ++i)
	{
		float x = (float)i/255.0f;
		float y;
		if (x <= m_min)			y = 0.0f;
		else if (x >= m_max)	y = 1.0f;
		else
		{
			y = powf((x - m_min)/(m_max-m_min), p);
			clamp(y, 0.0f, 1.0f);
		}
		m_gammaRamp[i] = BYTE(y*255.0f + 0.5f);
	}

	sTextureLockRect lockRect;
	if (!m_gammaRampTexture->lock(lockRect))
		return ;

	BYTE* bits = (BYTE*)lockRect.m_data;
	for (uint y = 0; y < m_gammaRampTexHeight; ++y)
	{
		BYTE b = m_gammaRamp[y*4];
		for (uint x = 0; x < m_gammaRampTexWidth; ++x)
		{
			BYTE g = m_gammaRamp[x*4];
			*( (DWORD*)(bits + lockRect.m_pitch * y + 4 * x ) ) = (g<<24)|(b<<16)|(g<<8)|(b<<0);
		}
	}

	m_gammaRampTexture->unlock();
}
