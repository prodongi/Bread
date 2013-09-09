
#ifndef _BreadRenderData_h_
#define _BreadRenderData_h_

#include "BreadDeclaration.h"
#include "BreadVector3.h"
#include "BreadMatrix4.h"
#include "BreadBounding.h"
#include "BreadOctree.h"
#include "BreadBucketMap.h"
#include "BreadMaterial.h"
#include "BreadMesh.h"

namespace Bread
{
	class cEntityModel;

	/*
	*	
	*/
	struct sFace
	{
		enum {	VERT_NUM = 3	};
		float minx() const	{	return min(min(m_v[0].x, m_v[1].x), m_v[2].x);	}
		float maxx() const	{	return max(max(m_v[0].x, m_v[1].x), m_v[2].x);	}
		float miny() const	{	return min(min(m_v[0].y, m_v[1].y), m_v[2].y);	}
		float maxy() const	{	return max(max(m_v[0].y, m_v[1].y), m_v[2].y);	}
		float minz() const	{	return min(min(m_v[0].z, m_v[1].z), m_v[2].z);	}
		float maxz() const	{	return max(max(m_v[0].z, m_v[1].z), m_v[2].z);	}

		sVector3 m_v[VERT_NUM];
	};
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
	 * \date 2009-09-30
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
	class cRenderData
	{
	public:
		cRenderData(BUID buid);
		cRenderData();
		~cRenderData();
		/// @brief collector를 쓰지않고 직접 렌더링할때
		void render();
		void setTranslation(sVector3 const& pos, sMatrix4 const& matRot);
		BUID const& getBuid() const	{	return m_buid;	}
		/// @brief map buid list를 구한다
		void getMapBuidList(int mapType, std::vector<BUID>& list, bool first = false);
		void copy(cRenderData* copyData, uint shareType);

		cMesh* getMesh(BUID const& key) const		{	return m_meshList->get(key);		}
		cMaterial* getMtl(BUID const& key) const	{	return m_mtlList->get(key);		}

		friend class cRenderCacheData;

	public:
		/// @brief m_buid는 파일 이름과 같은 값이다
		BUID			m_buid;
		cEntityModel*	m_entity;
		sCullFigure		m_cullFigure;
		cOctree			m_octree;
		/// @brief scene에 현재 객체가 속한 옥트리의 노드
		cOctreeNode*	m_sceneOctreeNode;

	public:
		typedef cBucketMap<BUID, cMaterial*>	bucket_mtl;
		typedef cBucketMap<BUID, cMesh*>		bucket_mesh;

		bucket_mtl* getMtlList()	{	return m_mtlList;	}
		bucket_mesh* getMeshList()	{	return m_meshList;	}

	private:
		bucket_mtl* m_mtlList;
		bucket_mesh* m_meshList;
	};
}







#endif