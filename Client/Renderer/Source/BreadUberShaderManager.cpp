
#include "BreadUberShaderManager.h"
#include "BreadWaterUberShader.h"
#include "BreadCloudUberShader.h"
#include "BreadUberShaderFragment.h"
#include "BreadUtils.h"
#include "BreadLightManager.h"
#include "BreadCameraManager.h"
#include "BreadEnvironment.h"
#include "BreadFog.h"
#include "BreadStringHelper.h"
#include "BreadFileSystem.h"
#include "BreadFile.h"

namespace Bread
{
	struct sUpdateUberShader
	{
		sUpdateUberShader(cLight* light, cCamera* cam, float elapsedtime)
		{
			m_light = light;
			m_cam = cam;
			m_elapsedtime = elapsedtime;
		}
		void operator ()(cUberShader* shader)
		{
			assert(shader);

			shader->update(m_elapsedtime, NULL, NULL, NULL);

			/// update shared value
			if (m_light)
			{
				shader->setVector(_T("lightDir"), &m_light->getDirection());
			}
			if (m_cam)
			{
				shader->setVector(_T("eyePos"), &m_cam->getEye());
				shader->setMatrix(_T("matVP"), &m_cam->getVP());
			}
			if (_getEnv()->getFog()->getEnable())
			{
				shader->setFloatArray(_T("fog"), _getEnv()->getFog()->getDensity(), 2);
			}
		}
		cLight* m_light;
		cCamera* m_cam;
		float m_elapsedtime;
	};

	implementInstance(cUberShaderManager)

	/*
	 *	
	 */
	 cUberShaderManager::cUberShaderManager() : m_pool(NULL), 
												m_fragment(NULL),
												m_baseShader(NULL)
	{
		assignInstance(this)
	}
	/*
	 *	
	 */
	cUberShaderManager::~cUberShaderManager()
	{
		assignInstance(NULL)
		safeRelease(m_pool);
		safeDelete(m_fragment);
	}
	/*
	*	
	*/
	bool cUberShaderManager::initialize()
	{
		m_cs.initialize();
		registerObjectCallBack();
		if (!loadSourceEffectData())
			return false;
		createFragment();
		if (!checkHResultReturn(D3DXCreateEffectPool(&m_pool)))
			return false;
		return true;
	}
	/*
	*/
	void cUberShaderManager::createFragment()
	{
		m_fragment = new cUberShaderFragment;
		m_fragment->initialize();
	}
	/*
	 *	
	 */
	void cUberShaderManager::update(float elapsedtime)
	{
		cLight* light = _getLightMgr()->get(0);
		cCamera* cam = _getCameraMgr()->getMain();
		forEach(sUpdateUberShader(light, cam, elapsedtime));
	}
	/*
	*/
	void cUberShaderManager::registerObjectCallBack()
	{
		m_objFactory.registerCallBack(UBERSHADER_DEFAULT, createUberShader);
		m_objFactory.registerCallBack(UBERSHADER_WATER, createWaterUberShader);
		m_objFactory.registerCallBack(UBERSHADER_CLOUD, createCloudUberShader);
	}	
	/*
	 *	
	 */
	bool cUberShaderManager::loadSourceEffectData()
	{
		if (!loadSourceEffectData(UBERSHADER_DEFAULT, _T("ubershader.fx")))
			return false;
		if (!loadSourceEffectData(UBERSHADER_WATER, _T("waterubershader.fx")))
			return false;
		if (!loadSourceEffectData(UBERSHADER_CLOUD, _T("cloudubershader.fx")))
			return false;

		return true;
	}
	/*
	*/
	bool cUberShaderManager::loadSourceEffectData(TCHAR const* type, TCHAR const* filename)
	{
		std::string effectData;

		cFile* file = _getFileSystem()->open(filename);
		if (!file)
			return false;

		char str[MAX_PATH];
		while (!file->eof())
		{
			file->getLine(str, MAX_PATH);
			effectData += str;
			effectData += "\n";
		}

		safeDelete(file);

		m_srcEffectDatas.insert(type, effectData);

		return true;
	}
	/*
	*/
	void cUberShaderManager::createBaseShader()
	{
		m_baseShader = createShader(UBERSHADER_DEFAULT, FRAGMENT_BASE);
	}
	/*
	*/
	cUberShader* cUberShaderManager::createShader(std_string const& type, uint uberFragments)
	{
		lock();

		BUID buid = cUberShader::makeBUID(type, uberFragments);
		cUberShader* shader = get(buid);
		if (shader)
		{
			unlock();
			return shader;
		}

		shader = m_objFactory.createObject(type);
		if (!shader)
			return NULL;
		if (!shader->createEffect(type, m_srcEffectDatas.get(type), cD3DSystem::getD3DDevice(), cShader::makeFlags(), uberFragments, m_pool))
		{
			unlock();
			safeDelete(shader);
			return NULL;
		}
		insert(shader->getBuid(), &shader);

		unlock();
		return shader;
	}
	/*
	*/
	cUberShader* cUberShaderManager::createShader(std_string const& type, std_string const& uberFragments)
	{
		uint fragments = m_fragment->stringToFragment(uberFragments.c_str());
		return createShader(type, fragments);
	}
	/*
	*/
	cUberShader* cUberShaderManager::getBase() const
	{
		return m_baseShader;
	}
}















