
#include "BreadEnvironment.h"
#include "BreadFog.h"
#include "BreadSkyBox.h"
#include "BreadWeather.h"
#include "BreadCloud.h"

namespace Bread
{
	implementInstance(cEnvironment)

	/*
	*/
	cEnvironment::cEnvironment() : m_weather(NULL), 
								   m_skyBox(NULL), 
								   m_fog(NULL),
								   m_cloud(NULL)
	{
		assignInstance(this)
	}
	/*
	*/
	cEnvironment::~cEnvironment()
	{
		assignInstance(NULL)
	}
	/*
	*/
	bool cEnvironment::initialize(int skyBoxType, cEngineConfig const* config)
	{
		m_skyBox = new cSkyBox;
		if (!m_skyBox->initialize(skyBoxType))
			return false;

		m_fog = new cFog;
		m_fog->initialize(config);

		m_weather = new cWeather;
		if (!m_weather->initialize(config))
			return false;

		m_cloud = new cCloud;
		if (!m_cloud->initialize())
			return false;

		return true;
	}
	/*
	*/
	void cEnvironment::finalize()
	{
		safeDelete(m_skyBox, true);
		safeDelete(m_fog);
		safeDelete(m_weather, true);
		safeDelete(m_cloud, true);
	}
	/*
	*/
	bool cEnvironment::isRenderSkyBox() const
	{
		return m_skyBox->getIsRender();
	}
	/*
	*/
	void cEnvironment::setWeatherType(int type)
	{
		m_weather->setType(type);
	}
	/*
	*/
	void cEnvironment::update(float elapsedtime)
	{
		if (m_weather)
			m_weather->update(elapsedtime);
	}
	/*
	*/
	void cEnvironment::renderBeforeScene()
	{
		if (m_skyBox)
			m_skyBox->render();
	}
	/*
	*/
	void cEnvironment::renderAfterScene()
	{
		if (m_weather)
			m_weather->render();
	}
	/*
	*/
	int cEnvironment::renderDebugInfo(int y)
	{
		return y;
	}
}