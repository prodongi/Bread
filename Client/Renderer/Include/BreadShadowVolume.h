
#ifndef _BreadShadowVolume_h_
#define _BreadShadowVolume_h_

#include "BreadShadowBase.h"


namespace Bread
{
	/**
	* \ingroup Sample
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
	* \date 2010-03-05
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
	class cShadowVolume : public cShadowBase
	{
	public:
		cShadowVolume();
		virtual ~cShadowVolume();
		virtual void render();
		virtual void update(float elapsedtime);
		virtual bool initialize(uint width, uint height);

	private:
		void initLight();
		void initShader();
		void renderScene();
		void renderShadow();

	public:
		bool m_isScene;
	};

	//
	cShadowBase* createShadowVolume();
}





















#endif