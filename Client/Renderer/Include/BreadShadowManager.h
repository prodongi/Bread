
#ifndef _BreadShadowManager_h_
#define _BreadShadowManager_h_

#include "BreadShadowBase.h"
#include "BreadObjectFactory.h"

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
	class cShadowManager
	{
		declareClassCreater(cShadowManager)

	public:
		virtual void registerObjectCallBack();
		virtual bool initialize(std_string const& type, uint width, uint height);
		virtual void update(float /*elapsedtime*/);
		virtual void render();
		virtual void beforeRender3D();
		virtual void afterRender3D();
		virtual void beforeRender2D();
		virtual void afterRender2D();
		virtual int renderDebugInfo(int y);

		std_string const& getType() const			{	return m_type;				}
		bool isType(std_string const& type) const	{	return m_type == type;	}
		cShadowBase* getShadow() const			{	return m_shadow;			}

		static std_string DEFAULT_SHADOW;

	protected:
		cShadowBase* createShadow(std_string const& type);

	protected:
		std_string m_type;
		cShadowBase* m_shadow;
		cObjectFactory<std_string, cShadowBase> m_objFactory;
	};
}












#endif




