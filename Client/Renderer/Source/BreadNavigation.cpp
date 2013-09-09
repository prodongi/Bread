
/*
	BreadNavigation.cpp
	Copyright (c) prodongi. All rights resserved.
*/

#include "BreadNavigation.h"
#include "BreadNavigationHeap.h"
#include "BreadTerrain.h"
#include "BreadTerrainCell.h"

namespace Bread
{
	/*
	*/
	cNavigation::cNavigation() : m_sessionId(-1)
	{
	}
	/*
	*/
	cNavigation::~cNavigation()
	{
	}
	/*
	*/
	bool cNavigation::pathFind(cTerrain* terrain, 
							   sVector3 const& startPos, uint startCell, 
							   sVector3 const& endPos, uint endCell, sPathResult* result)
	{
		++m_sessionId;

		cNavigationHeap heap;
		heap.init(m_sessionId, startPos);

		cTerrainCell* terrainCell = terrain->getCell();
		terrainCell->queryPath(endCell, &heap, 0, 0);

		bool foundPath = false;
		while (heap.notEmpty() && !foundPath)
		{
			cNavigationNode node;
			heap.getTop(node);

			if (node.getCell() == startCell)
			{
				foundPath = true;
			}
			else
			{
				terrainCell->processPath(terrain, node.getCell(), &heap);
			}
		}

		if (foundPath)
		{
			result->clear();

			sVector3 midPoint;
			uint testCell = startCell;
			//result->m_path.push_back(startPos);
			while (testCell && testCell != endCell)
			{
				int linkWall = terrainCell->getArriveWall(testCell);
				terrainCell->getWallMidPoint(testCell, linkWall, midPoint);
				result->m_path.push_back(midPoint);
				
				testCell = terrainCell->getLink(testCell, linkWall);
			}
			result->m_path.push_back(endPos);
		}

		return foundPath;
	}
}