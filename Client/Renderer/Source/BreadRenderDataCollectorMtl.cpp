
#include "BreadRenderDataCollectorMtl.h"
#include "BreadRenderDataManager.h"

namespace Bread
{	
	/*
	 *	
	 */
	void cRenderDataCollectorMtl::collect(cRenderData const* renderData)
	{
		m_list.push_back(renderData->getBuid());
	}
	/*
	*/
	void cRenderDataCollectorMtl::render(cRenderDataManager const* rdMgr)
	{
		iter_col it = m_list.begin();
		for (; it != m_list.end(); ++it)
		{
			cRenderData* rd = rdMgr->get(*it);
			if (rd)
				rd->render();
		}
	}
}






















