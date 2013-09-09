
#ifndef _BreadPostEffectBase_h_
#define _BreadPostEffectBase_h_

#include "BreadShader.h"
#include "BreadD3DSystemClassify.h"
#include "BreadUiPrimitive.h"
#include "BreadRenderToTexture.h"

namespace Bread
{
	class cTexture;

	/**
	 * \ingroup Renderer
	 *
	 *
	 * \par requirements
	 * win98 or later\n
	 * win2k or later\n
	 * MFC\n
	 *
	 * \version 1.0
	 * first version
	 *
	 * \date 2010-03-22
	 *
	 * \author Prodongi
	 *
	 * \par license
	 * This code is absolutely free to use and modify. The code is provided "as is" with
	 * no expressed or implied warranty. The author accepts no liability if it causes
	 * any damage to your computer, causes your pet to fall ill, increases baldness
	 * or makes your car start emitting strange noises when you start it up.
	 * This code has no bugs, just undocumented features!
	 * 
	 * \todo 
	 *
	 * \bug 
	 *
	 */
	class cPostEffectBase
	{
	public:
		cPostEffectBase();
		virtual ~cPostEffectBase();
		virtual bool initialize(uint effectType, uint width, uint height);
		virtual void update(float elapsedtime);
		virtual void render(cRenderToTextureBase* /*rtt*/, cTexture* /*preSceneTexture*/, sUiPlane const* /*screen*/) = 0;
		virtual void renderDebugScreen() {}
		BUID const& getShaderBuid() const	{	return m_shader->getBuid();	}
		bool isShader() const				{	return NULL != m_shader;	}

		propertyConstFunc(uint, EffectType, m_effectType)
		propertyConstFunc(int, ScaleType, m_scaleType);

	protected:
		virtual bool initShader() = 0;
		virtual void initShaderParam(uint /*width*/, uint /*height*/) {}
		virtual bool initMember() {	return true;	}
		template <class T>	bool createShader(std_string const& filename)
		{	
			return createShader<T>(&m_shader, filename);
		}
		template <class T>	bool createShader(cShader** shader, std_string const& filename)
		{	
			assert(!*shader);
			*shader = new T;
			if (!(*shader)->createEffect(filename, cD3DSystem::getD3DDevice(), cShader::makeFlags()))
			{
				safeDelete(*shader);
				return false;
			}
			return true;
		}
		template <class T> void renderShader(cRenderToTextureBase* rtt, cTexture* preSceneTexture, sUiPlane const* screen)
		{
			T* shader = (T*)m_shader;
			shader->setTechnique(shader->m_t);
			shader->setTexture(shader->m_sceneTexture, preSceneTexture);
			cRenderer::renderScreen(rtt, m_shader, screen);
		}

	protected:
		cShader* m_shader;
		uint m_effectType;
		/// @brief -1 이면 이전 post effect의 scale type을 유지한다
		int m_scaleType;
	};
}













#endif