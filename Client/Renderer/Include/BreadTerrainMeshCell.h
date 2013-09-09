
#ifndef _BreadTerrainMeshCell_h_
#define _BreadTerrainMeshCell_h_

/*
	Bread$.h
	Copyright (c) prodongi. All rights resserved.

	date : 2012-09-dd
	purpose : 
*/

#include "BreadDefinition.h"
#include "BreadVector3.h"

namespace Bread
{	
	class cNavigationHeap;
	class cTerrain;

#pragma pack(push, 1)
	/*
	*/
	struct sTerrainHeightMeshCell
	{
		uint m_index[3];
		sVector3 m_normal;
	};
	/*
	*/
	struct sTerrainNaviMeshCell
	{
		struct sSide
		{
			uint m_v1;
			uint m_v2;
		};
		uint m_cellIndex;
		sVector3 m_normal;
		float m_distance;
		uint m_index[3];
		// center point
		sVector3 m_center;
		// side
		sSide m_side[3];
		// link
		uint m_link[3];
		sVector3 m_wallMidPoint[3];
		float m_wallDistance[3];
		
	};
	/* 길찾기 할 때만 쓰이는 구조체
	*/
	struct sTerrainNaviPathCell
	{
		float getPathFindingCost() const;

		int m_sessionId;
		float m_arriveCost;
		float m_heuristic;
		int m_arriveWall;
		bool m_open;
	};
#pragma pack(pop)
}

#endif