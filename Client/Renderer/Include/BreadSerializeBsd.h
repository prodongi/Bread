
#ifndef _BreadSerializeBsd_h_
#define _BreadSerializeBsd_h_

#include "BreadSerialize3D.h"
#include "BreadRenderData.h"
#include "BreadTerrain.h"

namespace Bread
{
	class cAnimation;

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
	* \date 2009-09-27
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
	class cSerializeBsd : public cSerialize3D
	{
	public:
		struct sData
		{
			/*
			*/
			struct sRenderData
			{
				BUID	 m_buid;
				sVector3 m_position;
			};

			/*
			*/
			sData() : m_octree(NULL), m_terrain(NULL) {}
			~sData()
			{
				m_rdList.clear();
				safeDelete(m_octree);
				safeDelete(m_terrain);
			}
			void copy(sData* data);
			uint getRdSize() const				{	return (uint)m_rdList.size();	}
			BUID const& getBuid() const			{	return m_buid;					}
			propertyFunc(cOctree*, Octree, m_octree)
			propertyFunc(cTerrain*, Terrain, m_terrain)
			cOctree* getOctreeOnce();	/// 이 함수는 한번만 호출 되어야 된다.
			cTerrain* getTerrainOnce();	/// 이 함수는 한번만 호출 되어야 된다.
			BUID m_buid;
			std::vector<sRenderData> m_rdList;
			sCullFigure m_cullFigure;

		private:
			cOctree* m_octree;
			cTerrain* m_terrain;
		};
	public:
		cSerializeBsd();
		virtual ~cSerializeBsd();

		bool load(std_string const& filename, sData& data);
	};
}










#endif