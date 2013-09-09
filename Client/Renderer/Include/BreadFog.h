
#ifndef _BreadFog_h_
#define _BreadFog_h_

#include "BreadClientDefinition.h"

namespace Bread
{
	class cEngineConfig;

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
	 * \date 2010-03-25
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
	class cFog
	{
	public:
		cFog();
		virtual ~cFog();
		virtual void initialize(cEngineConfig const* config);
		virtual void update(float elapsedtime);
		virtual int renderDebugInfo(int y);
		bool getEnable() const		{	return m_enable;	}
		float getZNear() const		{	return m_zNear;		}
		float getZFar() const		{	return m_zFar;		}
		float const* getDensity() const	{	return m_density;	}
		
	private:
		void calculDensity();

	private:
		bool m_enable;
		float m_zNear;
		float m_zFar;
		uint m_color;
		float m_density[2];
	};
}







#endif