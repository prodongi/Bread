
#ifndef _BreadEnvironment_h_
#define _BreadEnvironment_h_

#include "BreadDeclaration.h"

#define _getEnv()	Bread::cEnvironment::getInstance()

namespace Bread
{
	class cFog;
	class cSkyBox;
	class cWeather;
	class cEngineConfig;
	class cCloud;

	/*
		@date 2011.07.11
		@auth prodongi
		@desc 
		@todo 
	*/
	class cEnvironment
	{
		declareInstance(cEnvironment)
		declareClassCreater(cEnvironment)

	public:
		virtual bool initialize(int skyBoxType, cEngineConfig const* config);
		virtual void finalize();
		virtual void update(float elapsedtime);
		virtual void renderBeforeScene();
		virtual void renderAfterScene();
		virtual int renderDebugInfo(int y);

		cFog* getFog() const			{	return m_fog;		}
		cSkyBox* getSkyBox() const		{	return m_skyBox;	}
		cWeather* getWeather() const	{	return m_weather;	}
		bool isRenderSkyBox() const;
		void setWeatherType(int type);

	protected:
		cFog* m_fog;
		cSkyBox* m_skyBox;
		cWeather* m_weather;
		cCloud* m_cloud;
	};
}



#endif