
#ifndef _BreadTerrainCell_h_
#define _BreadTerrainCell_h_

/*
	BreadTerrainCell.h
	Copyright (c) prodongi. All rights resserved.

	date : 2012-09-25
	purpose : 
*/

#include "BreadDefinition.h"
#include "BreadLine.h"

namespace Bread
{
	class cTerrain;
	class cSerializeBase;
	class cNavigationHeap;
	struct sTerrainHeightMeshCell;
	struct sTerrainNaviMeshCell;
	struct sTerrainNaviPathCell;
	
	/*
	*/
	class cTerrainCell
	{
	public:
		enum {	VERT_A, VERT_B, VERT_C };
		enum {	SIDE_AB, SIDE_BC, SIDE_CA };
		enum CELL_TYPE { NAVI_CELL, HEIGHT_CELL, NONE_CELL };

	public:
		cTerrainCell();
		virtual ~cTerrainCell();
		virtual void createCell(uint cellNum);
		virtual void readCellData(cSerializeBase* /*serialize*/) {}
		virtual uint const* getIndex(uint /*cellIndex*/) const	{	return  NULL;	}
		virtual bool queryPath(uint /*cellIndex*/, cNavigationHeap* /*heap*/, uint /*caller*/, float /*arriveCost*/) {	return false; }
		virtual bool processPath(cTerrain* /*terrain*/, uint /*cellIndex*/, cNavigationHeap* /*heap*/) {	return false;	}
		virtual uint getArriveWall(uint /*cellIndex*/) const	{	return 0;	}
		virtual uint getLink(uint /*cellIndex*/, uint /*link*/) const	{	return 0;	}
		virtual void getCenter(uint /*cellIndex*/, sVector3& /*center*/) const	{}
		virtual void getWallMidPoint(uint /*cellIndex*/, uint /*link*/, sVector3& /*midPoint*/) const {}
		uint getCellNum() const	{	return m_cellNum;	}

		static cTerrainCell* create(int cellType);

	protected:		
		uint m_cellNum;
	};
	/*
	*/
	class cTerrainHeightMeshCell : public cTerrainCell
	{
	public:
		cTerrainHeightMeshCell();
		virtual ~cTerrainHeightMeshCell();
		virtual void createCell(uint cellNum);
		virtual void readCellData(cSerializeBase* serialize);
		virtual uint const* getIndex(uint cellIndex) const;

	private:
		sTerrainHeightMeshCell* m_cellList;
	};
	/*
	*/
	class cTerrainNaviMeshCell : public cTerrainCell
	{
	public:
		cTerrainNaviMeshCell();
		virtual ~cTerrainNaviMeshCell();
		virtual void createCell(uint cellNum);
		virtual void readCellData(cSerializeBase* serialize);
		virtual uint const* getIndex(uint cellIndex) const;
		virtual bool queryPath(uint cellIndex, cNavigationHeap* heap, uint caller, float arriveCost);
		virtual bool processPath(cTerrain* terrain, uint cellIndex, cNavigationHeap* heap);
		virtual uint getArriveWall(uint cellIndex) const;
		virtual uint getLink(uint cellIndex, uint link) const;
		virtual void getCenter(uint cellIndex, sVector3& center) const;
		virtual void getWallMidPoint(uint cellIndex, uint link, sVector3& midPoint) const;

	private:
		float computeHeuristic(sVector3 const& goal, sVector3 const& center);

	private:
		sTerrainNaviMeshCell* m_cellList;
		sTerrainNaviPathCell* m_pathDataList;
	};
}

#endif