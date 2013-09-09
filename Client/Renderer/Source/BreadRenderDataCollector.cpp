
#include "BreadRenderDataCollector.h"
#include "BreadBatchRenderManager.h"

namespace Bread
{
	/*
	*/
	cRenderDataCollector::~cRenderDataCollector()
	{
	}
	/*
	*/
	void cRenderDataCollector::update(float elapsedtime)
	{
		collect();
	}
	/*
	*/
	void cRenderDataCollector::collect()
	{
		struct sCollect
		{
			sCollect(cRenderDataCollector* collector) : m_collector(collector) {}
			void operator ()(cBatchRenderManager::sEntityList& rl)
			{
				m_collector->collect(rl.m_rd);
			}
			cRenderDataCollector* m_collector;
		};


		clearList();
		//_getBatchRenderMgr()->getEntityList()->forEach(sCollect(this));
		_getBatchRenderMgr()->forEachEntityList(sCollect(this));
		alphaSort();
	}
}






















