
#include "BreadTerrain.h"
#include "BreadDeclaration.h"
#include "BreadRenderer.h"
#include "BreadTerrainCell.h"
#include "BreadNavigation.h"

namespace Bread
{
	/*
	 *	
	 */
	cTerrain::cTerrain() : m_cell(NULL),
						   m_vertexNum(0), 
						   m_vertexList(NULL),
						   m_navigation(NULL)
	{
	}
	/*
	 *	
	 */
	cTerrain::~cTerrain()
	{
		safeDelete(m_cell);
		safeDeleteArray(m_vertexList);
	}
	/*
	*/
	void cTerrain::initialize()
	{
		m_navigation = new cNavigation;
	}
	/*
	*/
	void cTerrain::finalize()
	{
		safeDelete(m_navigation);
	}
	/*
	 *	
	 */
	void cTerrain::render()
	{
		cD3DSystem::getD3DDevice()->SetTexture(0, NULL);
		cD3DSystem::getD3DDevice()->SetFVF(D3DFVF_XYZ);

		// cell
		int cellNum = m_cell->getCellNum();
		for (int i = 0; i < cellNum; ++i)
		{
			cRenderer::renderIndexedPrimitiveUP(D3DPT_LINESTRIP, m_vertexNum, 2, m_cell->getIndex(i), D3DFMT_INDEX32, 
												m_vertexList, sizeof (sVector3));
		}

		//if (cOctree::m_isRender)
		//	cRenderer::renderOctree(&m_octree);
	}
	/*
	*/
	sVector3 const& cTerrain::getVector(uint vertIndex) const
	{
		assert(vertIndex < (uint)m_vertexNum);
		return m_vertexList[vertIndex];
	}
	/*
	*/
	uint const* cTerrain::getVertIndex(uint cellIndex) const
	{
		assert(m_cell);
		return m_cell->getIndex(cellIndex);
	}
	/*
	*/
	void cTerrain::createCell(uint cellNum, int cellType)
	{
		assert(!m_cell);
		assert(cellType != cTerrainCell::NONE_CELL);

		m_cell = cTerrainCell::create(cellType);
		m_cell->createCell(cellNum);
	}
	/*
	*/
	void cTerrain::createVertex(uint vertNum)
	{
		assert(!m_vertexList);

		m_vertexNum = vertNum;
		m_vertexList = new sVector3[vertNum];
	}
	/*
	*/
	bool cTerrain::pathFind(sVector3 const& startPos, uint startCell, sVector3 const& endPos, uint endCell, sPathResult* result)
	{
		return m_navigation->pathFind(this, startPos, startCell, endPos, endCell, result);
	}
	/*
	*/
	cTerrainCell* cTerrain::getCell() const
	{
		return m_cell;
	}
}