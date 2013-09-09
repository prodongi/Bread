
#include "BreadLightManager.h"
#include "BreadHelperClass.h"
#include "BreadCore.h"
#include "BreadGui.h"


namespace Bread
{
	implementInstance(cLightManager)

	/*
	 *	
	 */
	cLightManager::cLightManager()
	{
		assignInstance(this)
	}
	/*
	 *	
	 */
	cLightManager::~cLightManager()
	{
		assignInstance(NULL)
	}
	/*
	 *	
	 */
	void cLightManager::update(float elapsedtime)
	{
		std::for_each(m_list->begin(), m_list->end(), sUpdateFunctor<cLight>(elapsedtime));
	}
	/*
	*/
	int cLightManager::renderDebugInfo(int y)
	{
		cLight* light = get(0);
		if (!light) return y;
		_getFontMgr()->drawD3DText(10, y+=16, _T("[Light]"), cD3DFont::HEIGHT_14, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
		return light->renderDebugInfo(y);
	}
}