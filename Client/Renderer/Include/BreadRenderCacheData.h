
#ifndef _BreadRenderCacheData_h_
#define _BreadRenderCacheData_h_

#include "BreadDeclaration.h"
#include "BreadVector3.h"
#include "BreadBounding.h"
#include "BreadOctree.h"
#include "BreadBucketMap.h"
#include "BreadMaterial.h"
#include "BreadCacheMesh.h"

namespace Bread
{
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
	class cRenderCacheData
	{
	public:
		typedef cBucketMap<BUID, cMaterial*>	bucket_mtl;
		typedef cBucketMap<BUID, cCacheMesh*>	bucket_mesh;

	public:
		cRenderCacheData();
		~cRenderCacheData();
		BUID const& getBuid() const	{	return m_buid;	}
		void copy(cRenderData* copyData, uint shareType);

	public:
		/// @brief m_buid는 파일 이름과 같은 값이다
		BUID			m_buid;
		sCullFigure		m_cullFigure;
		cOctree			m_octree;
		/// @brief scene에 현재 객체가 속한 옥트리의 노드
		cOctreeNode*	m_sceneOctreeNode;

		bucket_mtl* getMtlList()	{	return m_mtlList;	}
		bucket_mesh* getMeshList()	{	return m_meshList;	}

	private:
		bucket_mtl* m_mtlList;
		bucket_mesh* m_meshList;
	};
}







#endif