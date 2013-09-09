
#ifndef _BreadTerrain_h_
#define _BreadTerrain_h_

#include "BreadVector3.h"
#include "BreadOctree.h"

namespace Bread
{
	class cNavigation;
	class cTerrainCell;
	struct sPathResult;

	/**
	 * \ingroup Renderer
	 *
	 *
	 * \par requirements
	 * win98 or later\n
	 * win2k or later\n
	 * MFC\n
	 *
	 * \version 1.0
	 * first version
	 *
	 * \date 2010-01-14
	 *
	 * \author Prodongi
	 *
	 * \par license
	 * This code is absolutely free to use and modify. The code is provided "as is" with
	 * no expressed or implied warranty. The author accepts no liability if it causes
	 * any damage to your computer, causes your pet to fall ill, increases baldness
	 * or makes your car start emitting strange noises when you start it up.
	 * This code has no bugs, just undocumented features!
	 * 
	 * \todo 
	 *
	 * \bug 
	 *
	 */
	class cTerrain
	{
	public:
		cTerrain();
		virtual ~cTerrain();
		virtual void initialize();
		virtual void finalize();
		void render();
		cOctree const& getOctree() const {	return m_octree; }
		sVector3 const& getVector(uint vertIndex) const;
		uint const* getVertIndex(uint cellIndex) const;
		void createCell(uint cellNum, int cellType);
		void createVertex(uint vertNum);
		bool pathFind(sVector3 const& startPos, uint startCell, sVector3 const& endPos, uint endCell, sPathResult* result);
		cTerrainCell* getCell() const;

	private:
		cTerrainCell* m_cell;
		int m_vertexNum;
		sVector3* m_vertexList;
		cOctree m_octree;
		cNavigation* m_navigation;

		friend class cSerialize3D;
	};
}












#endif

















