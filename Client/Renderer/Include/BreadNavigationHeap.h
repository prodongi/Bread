
#ifndef _BreadNavigationHeap_h_
#define _BreadNavigationHeap_h_

/*
	BreadNavigationHeap.h
	Copyright (c) prodongi. All rights resserved.

	date : 2012-10-23
	purpose : 
*/

#include "BreadVector3.h"
#include "BreadNavigationNode.h"
#include <vector>
#include <functional>

namespace Bread
{
	struct sTerrainNaviMeshCell;

	/*
		노드의 비용 별로 저장해주는 클래스,
		여기서 heap은 메모리 영역이 아니고 자료 구조를 의미한다
		항상 내림 차순으로 정렬 된다
	*/
	class cNavigationHeap
	{
	public:
		typedef std::vector<cNavigationNode> vec_node;
		std::greater<cNavigationNode> comp;

	public:
		cNavigationHeap();
		~cNavigationHeap();
		void init(int sessionId, sVector3 const& goal);
		int getSessionId() const		{	return m_sessionId;			}
		sVector3 const& getGoal() const	{	return m_goal;				}
		bool notEmpty() const			{	return !m_nodes.empty();	}
		void addCell(uint cellIndex, float pathFindingCost);
		void adjustCell(uint cellIndex, float pathFindingCost);
		void getTop(cNavigationNode& node);

	private:
		cNavigationHeap::vec_node::iterator findNode(uint cellIndex);
		
	private:
		vec_node m_nodes;
		sVector3 m_goal;
		int m_sessionId;
	};
}

#endif