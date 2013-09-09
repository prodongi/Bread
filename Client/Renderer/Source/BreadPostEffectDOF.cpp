
#include "BreadPostEffectDOF.h"
#include "BreadPostEffectShaderList.h"
#include "BreadRenderer.h"
#include "BreadGui.h"
#include "BreadPostProcessManager.h"

namespace Bread
{
	/*
	*/
	cPostEffectDOF::cPostEffectDOF(cTexture* oriSceneTexture) : 	m_oriSceneTexture(oriSceneTexture)
	{
		setScaleType(0);	/// original size
	}
	/*
	*/
	bool cPostEffectDOF::initShader()
	{
		if (!createShader<cPostEffectDOFShader>(_T("posteffectdof.fx")))
			return false;
		return true;
	}
	/*
	 */
	void cPostEffectDOF::initShaderParam(uint /*width*/, uint /*height*/)
	{
		m_focusDistance = 1000.0f;
		m_focusRange = 500.0f;

		m_shader->setFloat(_T("focusDistance"), m_focusDistance);
		m_shader->setFloat(_T("focusRange"), m_focusRange);
		m_shader->setFloat(_T("farClip"), _getEngineConfig()->getZFar());
		m_shader->setTexture(_T("depthTexture"), _getPostProcessMgr()->getHelperTexture(cPostProcessManager::HELPER_DEPTH));
		m_shader->setTexture(_T("oriSceneTexture"), m_oriSceneTexture);
	}
	/*
	*/
	void cPostEffectDOF::update(float /*elapsedtime*/)
	{
		if (_getInputSystem()->isPressedKey(DIK_N))
		{
			m_focusDistance += 100.0f;
			if (_getEngineConfig()->getZFar() <  m_focusDistance) m_focusDistance = _getEngineConfig()->getZFar();
			m_shader->setFloat(_T("focusDistance"), m_focusDistance);
			trace(_T("focus distance = %f\n"), m_focusDistance);
		}
		else if (_getInputSystem()->isPressedKey(DIK_M))
		{
			m_focusDistance -= 100.0f;
			if (0 > m_focusDistance) m_focusDistance = 0.0f;
			m_shader->setFloat(_T("focusDistance"), m_focusDistance);
			trace(_T("focus distance = %f\n"), m_focusDistance);
		}
	}
	/*
	 *	
	 */
	void cPostEffectDOF::render(cRenderToTextureBase* rtt, cTexture* preSceneTexture, sUiPlane const* screen)
	{
		cPostEffectBase::renderShader<cPostEffectDOFShader>(rtt, preSceneTexture, screen);
	}
}