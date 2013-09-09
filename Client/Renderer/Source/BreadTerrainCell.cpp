
/*
	BreadTerrainCell.cpp
	Copyright (c) prodongi. All rights resserved.
*/

#include "BreadTerrainCell.h"
#include "BreadInline.h"
#include "BreadSerializeBase.h"
#include "BreadNavigationHeap.h"
#include "BreadTerrainMeshCell.h"
#include "BreadTerrain.h"

namespace Bread
{
	/*
	*/
	cTerrainCell* cTerrainCell::create(int cellType)
	{
		switch (cellType)
		{
		case NAVI_CELL:		return new cTerrainNaviMeshCell;
		case HEIGHT_CELL:	return new cTerrainHeightMeshCell;
		default:			return NULL;
		}
	}
	
	/*
	*/
	cTerrainCell::cTerrainCell() : m_cellNum(0)
	{
	}
	/*
	*/
	cTerrainCell::~cTerrainCell()
	{
	}
	/*
	*/
	void cTerrainCell::createCell(uint cellNum)
	{
		m_cellNum = cellNum;
	}

	/*
	*/
	cTerrainHeightMeshCell::cTerrainHeightMeshCell() : m_cellList(NULL)
	{
	}
	/*
	*/
	cTerrainHeightMeshCell::~cTerrainHeightMeshCell()
	{
		safeDeleteArray(m_cellList);
	}
	/*
	*/
	void cTerrainHeightMeshCell::createCell(uint cellNum)
	{
		cTerrainCell::createCell(cellNum);
		m_cellList = new sTerrainHeightMeshCell[cellNum];
	}
	/*
	*/
	void cTerrainHeightMeshCell::readCellData(cSerializeBase* serialize)
	{
		serialize->readValueList(m_cellList, m_cellNum);
	}
	/*
	*/
	uint const* cTerrainHeightMeshCell::getIndex(uint cellIndex) const
	{
		return m_cellList[cellIndex].m_index;
	}

	/*
	*/
	cTerrainNaviMeshCell::cTerrainNaviMeshCell() : m_cellList(NULL)
	{
	}
	/*
	*/
	cTerrainNaviMeshCell::~cTerrainNaviMeshCell()
	{
		safeDeleteArray(m_cellList);
		safeDeleteArray(m_pathDataList);
	}
	/*
	*/
	void cTerrainNaviMeshCell::createCell(uint cellNum)
	{
		cTerrainCell::createCell(cellNum);
		m_cellList = new sTerrainNaviMeshCell[cellNum];
		m_pathDataList = new sTerrainNaviPathCell[cellNum];
	}
	/*
	*/
	void cTerrainNaviMeshCell::readCellData(cSerializeBase* serialize)
	{
		serialize->readValueList<sTerrainNaviMeshCell>(m_cellList, m_cellNum);
	}
	/*
	*/
	uint const* cTerrainNaviMeshCell::getIndex(uint cellIndex) const
	{
		return m_cellList[cellIndex].m_index;
	}
	/*
	*/
	bool cTerrainNaviMeshCell::queryPath(uint cellIndex, cNavigationHeap* heap, uint caller, float arriveCost)
	{
		sTerrainNaviMeshCell* cell = m_cellList + cellIndex;
		sTerrainNaviPathCell* pathCell = m_pathDataList + cellIndex;

		if (pathCell->m_sessionId != heap->getSessionId())
		{
			pathCell->m_sessionId = heap->getSessionId();

			if (caller)
			{
				pathCell->m_open = true;
				pathCell->m_heuristic = computeHeuristic(heap->getGoal(), cell->m_center);
				pathCell->m_arriveCost = arriveCost;

				if (caller == cell->m_link[0])		pathCell->m_arriveWall = 0;
				else if (caller == cell->m_link[1])	pathCell->m_arriveWall = 1;
				else if (caller == cell->m_link[2])	pathCell->m_arriveWall = 2;
			}
			else
			{
				pathCell->m_open = false;
				pathCell->m_arriveCost = 0;
				pathCell->m_heuristic = 0;
				pathCell->m_arriveWall = 0;
			}

			heap->addCell(cell->m_cellIndex, pathCell->getPathFindingCost());
			return true;
		}
		// 방문한 셀 일 경우에
		else if (pathCell->m_open)
		{
			// const가 더 작으면
			if (arriveCost + pathCell->m_heuristic < pathCell->m_arriveCost + pathCell->m_heuristic)
			{
				pathCell->m_arriveCost = arriveCost;

				if (caller == cell->m_link[0])		pathCell->m_arriveWall = 0;
				else if (caller == cell->m_link[1])	pathCell->m_arriveWall = 1;
				else if (caller == cell->m_link[2])	pathCell->m_arriveWall = 2;

				heap->adjustCell(cell->m_cellIndex, pathCell->getPathFindingCost());
				return true;
			}
		}
		return false;
	}
	/*
	*/
	bool cTerrainNaviMeshCell::processPath(cTerrain* terrain, uint cellIndex, cNavigationHeap* heap)
	{
		sTerrainNaviMeshCell* cell = m_cellList + cellIndex;
		sTerrainNaviPathCell* pathCell = m_pathDataList + cellIndex;

		if (pathCell->m_sessionId == heap->getSessionId())
		{
			pathCell->m_open = false;

			for (int i = 0; i < 3; ++i)
			{
				if ((uint)-1 != cell->m_link[i])
				{
					float arriveCost = pathCell->m_arriveCost + cell->m_wallDistance[abs(i-pathCell->m_arriveWall)];
					terrain->getCell()->queryPath(cell->m_link[i], heap, cell->m_cellIndex, arriveCost);
				}
			}
			return true;
		}
		return false;
	}
	/*
	*/
	float cTerrainNaviMeshCell::computeHeuristic(sVector3 const& goal, sVector3 const& center)
	{
		float deltaX = cMath::fabsf(goal.x - center.x);
		float deltaY = cMath::fabsf(goal.y - center.y);
		float deltaZ = cMath::fabsf(goal.z - center.z);

		return cMath::_max(deltaX, cMath::_max(deltaY, deltaZ));
	}
	/*
	*/
	uint cTerrainNaviMeshCell::getArriveWall(uint cellIndex) const
	{
		return m_pathDataList[cellIndex].m_arriveWall;
	}
	/*
	*/
	uint cTerrainNaviMeshCell::getLink(uint cellIndex, uint link) const
	{
		return m_cellList[cellIndex].m_link[link];
	}
	/*
	*/
	void cTerrainNaviMeshCell::getCenter(uint cellIndex, sVector3& center) const
	{
		center = m_cellList[cellIndex].m_center;
	}
	/*
	*/
	void cTerrainNaviMeshCell::getWallMidPoint(uint cellIndex, uint link, sVector3& midPoint) const
	{
		midPoint = m_cellList[cellIndex].m_wallMidPoint[link];
	}
}