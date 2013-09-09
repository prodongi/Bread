
#include "BreadVBManager.h"
#include "BreadD3DSystemClassify.h"
#include "BreadRenderer.h"
#include "BreadUtils.h"
#include "BreadStringHelper.h"

namespace Bread
{
	implementInstance(cVBManager)

	/*
	 *	
	 */
	 TCHAR const* cVBManager::makeBUID(TCHAR const* param1, TCHAR const* param2, uint identity)
	{
		static TCHAR temp[NAME_LEN];
		TCHAR const* buid = _getStringHelper()->makeBUID(param1, param2);
		_stprintf(temp, _T("%s_%d"), buid, identity);
		return temp;
	}

	/*
	 *	
	 */
	cVBManager::cVBManager()
	{
		assignInstance(this)
	}
	/*
	 *	
	 */
	cVBManager::~cVBManager()
	{
		assignInstance(NULL)
	}
	/*
	 *	
	 */
	bool cVBManager::create(sVBInfo* vbInfo, char const* buffer)
	{
		if (is(vbInfo->m_buid))
			return true;

		if (!cRenderer::createVertexBuffer(vbInfo, (void*)buffer))
			return false;

		insert(vbInfo->m_buid, &vbInfo);

		return true;
	}
	/*
	*/
	bool cVBManager::create(sVBInfo* vbInfo)
	{
		if (is(vbInfo->m_buid))
			return true;

		if (!cRenderer::createVertexBuffer(vbInfo))
			return false;

		insert(vbInfo->m_buid, &vbInfo);

		return true;
	}
}















