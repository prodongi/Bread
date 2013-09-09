
#ifndef _BreadNavigationPath_h_
#define _BreadNavigationPath_h_

/*
	BreadNavigationPath.h
	Copyright (c) prodongi. All rights resserved.

	date : 2012-10-21
	purpose : �׺���̼� �޽��� ��ã�� ����� �����ϴ� Ŭ����
*/

#include "BreadVector3.h"
#include <list>

namespace Bread
{
	struct sTerrainNaviMeshCell;
	class cTerrainNaviMeshCell;

	/*
	*/
	class cNavigationPath
	{
	public:
		cNavigationPath();
		~cNavigationPath();

	private:
		struct sWayPoint
		{
			sVector3 m_pos;
			sTerrainNaviMeshCell* m_cll;
		};
		typedef std::list<sWayPoint> l_waypoint;
		typedef l_waypoint::const_iterator it_waypoint;
		
		cTerrainNaviMeshCell* m_parent;
		sWayPoint m_startPoint;
		sWayPoint m_endPoint;
		it_waypoint m_endId;
		l_waypoint m_wayPointList;

	};
}

#endif
