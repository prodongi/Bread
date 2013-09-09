
#ifndef _BreadPostEffectSSAO_h_
#define _BreadPostEffectSSAO_h_

#include "BreadPostEffectBase.h"

namespace Bread
{
	class cTexture;
	/*
		@date 2010.00.00
		@auth prodongi
		@desc 
		@todo 
	*/
	class cPostEffectSSAO : public cPostEffectBase
	{
	public:
		cPostEffectSSAO();
		virtual ~cPostEffectSSAO();
		virtual void render(cRenderToTextureBase* rtt, cTexture* preSceneTexture, sUiPlane const* screen);
		virtual void renderDebugScreen();

	protected:
		virtual bool initShader();
		virtual bool initMember();
		void createRotTexture();

	private:
		cShader* m_ssaoShader;
		cRenderToTextureNAA* m_ssaoRtt;
		sUiPlane m_ssaoScreen;
	};
}



#endif