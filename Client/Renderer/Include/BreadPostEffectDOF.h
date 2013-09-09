
#ifndef _BreadPostEffectDOF_h_
#define _BreadPostEffectDOF_h_

#include "BreadPostEffectBase.h"

namespace Bread
{
	/*
		@date 2011.10.31
		@auth prodongi
		@desc 
		@todo 
				필요한 post process
				cPostProcessManager::SCALE_HALF
				cPostProcessManager::GBLUR_H
				cPostProcessManager::GBLUR_V
				cPostProcessManager::DOF
				cPostProcessManager::SCALE_ORI
	*/
	class cPostEffectDOF : public cPostEffectBase
	{
	public:
		cPostEffectDOF() : m_oriSceneTexture(NULL) {}
		cPostEffectDOF(cTexture* oriSceneTexture);
		virtual ~cPostEffectDOF() {}
		virtual void initShaderParam(uint /*width*/, uint /*height*/);
		virtual void render(cRenderToTextureBase* rtt, cTexture* preSceneTexture, sUiPlane const* screen);
		virtual void update(float elapsedtime);

	protected:
		virtual bool initShader();

	protected:
		cTexture* m_oriSceneTexture;	/// 원본 텍스춰
		float m_focusDistance;
		float m_focusRange;
	};
}








#endif