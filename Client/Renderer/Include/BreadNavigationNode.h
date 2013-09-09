
#ifndef _BreadNavigationNode_h_
#define _BreadNavigationNode_h_

/*
	BreadNavigationNode.h
	Copyright (c) prodongi. All rights resserved.

	date : 2012-10-23
	purpose : 
*/

#include "BreadMath.h"
#include "BreadDefinition.h"

namespace Bread
{
	class cNavigationNode
	{
	public:
		cNavigationNode();
		~cNavigationNode();
		propertyConstFunc(uint, Cell, m_cell)
		propertyConstFunc(float, Cost, m_cost)


		bool operator < (cNavigationNode const& node)
		{
			return m_cost < node.m_cost;
		}
		bool operator > (cNavigationNode const& node)
		{
			return m_cost > node.m_cost;
		}
		bool operator == (cNavigationNode const& node)
		{
			return (cMath::equal(m_cost, node.m_cost)) && (m_cell == node.m_cell);
		}
		bool operator == (uint cell)
		{
			return m_cell == cell;
		}

	private:
		float m_cost;
		uint m_cell;
	};

	inline bool operator < (cNavigationNode const& a, cNavigationNode const& b)
	{
		return a.getCost() < b.getCost();
	}
	inline bool operator > (cNavigationNode const& a, cNavigationNode const& b)
	{
		return a.getCost() > b.getCost();
	}
	inline bool operator == (cNavigationNode const& a, cNavigationNode const& b)
	{
		return (cMath::equal(a.getCost(), b.getCost())) && (a.getCell() == b.getCell());
	}
	inline bool operator == (uint cell, cNavigationNode const& node)
	{
		return cell == node.getCell();
	}
}

#endif