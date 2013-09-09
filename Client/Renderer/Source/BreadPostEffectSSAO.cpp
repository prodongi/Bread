
#include "BreadPostEffectSSAO.h"
#include "BreadPostEffectShaderList.h"
#include "BreadShaderList.h"
#include "BreadRenderer.h"
#include "BreadDebugRenderManager.h"
#include "BreadTextureManager.h"
#include "Bread2DTexture.h"
#include "BreadPostProcessManager.h"

namespace Bread
{
	/*
	*/
	cPostEffectSSAO::cPostEffectSSAO() : m_ssaoShader(NULL), m_ssaoRtt(NULL)
	{
	}
	/*
	*/
	cPostEffectSSAO::~cPostEffectSSAO()
	{
		safeDelete(m_ssaoShader);
		safeDelete(m_ssaoRtt);
	}
	/*
	*/
	bool cPostEffectSSAO::initShader()
	{
		/// ssao
		if (!createShader<cSSAOShader>(&m_ssaoShader, _T("ssao.fx")))
			return false;

		/// 
		if (!createShader<cPostEffectSSAOShader>(_T("posteffectssao.fx")))
			return false;

		return true;
	}
	/*
	*/
	bool cPostEffectSSAO::initMember()
	{
		createRotTexture();

		assert(!m_ssaoRtt);
		m_ssaoRtt = new cRenderToTextureNAA;
		if (!m_ssaoRtt->initialize(0, 512, 512, false)) return false;
		m_ssaoScreen.set(0.0f, 0.0f, 512.0f, 512.0f);

		/// 
		m_shader->setTexture(_T("ssaoTexture"), m_ssaoRtt->getTexture());

		m_ssaoShader->setTexture(_T("depthTexture"), _getPostProcessMgr()->getHelperTexture(cPostProcessManager::HELPER_DEPTH));
		m_ssaoShader->setTexture(_T("rotTexture"), _getTextureMgr()->get(_T("ssaoRotTexture")));

		m_shader->setFloat(_T("textureSize"), 512.0f);

		return true;
	}
	/*
	*/
	void cPostEffectSSAO::createRotTexture()
	{
		cTexture* tex = _getTextureMgr()->createTexture(_T("ssaoRotTexture"), TEXTURE_TYPE_2D, 4, 4, D3DFMT_A8R8G8B8);
		tex->getData()->makeRandomleyRotateData(tex->getWidth(), tex->getHeight());
	}
	/*
	 *	
	 */
	void cPostEffectSSAO::render(cRenderToTextureBase* rtt, cTexture* preSceneTexture, sUiPlane const* screen)
	{
		m_ssaoShader->setFloat(_T("farDistance"), _getEngineConfig()->getZFar());
		cRenderer::renderScreen(m_ssaoRtt, m_ssaoShader, &m_ssaoScreen, true);

		m_shader->setTexture(_T("ssaoTexture"), m_ssaoRtt->getTexture());
		cPostEffectBase::renderShader<cPostEffectSSAOShader>(rtt, preSceneTexture, screen);
	}
	/*
	*/
	void cPostEffectSSAO::renderDebugScreen()
	{
		_getDebugRenderMgr()->addScreen(140.0f, 400.0f, 128.0f, 128.0f, m_ssaoRtt->getTexture());
	}
}