
#include "BreadSerializeBmd.h"
#include "BreadRenderData.h"
#include "BreadRenderCacheData.h"
#include <fstream>
#include "BreadUtils.h"
#include "BreadAnimationJoint.h"
#include "BreadAnimation.h"
#include "BreadFileHelper.h"

namespace Bread
{
	/*
	*/
	void cSerializeBmd::makeFileName(BUID const& buid, std_string& filename)
	{
		filename = buid + _T(".bmd");
	}

	/*
	*/
	cSerializeBmd::sData::~sData()
	{
		safeDelete(m_octree);
		safeDelete(m_renderData);
		safeDelete(m_animation);
	}
	/*
	*/
	void cSerializeBmd::sData::copy(sData* copyData, uint shareType)
	{
		copyData->m_buid = m_buid;

		copyData->m_renderData = new cRenderData;
		m_renderData->copy(copyData->m_renderData, shareType);

		if (m_octree)
		{
			copyData->m_octree = new cOctree;
			copyData->m_octree->copy(*m_octree);
		}

		m_cullFigure.copy(copyData->m_cullFigure);

		if (m_animation)
		{
			copyData->m_animation = new cAnimation;
			m_animation->copy(copyData->m_animation);
		}
	}
	
	/*
	*/
	cSerializeBmd::sCacheData::~sCacheData()
	{
		safeDelete(m_octree);
		safeDelete(m_renderData);
		safeDelete(m_animation);
	}
	/*
	*/
	void cSerializeBmd::sCacheData::copy(sData* copyData, uint shareType)
	{
		copyData->m_buid = m_buid;

		if (m_renderData)
		{
			copyData->m_renderData = new cRenderData;
			m_renderData->copy(copyData->m_renderData, shareType);
		}

		if (m_octree)
		{
			copyData->m_octree = new cOctree;
			copyData->m_octree->copy(*m_octree);
		}

		m_cullFigure.copy(copyData->m_cullFigure);

		if (m_animation)
		{
			copyData->m_animation = new cAnimation;
			copyData->m_animation->copy(m_animation);
		}
	}

	/*
	*	
	*/
	cSerializeBmd::cSerializeBmd()
	{
	}
	/*
	*	
	*/
	cSerializeBmd::~cSerializeBmd()
	{
	}
	/*
	 *	
	 */
	bool cSerializeBmd::load(std_string const& filename, sData& data)
	{
		if (!open(filename))
			return false;

		assert(!data.m_octree);
		assert(!data.m_renderData);

		data.m_buid = cSerialize3D::makeBUID(filename);
		data.m_octree = new cOctree;
		data.m_renderData = new cRenderData;

		readVersion();
		readOctree(data.m_octree);
		readCullFigure(&data.m_cullFigure);
		readAnimation(&data.m_animation);
		readRenderData(data.m_renderData);

		close();

		return true;
	}
	/*
	*/
	bool cSerializeBmd::load(std_string const& filename, sCacheData& data)
	{
		if (!open(filename))
			return false;

		assert(!data.m_octree);
		assert(!data.m_renderData);

		data.m_buid = cSerialize3D::makeBUID(filename);
		data.m_octree = new cOctree;
		data.m_renderData = new cRenderCacheData;

		readVersion();
		readOctree(data.m_octree);
		readCullFigure(&data.m_cullFigure);
		readAnimation(&data.m_animation);
		readRenderData(data.m_renderData);

		close();

		return true;
	}
}





















