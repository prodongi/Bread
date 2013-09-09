
/*
	BreadNavigationHeap.cpp
	Copyright (c) prodongi. All rights resserved.
*/

#include "BreadNavigationHeap.h"
#include "BreadTerrainMeshCell.h"

namespace Bread
{
	/*
	*/
	cNavigationHeap::cNavigationHeap()
	{
	}
	/*
	*/
	cNavigationHeap::~cNavigationHeap()
	{
		m_nodes.clear();
	}
	/*
	*/
	void cNavigationHeap::init(int sessionId, sVector3 const& goal)
	{
		m_sessionId = sessionId;
		m_goal = goal;
		m_nodes.clear();
	}
	/*
	*/
	void cNavigationHeap::addCell(uint cellIndex, float pathFindingCost)
	{
		cNavigationNode node;
		node.setCell(cellIndex);
		node.setCost(pathFindingCost);

		m_nodes.push_back(node);
		std::push_heap(m_nodes.begin(), m_nodes.end(), comp);
	}
	/*
	*/
	void cNavigationHeap::adjustCell(uint cellIndex, float pathFindingCost)
	{
		vec_node::iterator it = findNode(cellIndex);
		if (it != m_nodes.end())
		{
			it->setCell(cellIndex);
			it->setCost(pathFindingCost);

			std::push_heap(m_nodes.begin(), it+1, comp);
		}
	}
	/*
	*/
	void cNavigationHeap::getTop(cNavigationNode& node)
	{
		node = m_nodes.front();
		std::pop_heap(m_nodes.begin(), m_nodes.end(), comp);
		m_nodes.pop_back();
	}
	/*
	*/
	cNavigationHeap::vec_node::iterator cNavigationHeap::findNode(uint cellIndex)
	{
		return std::find(m_nodes.begin(), m_nodes.end(), cellIndex);
	}
}