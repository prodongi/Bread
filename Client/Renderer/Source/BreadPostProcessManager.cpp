
#include "BreadPostProcessManager.h"
#include "BreadRenderer.h"
#include "BreadHelperClass.h"
#include "BreadPostEffectMonoChrome.h"
#include "BreadPostEffectSepia.h"
#include "BreadPostEffectInverse.h"
#include "BreadPostEffectGBlurH.h"
#include "BreadPostEffectGBlurV.h"
#include "BreadPostEffectBloomH.h"
#include "BreadPostEffectBloomV.h"
#include "BreadPostEffectScaleOri.h"
#include "BreadPostEffectScaleHalf.h"
#include "BreadPostEffectScaleQuart.h"
#include "BreadPostEffectBrightPass.h"
#include "BreadPostEffectToneMap.h"
#include "BreadPostEffectEdgeDetect.h"
#include "BreadPostEffectNoShader.h"
#include "BreadPostEffectNormalEdgeDetect.h"
#include "BreadPostEffectSSAO.h"
#include "BreadPostEffectDOF.h"
#include "BreadPostEffectLightScattering.h"
#include "BreadPostEffectShaderList.h"
#include "BreadPostEffectDecal.h"
#include "BreadShaderList.h"
#include "BreadGui.h"
#include "BreadDebugRenderManager.h"

namespace Bread
{
	/*
	 *	
	 */
	cPostProcessManager::cPostProcessManager() : m_rttScene(NULL),
												 m_rttIn(NULL),
												 m_rttOut(NULL)
	{
		int i;
		for (i = 0; i < RTT_SCALE_NUM; ++i)
		{
			for (int j = 0; j < RTT_NUM; ++j)
				m_rtt[i][j] = NULL;
		}

		for (i = 0; i < HELPER_NUM; ++i)
		{
			m_helper[i] = NULL;
		}
	}
	/*
	 *	
	 */
	cPostProcessManager::~cPostProcessManager()
	{
	}
	/*
	 *	
	 */
	bool cPostProcessManager::initialize(uint width, uint height)
	{
		if (!initMember())
			return false;
		if (!initRtt(width, height))
			return false;
		if (!initHelper(width, height))
			return false;
		if (!initPostEffect(width, height))
			return false;
		initUiScreen(width, height);

		return true;
	}
	/*
	 *	
	 */
	bool cPostProcessManager::initRtt(uint width, uint height)
	{
		assert(!m_rttScene);
		m_rttScene = new cRenderToTextureNAA;
		if (!m_rttScene->initialize(0, width, height))
			return false;

		for (int i = 0; i < RTT_NUM; ++i)
		{
			m_rtt[RTT_SCALE_ORI][i] = new cRenderToTextureNAA;
			if (!m_rtt[RTT_SCALE_ORI][i]->initialize(0, width, height))
				return false;

			m_rtt[RTT_SCALE_HALF][i] = new cRenderToTextureNAA;
			if (!m_rtt[RTT_SCALE_HALF][i]->initialize(0, width>>1, height>>1))
				return false;

			m_rtt[RTT_SCALE_QUART][i] = new cRenderToTextureNAA;
			if (!m_rtt[RTT_SCALE_QUART][i]->initialize(0, width>>2, height>>2))
				return false;			
		}		

		return true;
	}
	/*
	*/
	bool cPostProcessManager::initHelper(uint width, uint height)
	{
		sHelper* helper;
		/// @brief normal
		helper = m_helper[HELPER_NORMAL];
		if (helper)
		{
			assert(!helper->m_rtt);
			helper->m_rtt = new cRenderToTextureNAA;
			if (!helper->m_rtt->initialize(0, width, height))
				return false;
		}

		/// @brief depth
		helper = m_helper[HELPER_DEPTH];
		if (helper)
		{
			assert(!helper->m_rtt);
			helper->m_rtt = new cRenderToTextureNAA;
			if (!helper->m_rtt->initialize(0, width, height, true, D3DFMT_R32F))
				return false;
		}

		return true;
	}
	/*
	 *	
	 */
	bool cPostProcessManager::initPostEffect(uint width, uint height)
	{
		cPostEffectBase* effect;

		for (uint i = 0; i < EFFECT_NUM; ++i)
		{
			effect = createPostEffect(i, width, height);
			if (effect)	
				pushBack(&effect);
		}

		return true;
	}
	/*
	 *	
	 */
	void cPostProcessManager::initUiScreen(uint width, uint height)
	{
		m_screenScene.set(0.0f, 0.0f, (float)width, (float)height);
		m_screenScale[RTT_SCALE_ORI].set(0.0f, 0.0f, (float)width, (float)height);
		m_screenScale[RTT_SCALE_HALF].set(0.0f, 0.0f, (float)(width>>1), (float)(height>>1));
		m_screenScale[RTT_SCALE_QUART].set(0.0f, 0.0f, (float)(width>>2), (float)(height>>2));
	}
	/*
	 *	
	 */
	bool cPostProcessManager::initMember()
	{
		sHelper* helper;
		/// @brief normal
		assert(!m_helper[HELPER_NORMAL]);
		helper = new sHelper;
		helper->m_shader = new cPostEffectNormalShader;
		if (!helper->m_shader->createEffect(_T("fx\\posteffectnormal.fx"), cD3DSystem::getD3DDevice(), cShader::makeFlags()))
		{
			safeDelete(helper);
			return false;
		}
		m_helper[HELPER_NORMAL] = helper;

		/// @brief depth
		assert(!m_helper[HELPER_DEPTH]);
		helper = new sHelper;
		helper->m_shader = new cDepthTextureShader;
		if (!helper->m_shader->createEffect(_T("fx\\depthtexture.fx"), cD3DSystem::getD3DDevice(), cShader::makeFlags()))
		{
			safeDelete(helper);
			return false;
		}
		helper->m_shader->setFloat(_T("farClip"), _getEngineConfig()->getZFar());
		m_helper[HELPER_DEPTH] = helper;

		return true;
	}
	/*
	 *	
	 */
	void cPostProcessManager::finalize()
	{
		int i;
		for (i = 0; i < HELPER_NUM; ++i)
		{
			safeDelete(m_helper[i]);
		}
		safeDelete(m_rttScene);
		for (i = 0; i < RTT_SCALE_NUM; ++i)
		{
			for (int j = 0; j < RTT_NUM; ++j)
				safeDelete(m_rtt[i][j]);
		}
		m_activeList.clear();
	}
	/*
	 *	
	 */
	void cPostProcessManager::addActive(cPostProcessManager::ACTIVATE type)
	{
		checkValidActive(type);

		cPostEffectBase* effect = get(type);
		if (!effect)
		{
			assert(0 && _T("invalid post process active index"));
			return ;
		}
		m_activeList.push_back(effect);
	}
	/*
	 *	
	 */
	void cPostProcessManager::checkValidActive(uint type)
	{
		bool warning = false;

		if (MONOCHROME == type)
		{
			if (isActiveType(SEPIA))	warning = true;
		}
		else if (SEPIA == type)
		{
			if (isActiveType(MONOCHROME))	warning = true;
		}
		else if (GBLUR_H == type)
		{
			if (isActiveType(BLOOM_H))	warning = true;
		}
		else if (GBLUR_V == type)
		{
			if (isActiveType(BLOOM_V))	warning = true;
		}
		else if (EDGE_DETECT == type)
		{
			if (isActiveType(NORMAL_EDGE_DETECT))	warning = true;
		}
		else if (NORMAL_EDGE_DETECT == type)
		{
			if (isActiveType(EDGE_DETECT))	warning = true;
		}

		assert(!warning && _T("invalid post process active list"));
	}
	/*
	 *	
	 */
	void cPostProcessManager::update(float elapsedtime)
	{
		std::for_each(m_activeList.begin(), m_activeList.end(), sUpdateFunctor<cPostEffectBase>(elapsedtime));
	}
	/*
	*/
	void cPostProcessManager::renderHelper()
	{
		for (int i = 0; i < HELPER_NUM; ++i)
		{
			renderHelper(m_helper[i]);
		}
	}
	/*
	*/
	void cPostProcessManager::beginRttScene(bool clear, DWORD color)
	{
		m_rttScene->begin(clear, color);
	}
	/*
	*/
	void cPostProcessManager::endRttScene()
	{
		m_rttScene->end();
	}
	/*
	 *	
	 */
	void cPostProcessManager::renderHelper(sHelper* helper)
	{
		if (!helper || !helper->m_is)
			return ;
		cRenderer::renderSingleScene(helper->m_rtt, 1, helper->m_shader, _T("T"), true);
	}
	/*
	 *	
	 */
	void cPostProcessManager::render2D()
	{
		_getRenderPropertyMgr()->begin();
		_getRenderPropertyMgr()->setProperty(sRenderProperty::ZENABLE, FALSE);

		renderActiveEffect();
		// swap을 했으므로 m_rttOut이 아니고 m_rttIn을 참조해야 된다.
		cRenderer::renderScreen(&m_screenScene, m_rttIn->getTexture());

		_getRenderPropertyMgr()->end();
	}
	/*
	 *	
	 */
	void cPostProcessManager::renderActiveEffect()
	{
		m_scaleType = RTT_SCALE_ORI;
		m_rttIn = m_rttScene;
		m_rttOut = m_rtt[m_scaleType][0];
		m_screenOut = &m_screenScale[m_scaleType];

		uint effectType;
		it_active it = m_activeList.begin();
		for (; it != m_activeList.end(); ++it)
		{
			cPostEffectBase* base = *it;
			effectType = base->getEffectType();
			findRttOut(effectType, base->getScaleType());
			base->render(m_rttOut, m_rttIn->getTexture(), m_screenOut);
			swapRttInOut();
			// swap 했으므로 다시 한번 찾는다.
			findRttOut(effectType);
		}
	}
	/*
	 *	
	 */
	int cPostProcessManager::renderDebugInfo(int y)
	{
		y += 16;

		TCHAR str[NAME_LEN];
		_getFontMgr()->drawD3DText(10, y+=16, _T("[PostProcess]"), cD3DFont::HEIGHT_14, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));

		uint count = 0;
		it_active it = m_activeList.begin();
		for (; it != m_activeList.end(); ++it, ++count)
		{
			if ((*it)->isShader())
				_stprintf_s(str, NAME_LEN, _T("%d: %s"), count, (*it)->getShaderBuid().c_str());
			else
				_stprintf_s(str, NAME_LEN, _T("%d: %d"), count, (*it)->getEffectType());
			_getFontMgr()->drawD3DText(10, y+=16, str);
		}

		return y;
	}
	/*
	 *	
	 */
	void cPostProcessManager::renderDebugScreen()
	{
		//if (m_helper[HELPER_DEPTH])
		//	_getDebugRenderMgr()->addScreen(140.0f, 550.0f, 128.0f, 128.0f, m_helper[HELPER_DEPTH]->m_rtt->getTexture());

		it_active it = m_activeList.begin();
		for (; it != m_activeList.end(); ++it)
		{
			(*it)->renderDebugScreen();
		}
	}
	/*
	 *	
	 */
	cPostEffectBase* cPostProcessManager::createPostEffect(uint type, uint width, uint height)
	{
		cPostEffectBase* effect = NULL;
		switch (type)
		{
		case MONOCHROME:				effect = new cPostEffectMonoChrome;			break;
		case SEPIA:						effect = new cPostEffectSepia;				break;
		case INVERSE:					effect = new cPostEffectInverse;			break;
		case GBLUR_H:					effect = new cPostEffectGBlurH;				break;
		case GBLUR_V:					effect = new cPostEffectGBlurV;				break;
		case BLOOM_H:					effect = new cPostEffectBloomH;				break;
		case BLOOM_V:					effect = new cPostEffectBloomV;				break;
		case SCALE_ORI:					effect = new cPostEffectScaleOri;			break;
		case SCALE_HALF:				effect = new cPostEffectScaleHalf;			break;
		case SCALE_QUART:				effect = new cPostEffectScaleQuart;			break;
		case BRIGHT_PASS:				effect = new cPostEffectBrightPass;			break;
		case TONE_MAP:					effect = new cPostEffectToneMap;			break;
		case EDGE_DETECT:				effect = new cPostEffectEdgeDetect;			break;
		case NORMAL_EDGE_DETECT:		effect = new cPostEffectNormalEdgeDetect;	break;
		case SSAO:						effect = new cPostEffectSSAO();				break;
		case DOF:						effect = new cPostEffectDOF(m_rttScene->getTexture());	break;
		case LIGHT_SCATTERING:			effect = new cPostEffectLightScattering;	break;
		case DECAL:						effect = new cPostEffectDecal;				break;
		default:			assert(0 && _T("invalid post effect type"));
		};

		if (!effect) return NULL;
		if (!effect->initialize(type, width, height))
		{
			assert(0 && _T("failed initialize post effect"));
			safeDelete(effect);
			return NULL;
		}
		return effect;
	}
	/*
	 *	
	 */
	void cPostProcessManager::swapRttInOut()
	{
		std::swap(m_rttIn, m_rttOut);
	}
	/*
	 *	
	 */
	void cPostProcessManager::findRttOut(uint type, int custumScaleType)
	{
		if (-1 != custumScaleType)
		{
			m_scaleType = custumScaleType;
		}
		else
		{
			if (SCALE_ORI != type && SCALE_HALF != type && SCALE_QUART != type)
				return ;

			if (SCALE_ORI == type)			m_scaleType = RTT_SCALE_ORI;
			else if (SCALE_HALF == type)	m_scaleType = RTT_SCALE_HALF;
			else if (SCALE_QUART == type)	m_scaleType = RTT_SCALE_QUART;
			else						return ;
		}

		m_screenOut = &m_screenScale[m_scaleType];
		m_rttOut = findNotEqualRtt(m_rttIn);
		assert(m_rttOut && _T("falid find out size"));
	}
	/*
	 *	
	 */
	bool cPostProcessManager::isActiveType(uint type)
	{
		it_active it = m_activeList.begin();
		for (; it != m_activeList.end(); ++it)
		{
			if (type == (*it)->getEffectType())
				return true;
		}
		return false;
	}
	/*
	 *	
	 */
	void cPostProcessManager::clearActive()
	{
		m_activeList.clear();
		for (int i = 0; i < HELPER_NUM; ++i)
		{
			m_helper[i]->m_is = false;
		}
	}
	/*
	*/
	cRenderToTextureBase* cPostProcessManager::findNotEqualRtt(cRenderToTextureBase* rtt)
	{
		for (int i = 0; i < RTT_NUM; ++i)
		{
			// m_rttIn이 아닌걸 찾아 m_rttOut에 설정한다
			if (m_rtt[m_scaleType][i] != rtt)
			{
				return m_rtt[m_scaleType][i];
			}
		}
		return NULL;
	}
	/*
	*/
	void cPostProcessManager::isHelperRender(cPostProcessManager::HELPER type, bool is)
	{
		if (HELPER_NUM <= type)
		{
			assert(0 && _T("invalid post effect helper type"));
			return ;
		}
		m_helper[type]->m_is = is;
	}
	/*
	*/
	cTexture* cPostProcessManager::getHelperTexture(cPostProcessManager::HELPER type) const
	{
		if (HELPER_NUM <= type)
		{
			assert(0 && _T("failed getHelperTexture : invalid post effect helper type"));
			return NULL;
		}
		return m_helper[type]->m_rtt->getTexture();
	}
}