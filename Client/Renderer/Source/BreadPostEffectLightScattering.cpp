
#include "BreadPostEffectLightScattering.h"
#include "BreadPostEffectShaderList.h"
#include "BreadShaderList.h"
#include "BreadRenderer.h"
#include "BreadSceneManager.h"
#include "BreadTextureManager.h"
#include "BreadDebugRenderManager.h"
#include "BreadGui.h"
#include "BreadCameraManager.h"

namespace Bread
{
	/*
	*/
	cPostEffectLightScattering::cPostEffectLightScattering() : m_noTexShader(NULL),
															   m_rttSun(NULL)
	{
	}
	/*
	*/
	cPostEffectLightScattering::~cPostEffectLightScattering()
	{
		safeDelete(m_noTexShader);
		safeDelete(m_rttSun);
	}
	/*
	*/
	bool cPostEffectLightScattering::initShader()
	{
		if (!createShader<cPostEffectLightScatteringShader>(_T("posteffectlightscattering.fx")))
			return false;
		
		if (!initParams())
			return false;

		return true;
	}
	/*
	*/
	bool cPostEffectLightScattering::initParams()
	{
		/// create notexture shader
		m_noTexShader = new cNoTextureShader;
		if (!m_noTexShader->createEffect(_T("notexture.fx"), cD3DSystem::getD3DDevice(), cShader::makeFlags()))
			return false;
		sVector3 color(0.0f, 0.0f, 0.0f);
		m_noTexShader->setVector(_T("color"), &color);
				
		/// create sun tetxture
		if (!_getTextureMgr()->loadTexture(_T("sun.dds"), TEXTURE_TYPE_2D, LOADING_DIRECT))
			return false;

		int w = _getEngineConfig()->getWidth();
		int h = _getEngineConfig()->getHeight();

		m_rttSun = new cRenderToTextureNAA;
		if (!m_rttSun->initialize(0, w, h, false))
			return false;

		m_sunPos = sVector4(100.0f, 100.0f, 100.0f, 1.0f);
		m_sunSize = sVector2(200.0f, 200.0f);

		m_density = 0.36f;
		m_weight = 5.65f;
		m_exposure = 0.011f;
		m_decay = 1.0f;
		m_shader->setFloat(_T("density"), m_density);
		m_shader->setFloat(_T("weight"), m_weight);
		m_shader->setFloat(_T("exposure"), m_exposure);
		m_shader->setFloat(_T("decay"), m_decay);
		m_shader->setTexture(_T("samplingTexture"), m_rttSun->getTexture());

		return true;
	}
	/*
	*/
	void cPostEffectLightScattering::update(float /*elapsedtime*/)
	{
		updateProjectionSunPos();

		/*
		float offset;
		if (_getInputSystem()->isPressedKey(DIK_Q))		offset = 1.0f;
		else											offset = 0.001f;

		if (_getInputSystem()->isPressedKey(DIK_1))	m_density += offset;
		else if (_getInputSystem()->isPressedKey(DIK_2)) m_density -= offset;
		m_density = max(0.0f, m_density);

		if (_getInputSystem()->isPressedKey(DIK_3))	m_weight += offset;
		else if (_getInputSystem()->isPressedKey(DIK_4)) m_weight -= offset;

		if (_getInputSystem()->isPressedKey(DIK_5))	m_exposure += 0.001f;
		else if (_getInputSystem()->isPressedKey(DIK_6)) m_exposure -= 0.001f;

		if (_getInputSystem()->isPressedKey(DIK_7))	m_decay += offset;
		else if (_getInputSystem()->isPressedKey(DIK_8)) m_decay -= offset;

		// ÃÊ±âÈ­
		if (_getInputSystem()->isPressedKey(DIK_W))
		{
			m_density = 0.84f;
			m_weight = 5.65f;
			m_exposure = 0.0034f;
			m_decay = 1.0f;
		}
		*/
	}
	/*
	*/
	void cPostEffectLightScattering::updateProjectionSunPos()
	{
		cCamera* cam = _getCameraMgr()->getMain();

		sVector4 sunPos;
		sMatrix4 matWVP;
		mulMat4(cam->getWorld(), cam->getVP(), matWVP);
		matWVP.transform(m_sunPos, sunPos);
		sunPos.x /= sunPos.w;
		sunPos.y /= sunPos.w;
		
		sVector2 p;
		p.x = sunPos.x * 0.5f + 0.5f;
		p.y = -1.0f * (sunPos.y * 0.5f) + 0.5f;
		m_shader->setFloatArray(_T("screenLightPos"), &p[0], 2);

		p.x *= (float)_getEngineConfig()->getWidth();
		p.y *= (float)_getEngineConfig()->getHeight();
		m_sunScreen.set(p.x-m_sunSize.x/2.0f, p.y-m_sunSize.y/2.0f, m_sunSize.x, m_sunSize.y, 0xffffffff);
	}
	/*
	 *	
	 */
	void cPostEffectLightScattering::render(cRenderToTextureBase* rtt, cTexture* preSceneTexture, sUiPlane const* screen)
	{
		// render black
		renderSun();

		// render sampling
		cPostEffectBase::renderShader<cPostEffectLightScatteringShader>(rtt, preSceneTexture, screen);
	}
	/*
	*/
	void cPostEffectLightScattering::renderSun()
	{
		_getRenderPropertyMgr()->begin();
		_getRenderPropertyMgr()->setProperty(sRenderProperty::ZWRITEENABLE, FALSE);
		m_rttSun->begin(true, D3DCOLOR_ARGB(255, 0, 0, 0));

		cTexture* texSun = _getTextureMgr()->get(_T("sun.dds"));
		cRenderer::renderScreen(&m_sunScreen, texSun);

		_getSceneMgr()->renderSingleShader(m_noTexShader, _T("T"), cBatchRenderManager::MASK_SCENE);
		
		m_rttSun->end();
		_getRenderPropertyMgr()->end();
	}
	/*
	*/
	void cPostEffectLightScattering::renderDebugScreen()
	{
		//_getDebugRenderMgr()->addScreen(140.0f, 400.0f, 128.0f, 128.0f, m_rttSun->getTexture());
	}
}