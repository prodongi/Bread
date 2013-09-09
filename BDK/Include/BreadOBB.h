
#ifndef _BreadOBB_h_
#define _BreadOBB_h_

#include "BreadVector3.h"
#include "BreadMatrix3.h"


namespace Bread
{
	struct sRay;
	struct sPlane;

	/**
	 * \class cOBB
	 * \date 2008-10-09
	 * \author prodongi
	 */
	class cOBB
	{
	public:
		cOBB(){}
		cOBB(const sVector3& center, const sMatrix3& rotation, const sVector3& extent)
		{
			m_center = center;
			m_rotation = rotation;
			m_extent = extent;
		}
		~cOBB(){}

		void set(const sVector3& center, const sVector3& dir, const sVector3& extent);
		bool collision(const cOBB& other) const;
		bool collision(const sRay& ray) const;
		float collision(const sPlane& plane) const;
		void set(const sVector3& center, const sMatrix3& rotation, const sVector3& extent);

	public:
		sVector3 m_center;
		sMatrix3 m_rotation;
		sVector3 m_extent;
	};
}







#endif











