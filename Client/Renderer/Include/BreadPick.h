
#ifndef _BreadPick_h_
#define _BreadPick_h_

#include "BreadDeclaration.h"
#include "BreadVector3.h"
#include "BreadRay.h"
#include "BreadOctree.h"


namespace Bread
{
	class cCamera;
	class cOctreeNode;
	class cTerrain;

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
	 * \date 2010-01-15
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
	 * \todo 기본 알고리즘은 d3d sample에서 가져옵니다.
	 *
	 * \bug 
	 *
	 */
	class cPick
	{
	public:
		cPick();
		~cPick();
		bool begin(cCamera* camera, int mx, int my);
		void pickScene();
		/// @brief 마우스에 의해서 픽킹된 지형의 페이스 인덱스를 구한다
		bool pickTerrainFace(cTerrain const* terrain);
		/// @brief 픽킹된 위치의 높이를 구한다.
		bool pickTerrainHeight(cTerrain const* terrain, sVector3 const& orig, float& height, uint& cellIndex);
		void render(cTerrain const* terrain);
		int renderDebugInfo(int y);
		sVector3 const& getPickPos() const	{	return m_terrainResult.m_pickPos;	}
		uint getPickCellIndex() const		{	return m_terrainResult.m_cellIndex;	}

		propertyConstFunc(int, Type, m_type)

	private:
		typedef std::vector<cOctreeNode const*>	v_octreeNode;
		typedef v_octreeNode::iterator			it_octreeNode;
		typedef v_octreeNode::const_iterator	cit_octreeNode;
		typedef std::vector<BUID>				v_buid;
		typedef v_buid::const_iterator			cit_buid;

		struct sPickResult
		{
			enum { OCTREE_CONTAINER_SIZE = 20	};
			sPickResult()
			{
				m_octreeList.reserve(OCTREE_CONTAINER_SIZE);
				m_pickPos = sVector3::ZERO;
				m_cellIndex = (uint)-1;
			}
			~sPickResult()
			{
				clear();
			}
			void clear()
			{
				m_picked = false;
				m_octreeList.clear();
			}
			void setFind(DWORD cellIndex, float t, float u, float v, sVector3 const& v0, sVector3 const& v1, sVector3 const& v2)
			{
				m_t = t;
				m_u = u;
				m_v = v;
				m_cellIndex = cellIndex;
				m_pickVertex[0] = v0;
				m_pickVertex[1] = v1;
				m_pickVertex[2] = v2;

				sVector3 oa, ob;
				subVec3(v1, v0, oa);
				subVec3(v2, v0, ob);
				mulVec3(oa, m_u, oa);
				mulVec3(ob, m_v, ob);

				addVec3(oa, ob, m_pickPos);
				addVec3(m_pickPos, v0, m_pickPos);
			}

			v_octreeNode m_octreeList;
			float m_t, m_u, m_v;
			sVector3 m_pickVertex[3];
			sVector3 m_pickPos;
			uint m_cellIndex;
			bool m_picked;
		};

		/// @param dirIsTop dir.y만 세팅되어 있은 경우에 true로 설정해주면 빠르다. 지형의 높이를 구할때 true로 세팅하면 된다.
		bool pickOctreeNode(sRay const& ray, cOctreeNode const* octreeNode, sPickResult& pickResult, bool dirIsTop);
		bool pickBox(sRay const& ray, sBox const* box);
		bool pickTerrainFace(cTerrain const* terrain, sRay const& ray, sPickResult& pickResult, 
						     bool dirIsTop, bool checkIsInFace);
		bool intersectTriangle(sRay const& ray, sVector3 const& _v0, sVector3 const& _v1, sVector3 const& _v2,
								float& t, float& u, float& v);

	public:
		enum{	FIRST, CLOSER, ALL };

	private:
		int m_type;
		sRay m_ray;
		sPickResult m_terrainResult;
	};
}











#endif