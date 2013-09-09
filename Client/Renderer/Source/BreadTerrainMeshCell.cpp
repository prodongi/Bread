
/*
	Bread$.cpp
	Copyright (c) prodongi. All rights resserved.
*/

#include "BreadTerrainMeshCell.h"
#include "BreadNavigationHeap.h"
#include "BreadTerrain.h"
#include "BreadTerrainCell.h"

namespace Bread
{
	/*
	*/
	float sTerrainNaviPathCell::getPathFindingCost() const
	{
		return m_arriveCost + m_heuristic;
	}
}