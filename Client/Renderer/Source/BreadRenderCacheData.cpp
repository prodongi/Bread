
#include "BreadRenderCacheData.h"
#include "BreadRenderData.h"

namespace Bread
{
	/*
	*/
	cRenderCacheData::cRenderCacheData()
	{
		m_mtlList = new bucket_mtl;
		m_meshList = new bucket_mesh;
	}
	/*
	*/
	cRenderCacheData::~cRenderCacheData()
	{
		safeDelete(m_mtlList);
		safeDelete(m_meshList);
	}
	/*
	*/
	void cRenderCacheData::copy(cRenderData* copyData, uint shareType)
	{
		copyData->m_buid = m_buid;
		m_cullFigure.copy(copyData->m_cullFigure);
		copyData->m_octree.copy(m_octree);
		m_mtlList->copy(*copyData->m_mtlList, shareType);
		m_meshList->copy(*copyData->m_meshList, shareType);
	}
}