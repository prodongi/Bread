
#ifndef _BreadOctreeNode_h_
#define _BreadOctreeNode_h_

#include "BreadStdHeader.h"
#include "BreadRenderData.h"
#include <fstream>

namespace Bread
{
	class cFrustum;

	/**
	* \ingroup BreadMaxExport
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
	* \date 2009-11-04
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
	class cOctreeNode
	{
	public:
		/// @brief L: left, R: right, T: top, B:bottom, U:up, D:down
		enum {	LTU, RTU, LBU, RBU, LTD, RTD, LBD, RBD, REGION_NUM	};
	public:
		cOctreeNode(cOctreeNode* parent);
		cOctreeNode();
		~cOctreeNode();

		void create(int depth, sBox const* in_bound);
		/// @brief 확장용
		bool add(cRenderData* renderData, sBox const* in_bound, float k, int maxDepth = -1);
		/// @brief 비확장용
		bool add(cRenderData* renderData, sBox const* in_bound, bool isSceneObject, int maxDepth = -1);
		void add(cOctreeNode const* node);
		void del(BUID const& buid);
		void copy(cOctreeNode const* srcData);
		void expandK(float k);
		void culling(cFrustum const* frustum);
		bool isIn(sVector3 const& position)		{	return m_bound.isIn(position);	}
		/// @brief pos에 해당되는 노드를 재 설정한다
		void reconstruct(cRenderData* renderData, sBox const* box);
		int getDepth() const							{	return m_depth;			}
		void setParent(cOctreeNode* parent)				{	m_parent = parent;		}
		cOctreeNode* getParent() const					{	return m_parent;		}
		cOctreeNode const* getNode(int region) const	{	return m_node[region];	}
		sBox const* getBound() const					{	return &m_bound;		}

		void read(cSerializeBase* serialize);

	private:
		void getRegion(int region, sBox* bound);
		/// @brief k만큼 확장해야 될때(익스포트할때 쓰이고 있다)
		bool isIn(int region, sBox const* in_bound, float k);
		bool isIn(int region, sBox const* in_bound);
		/// @brief 컬링 체크 안하고 m_rdList를 모두 추가한다
		void notculling();

	public:
		static uint m_renderColor;

	private:
		int m_depth;
		sBox m_bound;
		cOctreeNode* m_parent;
		cOctreeNode* m_node[REGION_NUM];
	public:
		/// @brief <render data BUID, isSceneObject>
		typedef std::map<BUID, bool>	m_rdlist;
		typedef m_rdlist::iterator		 it_rdlist;
		typedef m_rdlist::const_iterator cit_rdlist;
		m_rdlist m_rdList;
	};
}









#endif