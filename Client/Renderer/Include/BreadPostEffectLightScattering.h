
#ifndef _BreadPostEffectLightScattering_h_
#define _BreadPostEffectLightScattering_h_

#include "BreadPostEffectBase.h"
#include "BreadVector2.h"
#include "BreadVector4.h"

namespace Bread
{
	class cRenderToTextureBase;

	/*
		@date 2011.10.31
		@auth prodongi
		@desc gpu gem3 chapter13 ÂüÁ¶
		@todo 
	*/
	class cPostEffectLightScattering : public cPostEffectBase
	{
	public:
		cPostEffectLightScattering();
		virtual ~cPostEffectLightScattering();
		virtual void update(float elapsedtime);
		virtual void render(cRenderToTextureBase* rtt, cTexture* preSceneTexture, sUiPlane const* screen);
		virtual void renderDebugScreen();

	protected:
		virtual bool initShader();
		bool initParams();
		void renderSun();
		void updateProjectionSunPos();

	protected:
		float m_density;
		float m_weight;
		float m_exposure;
		float m_decay;

		sUiPlane m_sunScreen;
		sVector2 m_sunSize;
		sVector4 m_sunPos;

		cRenderToTextureBase* m_rttSun;
		cShader* m_noTexShader;
	};
}








#endif