
#ifndef _BreadNavigation_h_
#define _BreadNavigation_h_

/*
	BreadNavigation.h
	Copyright (c) prodongi. All rights resserved.

	date : 2012-10-23
	purpose : 
*/

#include "BreadDefinition.h"
#include "BreadVector3.h"

namespace Bread
{
	class cTerrain;

	/*
	*/
	struct sPathResult
	{
		~sPathResult()
		{
			clear();
		}
		void clear()
		{
			m_path.clear();
		}
		std::vector<sVector3> m_path;
	};
	/*
	*/
	class cNavigation
	{
	public:
		cNavigation();
		~cNavigation();
		bool pathFind(cTerrain* terrain, sVector3 const& startPos, uint startCell, 
					  sVector3 const& endPos, uint endCell, sPathResult* result);

	private:
		int m_sessionId;
	};
}

#endif