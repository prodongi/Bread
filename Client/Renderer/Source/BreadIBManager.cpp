
#include "BreadIBManager.h"
#include "BreadD3DSystemClassify.h"
#include "BreadRenderer.h"
#include "BreadUtils.h"
#include "BreadStringHelper.h"

namespace Bread
{
	implementInstance(cIBManager)

	/*
	 *	
	 */
	 TCHAR const* cIBManager::makeBUID(TCHAR const* param1, TCHAR const* param2, uint identity)
	{
		static TCHAR temp[NAME_LEN];
		TCHAR const* buid = _getStringHelper()->makeBUID(param1, param2);
		_stprintf(temp, _T("%s_%d"), buid, identity);
		return temp;
	}

	/*
	 *	
	 */
	cIBManager::cIBManager()
	{
		assignInstance(this)
	}
	/*
	 *	
	 */
	cIBManager::~cIBManager()
	{
		assignInstance(NULL)
	}
	/*
	 *	
	 */
	bool cIBManager::create(sIBInfo* ibInfo, char const* buffer)
	{
		if (is(ibInfo->m_buid))
			return true;

		if (!cRenderer::createIndexBuffer(ibInfo, (void*)buffer))
			return false;

		insert(ibInfo->m_buid, &ibInfo);

		return true;
	}
	/*
	*/
	bool cIBManager::create(sIBInfo* ibInfo)
	{
		if (is(ibInfo->m_buid))
			return true;

		if (!cRenderer::createIndexBuffer(ibInfo))
			return false;

		insert(ibInfo->m_buid, &ibInfo);

		return true;
	}
}















