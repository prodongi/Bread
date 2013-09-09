
#include "BreadSerializeBsd.h"
#include "BreadRenderData.h"
#include <fstream>
#include "BreadUtils.h"
#include "BreadAnimationJoint.h"
#include "BreadFileHelper.h"

namespace Bread
{
	/*
	*/
	cOctree* cSerializeBsd::sData::getOctreeOnce()
	{
		cOctree* temp = m_octree;
		m_octree = NULL;
		return temp;
	}
	/*
	*/
	cTerrain* cSerializeBsd::sData::getTerrainOnce()
	{
		cTerrain* temp = m_terrain;
		m_terrain = NULL;
		return temp;
	}
	/*
	*/
	void cSerializeBsd::sData::copy(sData* data)
	{
		data = data;
	}

	/*
	*	
	*/
	cSerializeBsd::cSerializeBsd()
	{
	}
	/*
	*	
	*/
	cSerializeBsd::~cSerializeBsd()
	{
	}
	/*
	 *	
	 */
	bool cSerializeBsd::load(std_string const& filename, sData& data)
	{
		if (!open(filename))
			return false;

		data.m_buid = cSerialize3D::makeBUID(filename);

		data.setOctree(new cOctree);
		data.setTerrain(new cTerrain);

		readVersion();
		readOctree(data.getOctree());
		readCullFigure(&data.m_cullFigure);
		uint rdNum;
		readValue(rdNum);
		for (uint i = 0; i < rdNum; ++i)
		{
			sData::sRenderData rd;
			readBUIDW(rd.m_buid);
			readVector3(rd.m_position);
			data.m_rdList.push_back(rd);
		}
		readTerrain(data.getTerrain());

		close();

		return true;
	}
}





















